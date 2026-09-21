#include "../../include/jogo/Jogo.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cctype>
#include <vector>

// Quebra "Adaga de prata;w;1;1;1" em {"Adaga de prata","w","1","1","1"}.
static vector<string> dividir(string texto, char separador)
{
    vector<string> partes;
    string atual = "";

    for (size_t i = 0; i < texto.size(); i++)
    {
        if (texto[i] == separador)
        {
            partes.push_back(atual);
            atual = "";
        }
        else
        {
            atual += texto[i];
        }
    }
    partes.push_back(atual);
    return partes;
}

Jogo::Jogo()
{
    jogador = nullptr;
    cenaAtual = 1;
}

Jogo::~Jogo()
{
    delete jogador;
}

// ------------------------------------------------------------------
// Entrada e telas
// ------------------------------------------------------------------

// Mostra na tela o conteudo de um arquivo de texto (usa ifstream + getline).
void Jogo::exibirArquivo(string caminho)
{
    ifstream arquivo;
    arquivo.open(caminho);

    if (arquivo.fail())
    {
        cout << "Nao foi possivel abrir " << caminho << endl;
        return;
    }

    string linha;
    while (getline(arquivo, linha))
    {
        cout << linha << endl;
    }

    arquivo.close();
}

// Le uma linha inteira e converte para numero. Retorna -1 se nao for um
// numero valido. Ler a linha toda evita o problema de misturar cin >> com getline.
int Jogo::lerInteiro()
{
    string linha;

    if (!getline(cin, linha))
    {
        // Entrada encerrada (Ctrl+Z / Ctrl+D): sai do programa.
        cout << endl
             << "Entrada encerrada. Ate a proxima!" << endl;
        exit(0);
    }

    if (linha.empty() || linha.size() > 6)
    {
        return -1;
    }

    for (size_t i = 0; i < linha.size(); i++)
    {
        if (!isdigit(static_cast<unsigned char>(linha[i])))
        {
            return -1;
        }
    }

    return stoi(linha);
}

void Jogo::mostrarCreditos()
{
    exibirArquivo("telas/creditos.txt");
    cout << "Pressione ENTER para voltar...";
    string descarte;
    getline(cin, descarte);
}

// Tela de criacao: mostra as regras (arquivo) e le a distribuicao dos 12 pontos.
void Jogo::criarPersonagem()
{
    exibirArquivo("telas/criacao.txt");

    cout << "Nome do personagem: ";
    string nome;
    getline(cin, nome);
    if (nome.empty())
    {
        nome = "Aventureiro";
    }

    int habilidadeExtra, energiaExtra, sorteExtra, pontosExtras, pontosSobrando;

    while (true)
    {
        cout << "Pontos extras em HABILIDADE (0-6): ";
        habilidadeExtra = lerInteiro();
        cout << "Pontos extras em ENERGIA (0-12): ";
        energiaExtra = lerInteiro();
        cout << "Pontos extras em SORTE (0-6): ";
        sorteExtra = lerInteiro();

        pontosExtras = habilidadeExtra + energiaExtra + sorteExtra;
        if (habilidadeExtra < 0 || habilidadeExtra > 6 ||
            energiaExtra < 0 || energiaExtra > 12 ||
            sorteExtra < 0 || sorteExtra > 6){
            cout << endl
                 << "Valor fora dos limites. Tente de novo." << endl
                 << endl;
        } else if (pontosExtras > 12) {
            cout << endl
                 << "A soma dos pontos extras ficou maior que 12: (soma = "
                 << pontosExtras << "). Tente de novo." << endl
                 << endl;
        } else if (pontosExtras != 12)  {
            cout << endl
                 << "A soma dos pontos extras ficou em "
                 << pontosExtras << ". Abra o Menu caso queira ajustar seus pontos." << endl
                 << endl;
                 pontosSobrando = 12 - pontosExtras;
                 break;
        } else {
            break;
        }
    }

    // Se ja havia um personagem (novo jogo depois de outro), libera o antigo.
    delete jogador;
    jogador = new Jogador(nome, 6 + habilidadeExtra, 12 + energiaExtra, 6 + sorteExtra);
    if(pontosSobrando > 0) {
        jogador->setPontosEvolucao(pontosSobrando);
    }

    cout << endl;
    mostrarFicha();
}

