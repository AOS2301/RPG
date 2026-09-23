#include "../../include/jogo/Jogo.h"
#include "../../include/util/Texto.h"
#include <iostream>
#include <fstream>
#include <cstdlib> // exit()
#include <cctype>
#include <vector>

Jogo::Jogo()
{
    jogador = nullptr;
    cenaAtual = 1;
}

Jogo::~Jogo()
{
    delete jogador;
}

// Começo do jogo: tela de abertura + menu principal (novo jogo, carregar, creditos, sair).
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

            if (carregarJogo(aparar(nomeSalvo)))
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
                     << "Nao foi possivel carregar: nao ha jogo salvo com esse nome," << endl
                     << "ou o save eh de uma versao antiga do jogo." << endl
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

    linha = aparar(linha);
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
    nome = aparar(nome);
    if (nome.empty())
    {
        nome = "Aventureiro";
    }

    int habilidadeExtra, energiaExtra, sorteExtra;

    while (true)
    {
        cout << "Pontos extras em HABILIDADE (0-6): ";
        habilidadeExtra = lerInteiro();
        cout << "Pontos extras em ENERGIA (0-12): ";
        energiaExtra = lerInteiro();
        cout << "Pontos extras em SORTE (0-6): ";
        sorteExtra = lerInteiro();

        int soma = habilidadeExtra + energiaExtra + sorteExtra;

        if (habilidadeExtra < 0 || habilidadeExtra > 6 ||
            energiaExtra < 0 || energiaExtra > 12 ||
            sorteExtra < 0 || sorteExtra > 6)
        {
            cout << endl
                 << "Valor fora dos limites. Tente de novo." << endl
                 << endl;
        }
        else if (soma > 12)
        {
            cout << endl
                 << "A soma dos pontos extras ficou maior que 12 (soma = "
                 << soma << "). Tente de novo." << endl
                 << endl;
        }
        else
        {
            if (soma < 12)
            {
                cout << endl
                     << "A soma dos pontos extras ficou em " << soma
                     << ". Os pontos restantes ficam guardados para distribuir depois, pela ficha do personagem." << endl
                     << endl;
            }
            break;
        }
    }

    // Pontos extras que nao foram usados na criacao ficam guardados como
    // pontos de evolucao, para serem distribuidos depois pela ficha.
    int pontosRestantes = 12 - (habilidadeExtra + energiaExtra + sorteExtra);

    // Se ja havia um personagem (novo jogo depois de outro), libera o antigo.
    delete jogador;
    jogador = new Jogador(nome, 6 + habilidadeExtra, 12 + energiaExtra, 6 + sorteExtra);
    jogador->setPontosEvolucao(pontosRestantes);

    cout << endl;
    mostrarFicha();
}

