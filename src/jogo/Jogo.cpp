#include "../../include/jogo/Jogo.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cctype>
#include <vector>

// Quebra "Adaga de prata;w;1;1;1" em {"Adaga de prata","w","1","1","1"}.
static vector<string> dividir(string texto, char separador) {
    vector<string> partes;
    string atual = "";

    for (size_t i = 0; i < texto.size(); i++) {
        if (texto[i] == separador) {
            partes.push_back(atual);
            atual = "";
        } else {
            atual += texto[i];
        }
    }
    partes.push_back(atual);
    return partes;
}

Jogo::Jogo() {
    jogador = nullptr;
    cenaAtual = 1;
}

Jogo::~Jogo() {
    delete jogador;
}

// ------------------------------------------------------------------
// Entrada e telas
// ------------------------------------------------------------------

// Mostra na tela o conteudo de um arquivo de texto (usa ifstream + getline).
void Jogo::exibirArquivo(string caminho) {
    ifstream arquivo;
    arquivo.open(caminho);

    if (arquivo.fail()) {
        cout << "Nao foi possivel abrir " << caminho << endl;
        return;
    }

    string linha;
    while (getline(arquivo, linha)) {
        cout << linha << endl;
    }

    arquivo.close();
}

// Le uma linha inteira e converte para numero. Retorna -1 se nao for um
// numero valido. Ler a linha toda evita o problema de misturar cin >> com getline.
int Jogo::lerInteiro() {
    string linha;

    if (!getline(cin, linha)) {
        // Entrada encerrada (Ctrl+Z / Ctrl+D): sai do programa.
        cout << endl << "Entrada encerrada. Ate a proxima!" << endl;
        exit(0);
    }

    if (linha.empty() || linha.size() > 6) {
        return -1;
    }

    for (size_t i = 0; i < linha.size(); i++) {
        if (!isdigit(static_cast<unsigned char>(linha[i]))) {
            return -1;
        }
    }

    return stoi(linha);
}

void Jogo::mostrarCreditos() {
    exibirArquivo("telas/creditos.txt");
    cout << "Pressione ENTER para voltar...";
    string descarte;
    getline(cin, descarte);
}

// Tela de criacao: mostra as regras (arquivo) e le a distribuicao dos 12 pontos.
void Jogo::criarPersonagem() {
    exibirArquivo("telas/criacao.txt");

    cout << "Nome do personagem: ";
    string nome;
    getline(cin, nome);
    if (nome.empty()) {
        nome = "Aventureiro";
    }

    int habilidadeExtra, energiaExtra, sorteExtra;

    while (true) {
        cout << "Pontos extras em HABILIDADE (0-6): ";
        habilidadeExtra = lerInteiro();
        cout << "Pontos extras em ENERGIA (0-12): ";
        energiaExtra = lerInteiro();
        cout << "Pontos extras em SORTE (0-6): ";
        sorteExtra = lerInteiro();

        if (habilidadeExtra < 0 || habilidadeExtra > 6 ||
            energiaExtra < 0 || energiaExtra > 12 ||
            sorteExtra < 0 || sorteExtra > 6) {
            cout << endl << "Valor fora dos limites. Tente de novo." << endl << endl;
        }
        else if (habilidadeExtra + energiaExtra + sorteExtra != 12) {
            cout << endl << "A soma deve ser exatamente 12 (voce usou "
                 << habilidadeExtra + energiaExtra + sorteExtra << "). Tente de novo." << endl << endl;
        }
        else {
            break;
        }
    }

    // Se ja havia um personagem (novo jogo depois de outro), libera o antigo.
    delete jogador;
    jogador = new Jogador(nome, 6 + habilidadeExtra, 12 + energiaExtra, 6 + sorteExtra);

    cout << endl;
    mostrarFicha();
}

// Ficha do personagem (tela de inventario, versao inicial).
void Jogo::mostrarFicha() {
    cout << "------------ " << jogador->getNome() << " ------------" << endl;
    cout << "Nivel:      " << jogador->getNivel()
         << "  (experiencia " << jogador->getExperiencia() << "/"
         << jogador->getExperienciaParaProximoNivel() << ")" << endl;
    cout << "Habilidade: " << jogador->getHabilidade() << endl;
    cout << "Energia:    " << jogador->getEnergia() << "/" << jogador->getEnergiaMaxima() << endl;
    cout << "Sorte:      " << jogador->getSorte() << endl;
    cout << "Provisoes:  " << jogador->getProvisoes() << endl;
    cout << "Ouro:       " << jogador->getOuro() << endl;

    Arma* arma = jogador->getArmaEquipada();
    if (arma != nullptr) {
        cout << "Arma:       " << arma->imprimeInfo() << endl;
    } else {
        cout << "Arma:       nenhuma" << endl;
    }

    if (jogador->getPontosEvolucao() > 0) {
        cout << "Pontos de evolucao para distribuir: " << jogador->getPontosEvolucao() << endl;
    }
    cout << "-----------------------------------" << endl;
}