// Ficha do personagem (tela de inventario): atributos, arma equipada e
// os demais itens guardados no Inventario.
void Jogo::mostrarFicha()
{
    Inventario &inv = jogador->getInventario();

    cout << "------------ " << jogador->getNome() << " ------------" << endl;
    cout << "Nivel: " << jogador->getNivel()
         << " (experiencia " << jogador->getExperiencia() << "/"
         << jogador->getExperienciaParaProximoNivel() << ")" << endl;
    cout << "Habilidade: " << jogador->getHabilidade() << endl;
    cout << "Energia: " << jogador->getEnergia() << "/" << jogador->getEnergiaMaxima() << endl;
    cout << "Sorte: " << jogador->getSorte() << endl;
    cout << "Provisoes: " << jogador->getProvisoes() << endl;
    cout << "Ouro: " << jogador->getOuro() << endl;

    Arma *arma = inv.getArmaEquipada();
    if (arma != nullptr)
    {
        cout << "Arma: " << arma->imprimeInfo() << endl;
    }
    else
    {
        cout << "Arma: nenhuma" << endl;
    }

    // Outras armas guardadas, mas nao equipadas.
    bool temOutrasArmas = false;
    for (int i = 0; i < inv.getQuantidadeArmas(); i++)
    {
        if (inv.getArma(i) != arma)
        {
            if (!temOutrasArmas)
            {
                cout << "Mochila:" << endl;
                temOutrasArmas = true;
            }
            cout << " - " << inv.getArma(i)->imprimeInfo() << endl;
        }
    }

    // Itens magicos (itens comuns com combate=1: usaveis em "Usar Magia" na batalha).
    if (inv.getQuantidadeItensMagicos() > 0)
    {
        cout << "Magias:" << endl;
        for (int i = 0; i < inv.getQuantidadeItensMagicos(); i++)
        {
            Arma *m = inv.getItemMagico(i);
            cout << " - " << m->getNome() << " (dano magico: " << (2 + m->getDano()) << ")" << endl;
        }
    }

    // Armaduras e itens comuns sem efeito em combate.
    if (inv.getQuantidadeOutrosItens() > 0)
    {
        cout << "Itens:" << endl;
        for (int i = 0; i < inv.getQuantidadeOutrosItens(); i++)
        {
            cout << " - " << inv.getOutroItem(i) << endl;
        }
    }

    if (jogador->getPontosEvolucao() > 0)
    {
        cout << "Pontos de evolucao para distribuir: " << jogador->getPontosEvolucao() << endl;
    }
    cout << "-----------------------------------" << endl;
}

// Ficha + acoes que so podem ser feitas fora de combate.
void Jogo::menuFicha()
{
    bool voltar = false;

    while (!voltar)
    {
        mostrarFicha();
        cout << " 1 - Comer uma provisao (+4 de energia)" << endl;
        cout << " 2 - Distribuir pontos de evolucao" << endl;
        cout << " 0 - Voltar para a historia" << endl;
        cout << "> ";

        int escolha = lerInteiro();

        if (escolha == 1)
        {
            if (jogador->usarProvisao())
            {
                cout << "Voce comeu uma provisao e recuperou energia." << endl;
            }
            else
            {
                cout << "Nao foi possivel: voce nao tem provisoes ou a energia ja esta cheia." << endl;
            }
        }
        else if (escolha == 2)
        {
            if (jogador->podeEvoluir())
            {
                distribuirPontos();
            }
            else
            {
                cout << "Nao ha pontos para distribuir." << endl;
            }
        }
        else if (escolha == 0)
        {
            voltar = true;
        }
        else
        {
            cout << "Opcao invalida." << endl;
        }
    }
}