// Ficha do personagem (tela de inventario): atributos, equipamentos e
// todos os itens guardados no Inventario.
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
    cout << "Provisoes: " << inv.getProvisoes() << endl;
    cout << "Ouro: " << inv.getOuro() << endl;

    Arma *arma = inv.getArmaEquipada();
    if (arma != nullptr)
    {
        cout << "Arma equipada: " << arma->imprimeInfo() << endl;
    }
    else
    {
        cout << "Arma equipada: nenhuma" << endl;
    }

    Armadura *armadura = inv.getArmaduraEquipada();
    if (armadura != nullptr)
    {
        cout << "Armadura equipada: " << armadura->imprimeInfo() << endl;
    }
    else
    {
        cout << "Armadura equipada: nenhuma" << endl;
    }

    // Armas e armaduras guardadas, mas nao equipadas.
    bool temMochila = false;
    for (int i = 0; i < inv.getQuantidadeArmas(); i++)
    {
        if (inv.getArma(i) != arma)
        {
            if (!temMochila)
            {
                cout << "Mochila:" << endl;
                temMochila = true;
            }
            cout << " - " << inv.getArma(i)->imprimeInfo() << endl;
        }
    }
    for (int i = 0; i < inv.getQuantidadeArmaduras(); i++)
    {
        if (inv.getArmadura(i) != armadura)
        {
            if (!temMochila)
            {
                cout << "Mochila:" << endl;
                temMochila = true;
            }
            cout << " - " << inv.getArmadura(i)->imprimeInfo() << endl;
        }
    }

    // Itens magicos: usados na opcao "Usar Magia" da batalha.
    if (inv.getQuantidadeItensMagicos() > 0)
    {
        cout << "Magias:" << endl;
        for (int i = 0; i < inv.getQuantidadeItensMagicos(); i++)
        {
            cout << " - " << inv.getItemMagico(i)->imprimeInfo() << endl;
        }
    }

    // Itens comuns, sem efeito em combate.
    if (inv.getQuantidadeItensComuns() > 0)
    {
        cout << "Itens:" << endl;
        for (int i = 0; i < inv.getQuantidadeItensComuns(); i++)
        {
            cout << " - " << inv.getItemComum(i)->imprimeInfo() << endl;
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
        cout << " 3 - Trocar arma ou armadura equipada" << endl;
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
        else if (escolha == 3)
        {
            trocarEquipamento();
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

// Enunciado: "Quando quiser o jogador pode acessar a tela de inventario e
// modificar o que o personagem esta equipado."
void Jogo::trocarEquipamento()
{
    Inventario &inv = jogador->getInventario();

    cout << " 1 - Trocar arma" << endl;
    cout << " 2 - Trocar armadura" << endl;
    cout << " 0 - Voltar" << endl;
    cout << "> ";
    int tipo = lerInteiro();

    if (tipo == 1)
    {
        if (inv.getQuantidadeArmas() == 0)
        {
            cout << "Voce nao tem nenhuma arma." << endl;
            return;
        }
        for (int i = 0; i < inv.getQuantidadeArmas(); i++)
        {
            cout << " " << (i + 1) << " - " << inv.getArma(i)->imprimeInfo();
            if (inv.getArma(i) == inv.getArmaEquipada())
            {
                cout << " [equipada]";
            }
            cout << endl;
        }
        cout << " 0 - Cancelar" << endl;
        cout << "> ";
        int escolha = lerInteiro();

        if (escolha >= 1 && inv.equiparArma(escolha - 1))
        {
            cout << inv.getArmaEquipada()->getNome() << " equipada." << endl;
        }
        else
        {
            cout << "Nada foi alterado." << endl;
        }
    }
    else if (tipo == 2)
    {
        if (inv.getQuantidadeArmaduras() == 0)
        {
            cout << "Voce nao tem nenhuma armadura." << endl;
            return;
        }
        for (int i = 0; i < inv.getQuantidadeArmaduras(); i++)
        {
            cout << " " << (i + 1) << " - " << inv.getArmadura(i)->imprimeInfo();
            if (inv.getArmadura(i) == inv.getArmaduraEquipada())
            {
                cout << " [equipada]";
            }
            cout << endl;
        }
        cout << " 0 - Cancelar" << endl;
        cout << "> ";
        int escolha = lerInteiro();

        if (escolha >= 1 && inv.equiparArmadura(escolha - 1))
        {
            cout << inv.getArmaduraEquipada()->getNome() << " equipada." << endl;
        }
        else
        {
            cout << "Nada foi alterado." << endl;
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
// versao do save (VERSAO_SAVE)
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

// Faz o caminho contrario do juntar: "1;2;3" -> {1,2,3}.
// Retorna false se algum numero for invalido ou a quantidade nao bater.
static bool separarNumeros(string linha, int quantidadeEsperada, vector<int> &destino)
{
    destino.clear();
    linha = aparar(linha);
    if (quantidadeEsperada == 0)
    {
        return linha.empty();
    }

    vector<string> partes = dividir(linha, ';');
    try
    {
        for (size_t i = 0; i < partes.size(); i++)
        {
            destino.push_back(stoi(partes[i]));
        }
    }
    catch (...)
    {
        return false;
    }
    return static_cast<int>(destino.size()) == quantidadeEsperada;
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

    // ofstream apaga o conteudo antigo ao abrir: um novo personagem com o
    // mesmo nome sobrescreve o save anterior.
    ofstream arquivo(caminhoSave(jogador->getNome()));
    if (arquivo.fail())
    {
        cout << "Aviso: nao foi possivel salvar o jogo (a pasta data/ existe?)." << endl;
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

    // Ouro, provisoes e itens: quem sabe salvar isso eh o Inventario.
    jogador->getInventario().salvar(arquivo);

    arquivo.close();
}

// Restaura a partida salva em data/<nome>.txt. Retorna false se nao existe
// save com esse nome, se for de outra versao ou se estiver corrompido.
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
    nomeSalvo = aparar(nomeSalvo);

    int habilidade, energia, energiaMaxima, sorte, nivel, experiencia, pontosEvolucao;
    int cena, quantidadeVisitadas, quantidadeDerrotados;
    string linhaVisitadas, linhaDerrotados;

    bool ok = lerLinhaInteira(arquivo, habilidade) &&
              lerLinhaInteira(arquivo, energia) &&
              lerLinhaInteira(arquivo, energiaMaxima) &&
              lerLinhaInteira(arquivo, sorte) &&
              lerLinhaInteira(arquivo, nivel) &&
              lerLinhaInteira(arquivo, experiencia) &&
              lerLinhaInteira(arquivo, pontosEvolucao) &&
              lerLinhaInteira(arquivo, cena) &&
              lerLinhaInteira(arquivo, quantidadeVisitadas) &&
              getline(arquivo, linhaVisitadas) &&
              lerLinhaInteira(arquivo, quantidadeDerrotados) &&
              getline(arquivo, linhaDerrotados);
    if (!ok)
    {
        return false; // arquivo de save incompleto ou corrompido
    }

    vector<int> visitadas;
    vector<int> derrotados;
    if (!separarNumeros(linhaVisitadas, quantidadeVisitadas, visitadas) ||
        !separarNumeros(linhaDerrotados, quantidadeDerrotados, derrotados))
    {
        return false;
    }

    // Trava de seguranca: pontosEvolucao nunca deveria ser negativo nem um
    // valor absurdamente alto (o maximo teorico e 12 da criacao + 2 por
    // nivel ja alcancado). Um valor fora disso indica um save corrompido,
    // entao zeramos em vez de aceitar.
    int limitePontosEvolucao = 12 + (nivel * 2);
    if (pontosEvolucao < 0 || pontosEvolucao > limitePontosEvolucao)
    {
        pontosEvolucao = 0;
    }

    // So depois de ler os dados do personagem eh que descartamos o jogador atual.
    delete jogador;
    jogador = new Jogador(nomeSalvo, habilidade, energiaMaxima, sorte);
    jogador->setEnergia(energia);
    jogador->setNivel(nivel);
    jogador->setExperiencia(experiencia);
    jogador->setPontosEvolucao(pontosEvolucao);

    // Ouro, provisoes e itens: quem sabe carregar isso eh o Inventario.
    if (!jogador->getInventario().carregar(arquivo))
    {
        delete jogador;
        jogador = nullptr;
        return false; // dados do inventario corrompidos: descarta tudo
    }

    arquivo.close();

    cenaAtual = cena;
    cenasVisitadas = visitadas;
    monstrosDerrotados = derrotados;
    return true;
}

// ------------------------------------------------------------------
// Loop da aventura: carrega a cena atual, executa e descobre a proxima.
// ------------------------------------------------------------------

void Jogo::jogar(bool novaPartida)
{
    if (novaPartida){
        cenaAtual = 1;
        cenasVisitadas.clear();
        monstrosDerrotados.clear();
    }
    bool fim = false;

    while (!fim)
    {
        Cena cena;
        string caminho = "cenes/" + to_string(cenaAtual) + ".txt";

        if (!cena.carregar(caminho)){
            cout << "Nao foi possivel abrir " << caminho << endl;
            return;
        }

        // Itens so sao entregues na primeira visita (evita pegar tudo de novo ao voltar).
        bool primeiraVez = !foiVisitada(cenaAtual);
        if (primeiraVez){
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

        limparTerminal();
    }
}

// Teste de Sorte fora de combate (ex: atravessar uma ponte velha ou passar
// por um espaco apertado). A regra fica em Personagem::testarSorteContra;
// aqui so mostramos o resultado e decidimos a proxima cena.
void Jogo::executarCenaTesteSorte(Cena &cena)
{
    cout << cena.getTexto() << endl;

    int sorteAntes = jogador->getSorte();
    int dado;
    bool passou = jogador->testarSorteContra(cena.getDificuldadeSorte(), dado);

    cout << "Testando a Sorte..." << endl;
    cout << "Dado (1-6): " << dado << " + Sorte (" << sorteAntes << ") = " << (dado + sorteAntes)
         << " (precisa de mais que " << cena.getDificuldadeSorte() << ")" << endl;
    cout << "Sua Sorte agora eh " << jogador->getSorte() << "." << endl;

    if (passou)
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
            cout << "Voce quase nao sobrevive... mas a historia continua." << endl;
            jogador->recuperarAposDerrota();
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
    cout << " 0 - Ficha do personagem (inventario, provisoes e evolucao)" << endl;

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
            cout << " 0 - Ficha do personagem (inventario, provisoes e evolucao)" << endl;
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

        // Recompensas do monstro (campos T, P e I do arquivo da cena).
        Inventario &inv = jogador->getInventario();
        if (cena.getOuro() > 0)
        {
            inv.adicionarOuro(cena.getOuro());
            cout << "Voce encontrou " << cena.getOuro() << " moedas de ouro." << endl;
        }
        if (cena.getProvisoes() > 0)
        {
            inv.adicionarProvisoes(cena.getProvisoes());
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
            jogador->recuperarAposDerrota();
        }
        cenaAtual = cena.getDestinoDerrota();
    }
}

// Tela de batalha. Retorna true se o jogador derrotou o monstro; false se
// perdeu ou fugiu. As regras ficam no Personagem/Jogador; aqui so o menu
// e as mensagens.
bool Jogo::batalha(Cena &cena)
{
    Monstro monstro(cena.getNomeMonstro(), cena.getHabilidadeMonstro(),
                    cena.getEnergiaMonstro(), cena.getSorteMonstro());

    cout << "*** BATALHA contra " << monstro.getNome() << " ***" << endl;

    while (jogador->estaVivo() && monstro.estaVivo())
    {
        bool temMagia = jogador->getInventario().getQuantidadeItensMagicos() > 0;

        cout << endl;
        cout << jogador->getNome() << " - Energia: " << jogador->getEnergia()
             << "/" << jogador->getEnergiaMaxima() << " | Sorte: " << jogador->getSorte() << endl;
        cout << monstro.getNome() << " - Energia: " << monstro.getEnergia() << endl;
        cout << " 1 - Atacar" << endl;
        cout << " 2 - Fugir" << endl;
        cout << " 3 - Atacar e testar a Sorte" << endl;
        if (temMagia)
        {
            cout << " 4 - Usar Magia" << endl;
        }
        cout << "> ";

        int escolha = lerInteiro();

        if (escolha == 1 || escolha == 3)
        {
            // Uma rodada: os dois calculam a FA e quem vencer fere o outro.
            int resultado = jogador->disputarRodada(&monstro);

            if (resultado == 1)
            {
                cout << "Voce acertou o golpe!" << endl;
            }
            else if (resultado == -1)
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
                else if (resultado == 0)
                {
                    cout << "Ninguem foi ferido, entao a Sorte nao foi usada." << endl;
                }
                else
                {
                    bool venceuRodada = (resultado == 1);
                    bool sucesso = jogador->usarSorteEmCombate(&monstro, venceuRodada);

                    if (venceuRodada && sucesso)
                    {
                        cout << "Sorte! Seu golpe foi mais forte (+2 de dano)." << endl;
                    }
                    else if (venceuRodada)
                    {
                        cout << "Azar! Seu golpe perdeu forca (-1 de dano)." << endl;
                    }
                    else if (sucesso)
                    {
                        cout << "Sorte! Voce amorteceu o golpe (-1 de dano)." << endl;
                    }
                    else
                    {
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
        else if (escolha == 4 && temMagia)
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

// Submenu de "Usar Magia": o jogador escolhe um item magico. A regra
// (consumir o item e causar o dano) fica em Jogador::usarItemMagico.
void Jogo::usarMagia(Monstro &monstro)
{
    Inventario &inv = jogador->getInventario();
    int quantidade = inv.getQuantidadeItensMagicos();

    cout << "Qual item magico usar?" << endl;
    for (int i = 0; i < quantidade; i++)
    {
        cout << " " << (i + 1) << " - " << inv.getItemMagico(i)->imprimeInfo() << endl;
    }
    cout << " 0 - Cancelar" << endl;
    cout << "> ";

    int escolha = lerInteiro();
    if (escolha < 1 || escolha > quantidade)
    {
        cout << "Voce guarda o item de volta." << endl;
        return;
    }

    string nomeItem = inv.getItemMagico(escolha - 1)->getNome(); // antes de ser consumido
    int dano = jogador->usarItemMagico(escolha - 1, &monstro);

    cout << "Voce usa " << nomeItem << "! Um efeito magico acerta "
         << monstro.getNome() << " em cheio e causa " << dano
         << " de dano (a magia nunca erra)." << endl;
}

// Entrega ao jogador um item vindo de uma cena ("nome;tipo;combate;FA;dano").
// Quem entende a linha e decide onde guardar eh o Inventario; aqui so avisamos.
void Jogo::receberItem(string linha)
{
    Inventario &inv = jogador->getInventario();
    Arma *armaAntes = inv.getArmaEquipada();
    Armadura *armaduraAntes = inv.getArmaduraEquipada();

    Item *item = inv.adicionarItem(linha);
    if (item == nullptr)
    {
        return; // linha de item mal formatada no arquivo da cena: ignora
    }

    cout << endl
         << "Voce obteve: " << item->getNome() << endl;

    if (inv.getArmaEquipada() != armaAntes)
    {
        cout << "(" << inv.getArmaEquipada()->getNome() << " foi equipada)" << endl;
    }
    if (inv.getArmaduraEquipada() != armaduraAntes)
    {
        cout << "(" << inv.getArmaduraEquipada()->getNome() << " foi equipada)" << endl;
    }
}

void Jogo::limparTerminal()
{
#ifdef _WIN32
    system("cls");
#else 
    system("clear");
#endif 
}