// Ficha + acoes que so podem ser feitas fora de combate.
void Jogo::menuFicha() {
    bool voltar = false;

    while (!voltar) {
        mostrarFicha();
        cout << "  1 - Comer uma provisao (+4 de energia)" << endl;
        cout << "  2 - Distribuir pontos de evolucao" << endl;
        cout << "  0 - Voltar para a historia" << endl;
        cout << "> ";

        int escolha = lerInteiro();

        if (escolha == 1) {
            if (jogador->usarProvisao()) {
                cout << "Voce comeu uma provisao e recuperou energia." << endl;
            } else {
                cout << "Nao foi possivel: voce nao tem provisoes ou a energia ja esta cheia." << endl;
            }
        }
        else if (escolha == 2) {
            if (jogador->podeEvoluir()) {
                distribuirPontos();
            } else {
                cout << "Nao ha pontos para distribuir." << endl;
            }
        }
        else if (escolha == 0) {
            voltar = true;
        }
        else {
            cout << "Opcao invalida." << endl;
        }
    }
}

// Cada ponto de evolucao aumenta em 1 um atributo, respeitando os limites do enunciado.
void Jogo::distribuirPontos() {
    while (jogador->podeEvoluir()) {
        cout << endl << "Pontos para distribuir: " << jogador->getPontosEvolucao() << endl;
        cout << "  1 - Habilidade (" << jogador->getHabilidade() << "/" << Jogador::MAX_HABILIDADE << ")" << endl;
        cout << "  2 - Energia    (" << jogador->getEnergiaMaxima() << "/" << Jogador::MAX_ENERGIA << ")" << endl;
        cout << "  3 - Sorte      (" << jogador->getSorte() << "/" << Jogador::MAX_SORTE << ")" << endl;
        cout << "  0 - Guardar os pontos para depois" << endl;
        cout << "> ";

        int escolha = lerInteiro();
        bool conseguiu = false;

        if (escolha == 0) {
            return;
        }
        else if (escolha == 1) {
            conseguiu = jogador->evoluirAtributo('h');
        }
        else if (escolha == 2) {
            conseguiu = jogador->evoluirAtributo('e');
        }
        else if (escolha == 3) {
            conseguiu = jogador->evoluirAtributo('s');
        }
        else {
            cout << "Opcao invalida." << endl;
            continue;
        }

        if (!conseguiu) {
            cout << "Esse atributo ja esta no limite." << endl;
        }
    }

    // Saiu do laco: acabaram os pontos, ou todos os atributos chegaram ao limite.
    if (jogador->getPontosEvolucao() > 0) {
        cout << "Todos os atributos estao no limite. Os pontos restantes ficam guardados." << endl;
    }
}

// ------------------------------------------------------------------
// Menu principal
// ------------------------------------------------------------------

void Jogo::iniciar() {
    bool sair = false;

    while (!sair) {
        exibirArquivo("telas/abertura.txt");
        cout << "> ";
        int opcao = lerInteiro();
        cout << endl;

        if (opcao == 1) {
            criarPersonagem();
            jogar();
        }
        else if (opcao == 2) {
            cout << "Carregar jogo: ainda nao implementado." << endl << endl;
        }
        else if (opcao == 3) {
            mostrarCreditos();
        }
        else if (opcao == 4) {
            sair = true;
        }
        else {
            cout << "Opcao invalida." << endl << endl;
        }
    }

    cout << "Ate a proxima aventura!" << endl;
}

// ------------------------------------------------------------------
// Controle do que ja aconteceu (percorre o vector com um laco simples)
// ------------------------------------------------------------------

bool Jogo::foiVisitada(int numeroCena) {
    for (size_t i = 0; i < cenasVisitadas.size(); i++) {
        if (cenasVisitadas[i] == numeroCena) {
            return true;
        }
    }
    return false;
}

bool Jogo::jaDerrotou(int numeroCena) {
    for (size_t i = 0; i < monstrosDerrotados.size(); i++) {
        if (monstrosDerrotados[i] == numeroCena) {
            return true;
        }
    }
    return false;
}