// Cada ponto de evolucao aumenta em 1 um atributo, respeitando os limites do enunciado.
void Jogo::distribuirPontos()
{
    while (jogador->podeEvoluir())
    {
        cout << endl
             << "Pontos para distribuir: " << jogador->getPontosEvolucao() << endl;
        cout << " 1 - Habilidade (" << jogador->getHabilidade() << "/" << Jogador::MAX_HABILIDADE << ")" << endl;
        cout << " 2 - Energia (" << jogador->getEnergiaMaxima() << "/" << Jogador::MAX_ENERGIA << ")" << endl;
        cout << " 3 - Sorte (" << jogador->getSorte() << "/" << Jogador::MAX_SORTE << ")" << endl;
        cout << " 0 - Guardar os pontos para depois" << endl;
        cout << "> ";

        int escolha = lerInteiro();
        bool conseguiu = false;

        if (escolha == 0)
        {
            return;
        }
        else if (escolha == 1)
        {
            conseguiu = jogador->evoluirAtributo('h');
        }
        else if (escolha == 2)
        {
            conseguiu = jogador->evoluirAtributo('e');
        }
        else if (escolha == 3)
        {
            conseguiu = jogador->evoluirAtributo('s');
        }
        else
        {
            cout << "Opcao invalida." << endl;
            continue;
        }

        if (!conseguiu)
        {
            cout << "Esse atributo ja esta no limite." << endl;
        }
    }

    // Saiu do laco: acabaram os pontos, ou todos os atributos chegaram ao limite.
    if (jogador->getPontosEvolucao() > 0)
    {
        cout << "Todos os atributos estao no limite. Os pontos restantes ficam guardados." << endl;
    }
}

// ------------------------------------------------------------------
// Menu principal
// ------------------------------------------------------------------

void Jogo::iniciar()
{
    bool sair = false;

    while (!sair)
    {
        exibirArquivo("telas/abertura.txt");
        cout << "> ";
        int opcao = lerInteiro();
        cout << endl;

        if (opcao == 1)
        {
            criarPersonagem();
            jogar();
        }
        else if (opcao == 2)
        {
            cout << "Nome do personagem salvo: ";
            string nomeSalvo;
            getline(cin, nomeSalvo);

            if (carregarJogo(nomeSalvo))
            {
                cout << endl
                     << "Jogo carregado com sucesso!" << endl
                     << endl;
                mostrarFicha();
                jogar(false); // continua da cena salva, sem reiniciar o progresso
            }
            else
            {
                cout << endl
                     << "Nao ha nenhum jogo salvo com esse nome." << endl
                     << endl;
            }
        }
        else if (opcao == 3)
        {
            mostrarCreditos();
        }
        else if (opcao == 4)
        {
            sair = true;
        }
        else
        {
            cout << "Opcao invalida." << endl
                 << endl;
        }
    }

    cout << "Ate a proxima aventura!" << endl;
}

// ------------------------------------------------------------------
// Controle do que ja aconteceu (percorre o vector com um laco simples)
// ------------------------------------------------------------------

bool Jogo::foiVisitada(int numeroCena)
{
    for (size_t i = 0; i < cenasVisitadas.size(); i++)
    {
        if (cenasVisitadas[i] == numeroCena)
        {
            return true;
        }
    }
    return false;
}

bool Jogo::jaDerrotou(int numeroCena)
{
    for (size_t i = 0; i < monstrosDerrotados.size(); i++)
    {
        if (monstrosDerrotados[i] == numeroCena)
        {
            return true;
        }
    }
    return false;
}

// ------------------------------------------------------------------
// Salvar/Carregar: um arquivo de texto por personagem, data/<nome>.txt
// (mesma ideia dos arquivos de cena: um dado por linha). Formato:
//
// nome
// habilidade
// energia
// energiaMaxima
// sorte
// nivel
// experiencia
// pontosEvolucao
// cenaAtual
// quantidade de cenas visitadas
// cenas visitadas separadas por ';' (linha vazia se nao houver nenhuma)
// quantidade de monstros derrotados
// monstros derrotados separados por ';' (linha vazia se nao houver nenhum)
// -- a partir daqui, quem escreve/le eh o Inventario (Inventario::salvar/carregar) --
// ouro
// provisoes
// quantidade de armas
// uma linha por arma -> nome;combate;fa;dano
// indice da arma equipada (-1 se nenhuma)
// quantidade de itens magicos
// uma linha por item magico -> nome;combate;fa;dano
// quantidade de outros itens
// uma linha por item
// ------------------------------------------------------------------

// Junta um vector<int> numa linha "1;2;3" (usada so para salvar).
static string juntar(const vector<int> &valores)
{
    string linha = "";
    for (size_t i = 0; i < valores.size(); i++)
    {
        if (i > 0)
        {
            linha += ";";
        }
        linha += to_string(valores[i]);
    }
    return linha;
}

string Jogo::caminhoSave(string nome)
{
    return "data/" + nome + ".txt";
}

void Jogo::salvarJogo()
{
    if (jogador == nullptr)
    {
        return; // nao ha partida em andamento para salvar
    }

    ofstream arquivo(caminhoSave(jogador->getNome()));
    if (arquivo.fail())
    {
        cout << "Aviso: nao foi possivel salvar o jogo." << endl;
        return;
    }

    arquivo << jogador->getNome() << "\n";
    arquivo << jogador->getHabilidade() << "\n";
    arquivo << jogador->getEnergia() << "\n";
    arquivo << jogador->getEnergiaMaxima() << "\n";
    arquivo << jogador->getSorte() << "\n";
    arquivo << jogador->getNivel() << "\n";
    arquivo << jogador->getExperiencia() << "\n";
    arquivo << jogador->getPontosEvolucao() << "\n";

    arquivo << cenaAtual << "\n";

    arquivo << cenasVisitadas.size() << "\n";
    arquivo << juntar(cenasVisitadas) << "\n";

    arquivo << monstrosDerrotados.size() << "\n";
    arquivo << juntar(monstrosDerrotados) << "\n";

    // Ouro, provisoes, armas e itens: quem sabe salvar isso eh o Inventario.
    jogador->getInventario().salvar(arquivo);

    arquivo.close();
}

// Le uma linha do arquivo de save e converte para inteiro.
// "ok" vira false se a linha nao existir ou nao for um numero valido.
static int lerLinhaInteira(ifstream &arquivo, bool &ok)
{
    string linha;
    if (!ok || !getline(arquivo, linha))
    {
        ok = false;
        return 0;
    }
    try
    {
        return stoi(linha);
    }
    catch (...)
    {
        ok = false; // linha nao era um numero (arquivo corrompido)
        return 0;
    }
}

// Restaura a partida salva em data/<nome>.txt. Retorna false se nao existe
// save com esse nome ou se o arquivo estiver corrompido/incompleto.
bool Jogo::carregarJogo(string nome)
{
    ifstream arquivo(caminhoSave(nome));
    if (arquivo.fail())
    {
        return false; // nenhum jogo salvo com esse nome
    }

    string nomeSalvo;
    if (!getline(arquivo, nomeSalvo))
    {
        return false;
    }

    bool ok = true;
    int habilidade = lerLinhaInteira(arquivo, ok);
    int energia = lerLinhaInteira(arquivo, ok);
    int energiaMaxima = lerLinhaInteira(arquivo, ok);
    int sorte = lerLinhaInteira(arquivo, ok);
    int nivel = lerLinhaInteira(arquivo, ok);
    int experiencia = lerLinhaInteira(arquivo, ok);
    int pontosEvolucao = lerLinhaInteira(arquivo, ok);

    int cena = lerLinhaInteira(arquivo, ok);
    int quantidadeVisitadas = lerLinhaInteira(arquivo, ok);

    string linhaVisitadas;
    if (!getline(arquivo, linhaVisitadas))
    {
        ok = false;
    }

    int quantidadeDerrotados = lerLinhaInteira(arquivo, ok);

    string linhaDerrotados;
    if (!getline(arquivo, linhaDerrotados))
    {
        ok = false;
    }

    if (!ok)
    {
        return false; // arquivo de save incompleto ou corrompido
    }

    // So depois de ler os dados do personagem eh que descartamos o jogador atual.
    delete jogador;
    jogador = new Jogador(nomeSalvo, habilidade, energiaMaxima, sorte);
    jogador->setEnergia(energia);
    jogador->setNivel(nivel);
    jogador->setExperiencia(experiencia);
    jogador->setPontosEvolucao(pontosEvolucao);

    // Ouro, provisoes, armas e itens: quem sabe se carregar isso eh o Inventario.
    if (!jogador->getInventario().carregar(arquivo))
    {
        delete jogador;
        jogador = nullptr;
        return false; // dados do inventario corrompidos: descarta tudo
    }

    arquivo.close();

    cenaAtual = cena;

    cenasVisitadas.clear();
    if (quantidadeVisitadas > 0 && !linhaVisitadas.empty())
    {
        vector<string> partes = dividir(linhaVisitadas, ';');
        for (size_t i = 0; i < partes.size(); i++)
        {
            cenasVisitadas.push_back(stoi(partes[i]));
        }
    }

    monstrosDerrotados.clear();
    if (quantidadeDerrotados > 0 && !linhaDerrotados.empty())
    {
        vector<string> partes = dividir(linhaDerrotados, ';');
        for (size_t i = 0; i < partes.size(); i++)
        {
            monstrosDerrotados.push_back(stoi(partes[i]));
        }
    }

    return true;
}