// ------------------------------------------------------------------
// Loop da aventura: carrega a cena atual, executa e descobre a proxima.
// ------------------------------------------------------------------

void Jogo::jogar() {
    cenaAtual = 1;
    cenasVisitadas.clear();
    monstrosDerrotados.clear();
    bool fim = false;

    while (!fim) {
        Cena cena;
        string caminho = "cenes/" + to_string(cenaAtual) + ".txt";

        if (!cena.carregar(caminho)) {
            cout << "Nao foi possivel abrir " << caminho << endl;
            return;
        }

        // Itens so sao entregues na primeira visita (evita pegar tudo de novo ao voltar).
        bool primeiraVez = !foiVisitada(cenaAtual);
        if (primeiraVez) {
            cenasVisitadas.push_back(cenaAtual);
        }

        // (Aqui entrara o salvamento automatico, a cada nova cena.)

        cout << endl << "==================================================" << endl;

        if (cena.ehMonstro()) {
            executarCenaMonstro(cena);
        } else {
            executarCenaNormal(cena, fim, primeiraVez);
        }
    }
}

void Jogo::executarCenaNormal(Cena& cena, bool& fim, bool primeiraVez) {
    cout << cena.getTexto() << endl;

    if (primeiraVez) {
        for (int i = 0; i < cena.getQuantidadeItens(); i++) {
            receberItem(cena.getItem(i));
        }
    }

    // Cena sem opcoes = final da historia.
    if (cena.getQuantidadeOpcoes() == 0) {
        cout << endl << "=== FIM DA AVENTURA ===" << endl << endl;
        fim = true;
        return;
    }

    for (int i = 0; i < cena.getQuantidadeOpcoes(); i++) {
        cout << "  " << (i + 1) << " - " << cena.getTextoOpcao(i) << endl;
    }
    cout << "  0 - Ficha do personagem (provisoes e evolucao)" << endl;

    while (true) {
        cout << "> ";
        int escolha = lerInteiro();

        if (escolha == 0) {
            menuFicha();
            // Reexibe as opcoes da cena depois de fechar a ficha.
            for (int i = 0; i < cena.getQuantidadeOpcoes(); i++) {
                cout << "  " << (i + 1) << " - " << cena.getTextoOpcao(i) << endl;
            }
            cout << "  0 - Ficha do personagem (provisoes e evolucao)" << endl;
        }
        else if (escolha >= 1 && escolha <= cena.getQuantidadeOpcoes()) {
            cenaAtual = cena.getDestinoOpcao(escolha - 1);
            return;
        }
        else {
            cout << "Opcao invalida." << endl;
        }
    }
}

void Jogo::executarCenaMonstro(Cena& cena) {
    // Monstro ja vencido antes: nao ressurge (evita "farmar" experiencia).
    if (jaDerrotou(cenaAtual)) {
        cout << "Voce ja derrotou " << cena.getNomeMonstro() << " aqui. O caminho esta livre." << endl;
        cenaAtual = cena.getDestinoSucesso();
        return;
    }

    cout << cena.getTexto() << endl;

    bool venceu = batalha(cena);

    if (venceu) {
        cout << endl << "Voce derrotou " << cena.getNomeMonstro() << "!" << endl;
        monstrosDerrotados.push_back(cenaAtual);

        // Experiencia: quanto mais forte o monstro, mais ele rende.
        int experiencia = cena.getHabilidadeMonstro() + cena.getEnergiaMonstro();
        cout << "Voce ganhou " << experiencia << " pontos de experiencia." << endl;
        int niveisGanhos = jogador->ganharExperiencia(experiencia);

        // Recompensas do monstro (campos T e P do arquivo da cena).
        if (cena.getOuro() > 0) {
            jogador->adicionarOuro(cena.getOuro());
            cout << "Voce encontrou " << cena.getOuro() << " moedas de ouro." << endl;
        }
        if (cena.getProvisoes() > 0) {
            jogador->adicionarProvisoes(cena.getProvisoes());
            cout << "Voce encontrou " << cena.getProvisoes() << " provisao(oes)." << endl;
        }

        for (int i = 0; i < cena.getQuantidadeItens(); i++) {
            receberItem(cena.getItem(i));
        }

        if (niveisGanhos > 0) {
            cout << endl << "*** VOCE SUBIU PARA O NIVEL " << jogador->getNivel()
                 << "! Sua energia foi restaurada. ***" << endl;
            distribuirPontos();
        }

        cenaAtual = cena.getDestinoSucesso();
    }
    else {
        if (!jogador->estaVivo()) {
            cout << endl << "Voce foi derrotado... mas a historia continua." << endl;
            // Decisao provisoria: volta com metade da energia maxima.
            jogador->setEnergia((jogador->getEnergiaMaxima() + 1) / 2);
        }
        cenaAtual = cena.getDestinoDerrota();
    }
}