// ------------------------------------------------------------------
// Loop da aventura: carrega a cena atual, executa e descobre a proxima.
// ------------------------------------------------------------------

void Jogo::jogar(bool novaPartida)
{
    if (novaPartida)
    {
        cenaAtual = 1;
        cenasVisitadas.clear();
        monstrosDerrotados.clear();
    }
    bool fim = false;

    while (!fim)
    {
        Cena cena;
        string caminho = "cenes/" + to_string(cenaAtual) + ".txt";

        if (!cena.carregar(caminho))
        {
            cout << "Nao foi possivel abrir " << caminho << endl;
            return;
        }

        // Itens so sao entregues na primeira visita (evita pegar tudo de novo ao voltar).
        bool primeiraVez = !foiVisitada(cenaAtual);
        if (primeiraVez)
        {
            cenasVisitadas.push_back(cenaAtual);
        }

        // Salvamento automatico: toda vez que uma cena nova eh carregada.
        salvarJogo();

        cout << endl
             << "==================================================" << endl;

        if (cena.ehMonstro())
        {
            executarCenaMonstro(cena);
        }
        else if (cena.ehTesteDeSorte())
        {
            executarCenaTesteSorte(cena);
        }
        else
        {
            executarCenaNormal(cena, fim, primeiraVez);
        }
    }
}

void Jogo::executarCenaTesteSorte(Cena &cena)
{
    cout << cena.getTexto() << endl;

    int dado = (rand() % 6) + 1;
    int total = dado + jogador->getSorte();

    cout << "Testando a Sorte..." << endl;
    cout << "Dado (1-6): " << dado << " + Sorte (" << jogador->getSorte() << ") = " << total
         << " (precisa de mais que " << cena.getDificuldadeSorte() << ")" << endl;

    jogador->decrementarSorte(); // usar a Sorte gasta 1 ponto, com sucesso ou nao

    if (total > cena.getDificuldadeSorte())
    {
        cout << "Sucesso! Voce consegue passar em seguranca." << endl;
        cenaAtual = cena.getDestinoSucesso();
    }
    else
    {
        cout << "Falha! Voce se machuca na tentativa." << endl;
        jogador->receberDano(cena.getDanoFalhaSorte());
        cout << "Voce perdeu " << cena.getDanoFalhaSorte() << " pontos de energia." << endl;

        if (!jogador->estaVivo())
        {
            // Decisao igual a derrota em combate: nao morre so pela queda.
            jogador->setEnergia((jogador->getEnergiaMaxima() + 1) / 2);
        }

        cenaAtual = cena.getDestinoDerrota();
    }
}

void Jogo::executarCenaNormal(Cena &cena, bool &fim, bool primeiraVez)
{
    cout << cena.getTexto() << endl;

    if (primeiraVez)
    {
        for (int i = 0; i < cena.getQuantidadeItens(); i++)
        {
            receberItem(cena.getItem(i));
        }
    }

    // Cena sem opcoes = final da historia.
    if (cena.getQuantidadeOpcoes() == 0)
    {
        cout << endl
             << "=== FIM DA AVENTURA ===" << endl
             << endl;
        fim = true;
        return;
    }

    for (int i = 0; i < cena.getQuantidadeOpcoes(); i++)
    {
        cout << " " << (i + 1) << " - " << cena.getTextoOpcao(i) << endl;
    }
    cout << " 0 - Ficha do personagem (provisoes e evolucao)" << endl;

    while (true)
    {
        cout << "> ";
        int escolha = lerInteiro();

        if (escolha == 0)
        {
            menuFicha();
            // Reexibe as opcoes da cena depois de fechar a ficha.
            for (int i = 0; i < cena.getQuantidadeOpcoes(); i++)
            {
                cout << " " << (i + 1) << " - " << cena.getTextoOpcao(i) << endl;
            }
            cout << " 0 - Ficha do personagem (provisoes e evolucao)" << endl;
        }
        else if (escolha >= 1 && escolha <= cena.getQuantidadeOpcoes())
        {
            cenaAtual = cena.getDestinoOpcao(escolha - 1);
            return;
        }
        else
        {
            cout << "Opcao invalida." << endl;
        }
    }
}

void Jogo::executarCenaMonstro(Cena &cena)
{
    // Monstro ja vencido antes: nao ressurge (evita "farmar" experiencia).
    if (jaDerrotou(cenaAtual))
    {
        cout << "Voce ja derrotou " << cena.getNomeMonstro() << " aqui. O caminho esta livre." << endl;
        cenaAtual = cena.getDestinoSucesso();
        return;
    }

    cout << cena.getTexto() << endl;

    bool venceu = batalha(cena);

    if (venceu)
    {
        cout << endl
             << "Voce derrotou " << cena.getNomeMonstro() << "!" << endl;
        monstrosDerrotados.push_back(cenaAtual);

        // Experiencia: quanto mais forte o monstro, mais ele rende.
        int experiencia = cena.getHabilidadeMonstro() + cena.getEnergiaMonstro();
        cout << "Voce ganhou " << experiencia << " pontos de experiencia." << endl;
        int niveisGanhos = jogador->ganharExperiencia(experiencia);

        // Recompensas do monstro (campos T e P do arquivo da cena).
        if (cena.getOuro() > 0)
        {
            jogador->adicionarOuro(cena.getOuro());
            cout << "Voce encontrou " << cena.getOuro() << " moedas de ouro." << endl;
        }
        if (cena.getProvisoes() > 0)
        {
            jogador->adicionarProvisoes(cena.getProvisoes());
            cout << "Voce encontrou " << cena.getProvisoes() << " provisao(oes)." << endl;
        }

        for (int i = 0; i < cena.getQuantidadeItens(); i++)
        {
            receberItem(cena.getItem(i));
        }

        if (niveisGanhos > 0)
        {
            cout << endl
                 << "*** VOCE SUBIU PARA O NIVEL " << jogador->getNivel()
                 << "! Sua energia foi restaurada. ***" << endl;
            distribuirPontos();
        }

        cenaAtual = cena.getDestinoSucesso();
    }
    else
    {
        if (!jogador->estaVivo())
        {
            cout << endl
                 << "Voce foi derrotado... mas a historia continua." << endl;
            // Decisao provisoria: volta com metade da energia maxima.
            jogador->setEnergia((jogador->getEnergiaMaxima() + 1) / 2);
        }
        cenaAtual = cena.getDestinoDerrota();
    }
}