// Retorna true se o jogador derrotou o monstro; false se perdeu ou fugiu.
bool Jogo::batalha(Cena& cena) {
    Monstro monstro(cena.getNomeMonstro(), cena.getHabilidadeMonstro(),
                    cena.getEnergiaMonstro(), cena.getSorteMonstro());

    cout << "*** BATALHA contra " << monstro.getNome() << " ***" << endl;

    while (jogador->estaVivo() && monstro.estaVivo()) {
        cout << endl;
        cout << jogador->getNome() << " - Energia: " << jogador->getEnergia()
             << "/" << jogador->getEnergiaMaxima() << " | Sorte: " << jogador->getSorte() << endl;
        cout << monstro.getNome() << " - Energia: " << monstro.getEnergia() << endl;
        cout << "  1 - Atacar" << endl;
        cout << "  2 - Fugir" << endl;
        cout << "  3 - Atacar e testar a Sorte" << endl;
        cout << "> ";

        int escolha = lerInteiro();

        if (escolha == 1 || escolha == 3) {
            int energiaJogadorAntes = jogador->getEnergia();
            int energiaMonstroAntes = monstro.getEnergia();

            jogador->atacar(&monstro);

            bool acertouMonstro = monstro.getEnergia() < energiaMonstroAntes;
            bool foiAcertado = jogador->getEnergia() < energiaJogadorAntes;

            if (acertouMonstro) {
                cout << "Voce acertou o golpe!" << endl;
            } else if (foiAcertado) {
                cout << monstro.getNome() << " acertou voce!" << endl;
            } else {
                cout << "Ninguem acertou." << endl;
            }

            // Uso da Sorte em combate: amplia o dano causado ou reduz o dano recebido.
            if (escolha == 3) {
                if (jogador->getSorte() <= 0) {
                    cout << "Voce nao tem mais Sorte para testar." << endl;
                }
                else if (!acertouMonstro && !foiAcertado) {
                    cout << "Ninguem foi ferido, entao a Sorte nao foi usada." << endl;
                }
                else {
                    bool sucesso = jogador->testarSorte(); // gasta 1 ponto de Sorte

                    if (acertouMonstro && sucesso) {
                        monstro.receberDano(2);
                        cout << "Sorte! Seu golpe foi mais forte (+2 de dano)." << endl;
                    } else if (acertouMonstro) {
                        monstro.recuperarEnergia(1);
                        cout << "Azar! Seu golpe perdeu forca (-1 de dano)." << endl;
                    } else if (sucesso) {
                        jogador->recuperarEnergia(1);
                        cout << "Sorte! Voce amorteceu o golpe (-1 de dano)." << endl;
                    } else {
                        jogador->receberDano(1);
                        cout << "Azar! O golpe foi pior do que parecia (+1 de dano)." << endl;
                    }
                }
            }
        }
        else if (escolha == 2) {
            cout << "Voce fugiu da batalha!" << endl;
            return false; // fugir conta como "monstro nao derrotado"
        }
        else {
            cout << "Opcao invalida." << endl;
        }
    }

    return jogador->estaVivo();
}

// Recebe uma linha de item "nome;tipo;combate;FA;dano".
void Jogo::receberItem(string linha) {
    vector<string> campos = dividir(linha, ';');

    if (campos.size() < 5) {
        return; // linha mal formatada: ignora
    }

    string nome = campos[0];
    string tipo = campos[1];

    cout << endl << "Voce obteve: " << nome << endl;

    // Por enquanto so armas ('w') tem efeito: a arma nova eh equipada se o jogador
    // estiver sem arma ou se ela for melhor (FA + dano maiores) que a atual.
    // Armaduras ('r') e itens comuns ('c') entram na etapa do inventario.
    if (tipo == "w") {
        bool combate = (campos[2] == "1");
        int fa = stoi(campos[3]);
        int dano = stoi(campos[4]);

        Arma* atual = jogador->getArmaEquipada();

        if (atual == nullptr || (fa + dano) > (atual->getFA() + atual->getDano())) {
            jogador->equiparArma(new Arma(nome, combate, fa, dano));
            cout << "(" << nome << " foi equipada)" << endl;
        }
    }
}