// Retorna true se o jogador derrotou o monstro; false se perdeu ou fugiu.
bool Jogo::batalha(Cena &cena)
{
    Monstro monstro(cena.getNomeMonstro(), cena.getHabilidadeMonstro(),
                    cena.getEnergiaMonstro(), cena.getSorteMonstro());

    cout << "*** BATALHA contra " << monstro.getNome() << " ***" << endl;

    while (jogador->estaVivo() && monstro.estaVivo())
    {
        cout << endl;
        cout << jogador->getNome() << " - Energia: " << jogador->getEnergia()
             << "/" << jogador->getEnergiaMaxima() << " | Sorte: " << jogador->getSorte() << endl;
        cout << monstro.getNome() << " - Energia: " << monstro.getEnergia() << endl;
        cout << " 1 - Atacar" << endl;
        cout << " 2 - Fugir" << endl;
        cout << " 3 - Atacar e testar a Sorte" << endl;
        if (jogador->getInventario().getQuantidadeItensMagicos() > 0)
        {
            cout << " 4 - Usar Magia" << endl;
        }
        cout << "> ";

        int escolha = lerInteiro();

        if (escolha == 1 || escolha == 3)
        {
            int energiaJogadorAntes = jogador->getEnergia();
            int energiaMonstroAntes = monstro.getEnergia();

            jogador->atacar(&monstro);

            bool acertouMonstro = monstro.getEnergia() < energiaMonstroAntes;
            bool foiAcertado = jogador->getEnergia() < energiaJogadorAntes;

            if (acertouMonstro)
            {
                cout << "Voce acertou o golpe!" << endl;
            }
            else if (foiAcertado)
            {
                cout << monstro.getNome() << " acertou voce!" << endl;
            }
            else
            {
                cout << "Ninguem acertou." << endl;
            }

            // Uso da Sorte em combate: amplia o dano causado ou reduz o dano recebido.
            if (escolha == 3)
            {
                if (jogador->getSorte() <= 0)
                {
                    cout << "Voce nao tem mais Sorte para testar." << endl;
                }
                else if (!acertouMonstro && !foiAcertado)
                {
                    cout << "Ninguem foi ferido, entao a Sorte nao foi usada." << endl;
                }
                else
                {
                    bool sucesso = jogador->testarSorte(); // gasta 1 ponto de Sorte

                    if (acertouMonstro && sucesso)
                    {
                        monstro.receberDano(2);
                        cout << "Sorte! Seu golpe foi mais forte (+2 de dano)." << endl;
                    }
                    else if (acertouMonstro)
                    {
                        monstro.recuperarEnergia(1);
                        cout << "Azar! Seu golpe perdeu forca (-1 de dano)." << endl;
                    }
                    else if (sucesso)
                    {
                        jogador->recuperarEnergia(1);
                        cout << "Sorte! Voce amorteceu o golpe (-1 de dano)." << endl;
                    }
                    else
                    {
                        jogador->receberDano(1);
                        cout << "Azar! O golpe foi pior do que parecia (+1 de dano)." << endl;
                    }
                }
            }
        }
        else if (escolha == 2)
        {
            cout << "Voce fugiu da batalha!" << endl;
            return false; // fugir conta como "monstro nao derrotado"
        }
        else if (escolha == 4 && jogador->getInventario().getQuantidadeItensMagicos() > 0)
        {
            usarMagia(monstro);
        }
        else
        {
            cout << "Opcao invalida." << endl;
        }
    }

    return jogador->estaVivo();
}

void Jogo::usarMagia(Monstro &monstro)
{
    Inventario &inv = jogador->getInventario();
    int quantidade = inv.getQuantidadeItensMagicos();

    cout << "Qual item magico usar?" << endl;
    for (int i = 0; i < quantidade; i++)
    {
        Arma *item = inv.getItemMagico(i);
        cout << " " << (i + 1) << " - " << item->getNome()
             << " (dano magico: " << (2 + item->getDano()) << ")" << endl;
    }
    cout << " 0 - Cancelar" << endl;
    cout << "> ";

    int escolha = lerInteiro();
    if (escolha < 1 || escolha > quantidade)
    {
        cout << "Voce guarda o item de volta." << endl;
        return;
    }

    Arma *item = inv.consumirItemMagico(escolha - 1);
    int dano = 2 + item->getDano(); // 2 = dano base de um acerto (mesma base do combate fisico)

    cout << "Voce usa " << item->getNome() << "! Um efeito magico acerta "
         << monstro.getNome() << " em cheio (a magia nunca erra)." << endl;
    monstro.receberDano(dano);

    delete item; // uso unico: o item foi consumido
}

// Recebe uma linha de item "nome;tipo;combate;FA;dano" e repassa para o
// Inventario do jogador (quem decide o que fazer com cada tipo de item).
void Jogo::receberItem(string linha)
{
    vector<string> campos = dividir(linha, ';');
    if (campos.size() < 5)
    {
        return; // linha mal formatada: ignora
    }
    string nome = campos[0];

    Arma *armaAntes = jogador->getArmaEquipada();
    jogador->getInventario().adicionarItem(linha);
    Arma *armaDepois = jogador->getArmaEquipada();

    cout << endl
         << "Voce obteve: " << nome << endl;
    if (armaDepois != armaAntes && armaDepois != nullptr)
    {
        cout << "(" << armaDepois->getNome() << " foi equipada)" << endl;
    }
}
