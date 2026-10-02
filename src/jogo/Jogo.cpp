#include <iostream>
#include <fstream>
#include <cstdlib>
#include "../../include/jogo/Jogo.h"
#include "../../include/itens/CriarItem.h"

using namespace std;

Jogo::Jogo()
{
    jogador = nullptr;
    cenaAtual = 1;
}

Jogo::~Jogo()
{
    if (jogador != nullptr)
    {
        delete jogador;
    }
}

// =====================================================================
// Auxiliares de tela e teclado
// =====================================================================

void Jogo::limparTela(){
    system("clear||cls");
}

void Jogo::separador()
{
    cout << endl << "==================================================" << endl;
}

void Jogo::pausar()
{
    cout << "(Pressione Enter para continuar)";
    string linha;
    getline(cin, linha);
}

// Le uma linha do teclado ate o usuario digitar um numero entre minimo e maximo.
int Jogo::lerOpcao(int minimo, int maximo)
{
    string linha;
    while (true)
    {
        cout << "> ";
        if (!getline(cin, linha))
        {
            // Entrada acabou (ex: Ctrl+D): encerra o programa
            cout << endl << "Ate a proxima!" << endl;
            exit(0);
        }

        // Confere se a linha tem so digitos (e nao e grande demais para um int)
        bool ehNumero = (linha.size() > 0 && linha.size() <= 4);
        for (int i = 0; i < (int)linha.size(); i++)
        {
            if (linha[i] < '0' || linha[i] > '9')
            {
                ehNumero = false;
            }
        }

        if (ehNumero)
        {
            int valor = stoi(linha);
            if (valor >= minimo && valor <= maximo)
            {
                return valor;
            }
        }
        cout << "Opcao invalida. Digite um numero de " << minimo << " a " << maximo << "." << endl;
    }
}

void Jogo::mostrarArquivo(string caminho)
{
    ifstream arquivo;
    arquivo.open(caminho);
    if (!arquivo.is_open())
    {
        cout << "(arquivo " << caminho << " nao encontrado)" << endl;
        return;
    }

    string linha;
    while (getline(arquivo, linha))
    {
        cout << linha << endl;
    }
    arquivo.close();
}

bool Jogo::estaNaLista(vector<int> &lista, int numero)
{
    for (int i = 0; i < (int)lista.size(); i++)
    {
        if (lista[i] == numero)
        {
            return true;
        }
    }
    return false;
}

// =====================================================================
// Tela de abertura
// =====================================================================

void Jogo::executar()
{
    while (true)
    {
        separador();
        mostrarArquivo("telas/abertura.txt");
        int opcao = lerOpcao(1, 4);

        if (opcao == 1) // Novo jogo
        {
            criarPersonagem();
            cenaAtual = 1;
            cenasVisitadas.clear();
            monstrosDerrotados.clear();
            telaInventario();
            jogar();
        }
        else if (opcao == 2) // Carregar jogo
        {
            if (carregarJogo())
            {
                telaInventario();
                jogar();
            }
        }
        else if (opcao == 3)
        {
            mostrarCreditos();
        }
        else
        {
            cout << "Ate a proxima!" << endl;
            return;
        }
    }
}

void Jogo::mostrarCreditos()
{
    separador();
    mostrarArquivo("telas/creditos.txt");
    pausar();
}

// =====================================================================
// Criacao do personagem
// =====================================================================

void Jogo::criarPersonagem()
{
    separador();
    mostrarArquivo("telas/criacao.txt");

    cout << "Nome do personagem: ";
    string nome;
    getline(cin, nome);
    while (nome == "")
    {
        cout << "O nome nao pode ficar vazio: ";
        getline(cin, nome);
    }

    cout << endl << "Como distribuir os 12 pontos?" << endl;
    cout << "  1 - Escolher os valores" << endl;
    cout << "  2 - Aleatorio" << endl;
    int modo = lerOpcao(1, 2);

    // Valores minimos do enunciado
    int habilidade = 6;
    int energia = 12;
    int sorte = 6;

    int guardados = 0; // pontos que o jogador deixa para usar depois

    if (modo == 1)
    {
        // O jogador escolhe quanto vai em cada atributo; o que sobrar fica guardado
        int restantes = 12;

        // Cada atributo aceita no maximo o seu limite (6, 12, 6)
        // ou os pontos que restam, o que for menor.
        int limite = 6;
        if (restantes < limite)
        {
            limite = restantes;
        }
        cout << "Pontos extras em HABILIDADE (0 a " << limite << "):" << endl;
        int extraH = lerOpcao(0, limite);
        restantes = restantes - extraH;

        limite = 12;
        if (restantes < limite)
        {
            limite = restantes;
        }
        cout << "Pontos extras em ENERGIA (0 a " << limite << "):" << endl;
        int extraE = lerOpcao(0, limite);
        restantes = restantes - extraE;

        limite = 6;
        if (restantes < limite)
        {
            limite = restantes;
        }
        cout << "Pontos extras em SORTE (0 a " << limite << "):" << endl;
        int extraS = lerOpcao(0, limite);
        restantes = restantes - extraS;

        habilidade = 6 + extraH;
        energia = 12 + extraE;
        sorte = 6 + extraS;
        guardados = restantes;

        if (guardados > 0)
        {
            cout << "Voce guardou " << guardados << " ponto(s). Use quando quiser na tela de inventario." << endl;
        }
    }
    else
    {
        // Distribui os 12 pontos um por um, sorteando o atributo
        // (se o atributo sorteado ja esta no maximo, sorteia de novo)
        int pontos = 12;
        while (pontos > 0)
        {
            int sorteio = rand() % 3;
            if (sorteio == 0 && habilidade < 12)
            {
                habilidade++;
                pontos--;
            }
            else if (sorteio == 1 && energia < 24)
            {
                energia++;
                pontos--;
            }
            else if (sorteio == 2 && sorte < 12)
            {
                sorte++;
                pontos--;
            }
        }
    }

    if (jogador != nullptr)
    {
        delete jogador; // apaga o personagem de uma partida anterior
    }
    jogador = new Jogador(nome, habilidade, energia, sorte);
    jogador->setPontosGuardados(guardados);

    // Itens basicos do inicio do jogo
    jogador->getInventario()->adicionarItem(criarItem("Espada curta;w;1;1;0"));
    jogador->getInventario()->adicionarItem(criarItem("Gibao de couro;r;1;0;1"));
    jogador->getInventario()->adicionarProvisoes(2);
}

// =====================================================================
// Tela de inventario: ficha, equipar e comer provisao
// =====================================================================

void Jogo::telaInventario()
{
    Inventario *inventario = jogador->getInventario();

    while (true)
    {
        separador();
        cout << "                  INVENTARIO" << endl;
        separador();
        cout << jogador->getNome() << endl;
        cout << "  HABILIDADE: " << jogador->getHabilidade() << endl;
        cout << "  ENERGIA:    " << jogador->getEnergia() << " / " << jogador->getEnergiaMaxima() << endl;
        cout << "  SORTE:      " << jogador->getSorte() << endl;
        cout << "  Pontos guardados: " << jogador->getPontosGuardados() << endl << endl;
        inventario->mostrar();

        cout << endl << "  1 - Equipar arma ou armadura" << endl;
        cout << "  2 - Comer provisao (+4 de energia)" << endl;
        cout << "  3 - Usar pontos guardados" << endl;
        cout << "  0 - Voltar para a aventura" << endl;
        int opcao = lerOpcao(0, 3);

        if (opcao == 0)
        {
            return;
        }
        else if (opcao == 1)
        {
            if (inventario->getQuantidade() == 0)
            {
                cout << "Voce nao tem itens." << endl;
                continue;
            }
            cout << "Numero do item (0 para cancelar):" << endl;
            int numero = lerOpcao(0, inventario->getQuantidade());
            if (numero > 0)
            {
                if (inventario->equipar(numero - 1)) // tela comeca em 1, vetor em 0
                {
                    cout << "Equipado!" << endl;
                }
                else
                {
                    cout << "Esse item nao pode ser equipado." << endl;
                }
            }
        }
        else if (opcao == 2)
        {
            if (jogador->getEnergia() == jogador->getEnergiaMaxima())
            {
                cout << "Sua energia ja esta no maximo." << endl;
            }
            else if (jogador->usarProvisao())
            {
                cout << "Voce comeu uma provisao. Energia: " << jogador->getEnergia() << endl;
            }
            else
            {
                cout << "Voce nao tem provisoes." << endl;
            }
        }
        else if (opcao == 3)
        {
            usarPontosGuardados();
        }
    }
}

void Jogo::usarPontosGuardados()
{
    if (jogador->getPontosGuardados() == 0)
    {
        cout << "Voce nao tem pontos guardados." << endl;
        return;
    }

    while (jogador->getPontosGuardados() > 0)
    {
        cout << endl << "Pontos guardados: " << jogador->getPontosGuardados() << endl;
        cout << "  1 - HABILIDADE (" << jogador->getHabilidade() << "/12)" << endl;
        cout << "  2 - ENERGIA    (" << jogador->getEnergiaMaxima() << "/24)" << endl;
        cout << "  3 - SORTE      (" << jogador->getSorte() << "/12)" << endl;
        cout << "  0 - Voltar" << endl;
        int opcao = lerOpcao(0, 3);

        if (opcao == 0)
        {
            return;
        }

        char atributo = 'h';
        if (opcao == 2)
        {
            atributo = 'e';
        }
        else if (opcao == 3)
        {
            atributo = 's';
        }

        if (!jogador->usarPonto(atributo))
        {
            cout << "Esse atributo ja esta no maximo." << endl;
        }
    }
    cout << "Todos os pontos foram usados." << endl;
}

// =====================================================================
// Loop da aventura
// =====================================================================

void Jogo::jogar()
{
    while (true)
    {
        Cena cena;
        if (!cena.carregar(cenaAtual))
        {
            cout << "Erro: nao foi possivel abrir a cena " << cenaAtual << "." << endl;
            return;
        }

        // Enunciado: sempre que uma nova cena e carregada, o jogo e salvo.
        // Salva antes de marcar como visitada, para que ao carregar a cena
        // seja jogada de novo por completo (inclusive recebendo os itens).
        salvarJogo();
        limparTela();
        int proxima;
        if (cena.getTipo() == 'm')
        {
            proxima = cenaMonstro(cena);
        }
        else if (cena.getTipo() == 's')
        {
            proxima = cenaSorte(cena);
        }
        else
        {
            proxima = cenaNarrativa(cena);
        }

        if (!estaNaLista(cenasVisitadas, cenaAtual))
        {
            cenasVisitadas.push_back(cenaAtual);
        }

        if (!jogador->estaVivo())
        {
            separador();
            cout << "Sua energia chegou a zero. FIM DE JOGO." << endl;
            pausar();
            return;
        }
        if (proxima == 0)
        {
            separador();
            cout << "Obrigado por jogar!" << endl;
            pausar();
            return;
        }
        cenaAtual = proxima;
       
    }
}

int Jogo::cenaNarrativa(Cena &cena)
{
    separador();
    cout << cena.getTexto() << endl;

    // Itens da cena: so na primeira visita
    if (!estaNaLista(cenasVisitadas, cenaAtual))
    {
        for (int i = 0; i < cena.getQuantidadeItens(); i++)
        {
            Item *item = criarItem(cena.getItem(i));
            cout << "Voce encontrou: ";
            item->mostrar();
            jogador->getInventario()->adicionarItem(item);
        }

        if(0 != cena.getProvisoes())        {
            cout << "Voce recebeu ";
            cout << cena.getProvisoes();
            cout << " provisoes!";

            jogador->getInventario()->adicionarProvisoes(cena.getProvisoes());
        }
    }

    // Sem opcoes: e um final da historia
    if (cena.getQuantidadeOpcoes() == 0)
    {
        return 0;
    }

    while (true)
    {
        cout << endl << "O que voce faz?" << endl;
        for (int i = 0; i < cena.getQuantidadeOpcoes(); i++)
        {
            cout << "  " << i + 1 << " - " << cena.getTextoOpcao(i) << endl;
        }
        cout << "  0 - Ver inventario" << endl;

        int opcao = lerOpcao(0, cena.getQuantidadeOpcoes());
        if (opcao == 0)
        {
            telaInventario();
        }
        else
        {
            return cena.getDestinoOpcao(opcao - 1);
        }
    }
}

int Jogo::cenaSorte(Cena &cena)
{
    separador();
    cout << cena.getTexto() << endl;
    cout << "TESTE DE SORTE! (sua sorte: " << jogador->getSorte() << ")" << endl;
    pausar();

    if (jogador->testarSorte())
    {
        cout << "Sucesso! Voce passa sem se ferir." << endl;
        pausar();
        return cena.getDestinoSucesso();
    }

    jogador->receberDano(cena.getDanoFalha());
    cout << "Falhou! Voce perde " << cena.getDanoFalha() << " de energia (energia: "
         << jogador->getEnergia() << ")." << endl;
    pausar();
    return cena.getDestinoFalha();
}

int Jogo::cenaMonstro(Cena &cena)
{
    separador();

    // Monstro ja derrotado em outra visita: o caminho esta livre
    if (estaNaLista(monstrosDerrotados, cenaAtual))
    {
        cout << "O inimigo que voce derrotou aqui nao existe mais. O caminho esta livre." << endl;
        pausar();
        return cena.getDestinoSucesso();
    }

    cout << cena.getTexto() << endl;
    pausar();

    Monstro *monstro = cena.criarMonstro();

    int pontosEvolucao = monstro->getEnergia() / 8;

    bool venceu = batalha(monstro);
    int proxima;

    if (venceu)
    {
        cout << "Voce derrotou " << monstro->getNome() << "!" << endl;
        monstrosDerrotados.push_back(cenaAtual);

        // Transfere o que o monstro carregava para o inventario
        Inventario *inventario = jogador->getInventario();
        Item *item = monstro->entregarItem();
        if (item != nullptr)
        {
            cout << "Voce pegou: ";
            item->mostrar();
            inventario->adicionarItem(item);
        }
        if (monstro->getTesouro() > 0)
        {
            cout << "Voce pegou " << monstro->getTesouro() << " moedas de ouro." << endl;
            inventario->adicionarOuro(monstro->getTesouro());
        }
        if (monstro->getProvisoes() > 0)
        {
            cout << "Voce pegou " << monstro->getProvisoes() << " provisao(oes)." << endl;
            inventario->adicionarProvisoes(monstro->getProvisoes());
        }
        if (pontosEvolucao > 0)
        {
            jogador->setPontosGuardados(jogador->getPontosGuardados() + pontosEvolucao);
            cout << "Voce ganhou " << pontosEvolucao
                 << " ponto(s) de evolucao! Use na tela de inventario." << endl;
        }
        proxima = cena.getDestinoSucesso();
    }
    else
    {
        // Derrota ou fuga: a historia segue pela cena de derrota.
        // Se a energia zerou, o jogador se recupera com metade da energia maxima.
        if (!jogador->estaVivo())
        {
            cout << "Voce foi derrotado..." << endl;
            jogador->setEnergia(jogador->getEnergiaMaxima() / 2);
        }
        proxima = cena.getDestinoFalha();
    }

    pausar();
    delete monstro; // se o item foi entregue, o monstro ja nao o apaga
    return proxima;
}

// =====================================================================
// Batalha
// =====================================================================

// Tela de batalha: mostra o inimigo e a energia, e repete ate alguem cair
// ou o jogador fugir.
bool Jogo::batalha(Monstro *monstro)
{
    while (true)
    {
        separador();
        cout << "                   BATALHA" << endl;
        separador();
        cout << monstro->getNome() << "  -  ENERGIA: " << monstro->getEnergia() << endl;
        cout << jogador->getNome() << "  -  ENERGIA: " << jogador->getEnergia() << endl << endl;
        cout << "  1 - Atacar" << endl;
        cout << "  2 - Atacar testando a sorte (sorte: " << jogador->getSorte() << ")" << endl;
        cout << "  3 - Usar item / magia" << endl;
        cout << "  4 - Fugir" << endl;
        int opcao = lerOpcao(1, 4);

        if (opcao == 1)
        {
            rodadaDeAtaque(monstro, false);
        }
        else if (opcao == 2)
        {
            if (jogador->getSorte() == 0)
            {
                cout << "Voce nao tem mais sorte para testar." << endl;
                continue;
            }
            rodadaDeAtaque(monstro, true);
        }
        else if (opcao == 3)
        {
            usarItemEmCombate(monstro);
        }
        else
        {
            if (monstro->getPodeFugir())
            {
                cout << "Voce foge do combate!" << endl;
                return false;
            }
            cout << "Voce tenta fugir, mas " << monstro->getNome() << " bloqueia o caminho!" << endl;
            continue;
        }

        if (!monstro->estaVivo())
        {
            return true;
        }
        if (!jogador->estaVivo())
        {
            return false;
        }
    }
}

// Uma troca de golpes (regra do enunciado): cada um sorteia 1 a 10 + habilidade.
// Quem tiver a maior Forca de Ataque (FA) tira 2 de energia do outro;
// no empate ninguem acerta. Somam-se os bonus da arma e da armadura do jogador.
//
// Se testarSorte for true, a sorte e testada ANTES do golpe (gasta 1 ponto):
//   jogador acerta: sucesso +2 de dano, falha -1 de dano
//   monstro acerta: sucesso -1 de dano recebido, falha +1 de dano recebido
// Quem acerta sempre tira pelo menos 1 de energia (a armadura reduz, mas nao anula).
void Jogo::rodadaDeAtaque(Monstro *monstro, bool testarSorte)
{
    bool sorteDeuCerto = false;
    if (testarSorte)
    {
        sorteDeuCerto = jogador->testarSorte();
        if (sorteDeuCerto)
        {
            cout << "Voce sente a sorte a seu favor!" << endl;
        }
        else
        {
            cout << "A sorte nao esta com voce..." << endl;
        }
    }

    // Polimorfismo: forcaAtaque() do Jogador soma a arma, a do Monstro nao
    int faJogador = jogador->forcaAtaque();
    int faMonstro = monstro->forcaAtaque() - jogador->protecaoFA();
    cout << "Sua Forca de Ataque: " << faJogador << " | do inimigo: " << faMonstro << endl;

    if (faJogador > faMonstro)
    {
        int dano = jogador->danoAtaque();
        if (testarSorte && sorteDeuCerto)
        {
            dano = dano + 2;
        }
        else if (testarSorte)
        {
            dano = dano - 1;
        }
        if (dano < 1)
        {
            dano = 1; // um golpe que acerta sempre fere
        }
        monstro->receberDano(dano);
        cout << "Voce acertou! " << monstro->getNome() << " perde " << dano << " de energia." << endl;
    }
    else if (faMonstro > faJogador)
    {
        int dano = 2 - jogador->protecaoDano();
        if (testarSorte && sorteDeuCerto)
        {
            dano = dano - 1;
        }
        else if (testarSorte)
        {
            dano = dano + 1;
        }
        if (dano < 1)
        {
            dano = 1; // um golpe que acerta sempre fere, mesmo com armadura
        }
        jogador->receberDano(dano);
        cout << monstro->getNome() << " acertou voce! Voce perde " << dano << " de energia." << endl;
    }
    else
    {
        cout << "Empate! Ninguem acertou." << endl;
    }
}

// Lista os itens comuns usaveis em combate (combate = 1). O item escolhido
// causa o seu dano direto no monstro (a magia nunca erra) e e gasto.
void Jogo::usarItemEmCombate(Monstro *monstro)
{
    Inventario *inventario = jogador->getInventario();

    // Guarda as posicoes (no inventario) dos itens que podem ser usados
    vector<int> posicoes;
    for (int i = 0; i < inventario->getQuantidade(); i++)
    {
        Item *item = inventario->getItem(i);
        if (item->getTipo() == 'c' && item->getCombate())
        {
            posicoes.push_back(i);
        }
    }

    if (posicoes.size() == 0)
    {
        cout << "Voce nao tem itens que possam ser usados em combate." << endl;
        return;
    }

    cout << "Qual item usar? (0 para cancelar)" << endl;
    for (int i = 0; i < (int)posicoes.size(); i++)
    {
        cout << "  " << i + 1 << " - ";
        inventario->getItem(posicoes[i])->mostrar();
    }
    int escolha = lerOpcao(0, posicoes.size());
    if (escolha == 0)
    {
        return;
    }

    int posicao = posicoes[escolha - 1];
    Item *item = inventario->getItem(posicao);
    int dano = item->getDano();

    cout << "Voce usa " << item->getNome() << "! " << monstro->getNome()
         << " perde " << dano << " de energia." << endl;
    monstro->receberDano(dano);
    inventario->removerItem(posicao); // item de uso unico
}

// =====================================================================
// Salvar / Carregar
// =====================================================================

// Formato de data/<nome>.txt (um valor por linha):
//   nome, habilidade, energia atual, energia maxima, sorte, pontos guardados,
//   cena atual,
//   quantidade de cenas visitadas + uma por linha,
//   quantidade de monstros derrotados + um por linha,
//   e depois a parte do inventario (Inventario::salvar)
void Jogo::salvarJogo()
{
    ofstream arquivo;
    arquivo.open("data/" + jogador->getNome() + ".txt");
    if (!arquivo.is_open())
    {
        cout << "(Aviso: nao foi possivel salvar. A pasta data/ existe?)" << endl;
        return;
    }

    arquivo << jogador->getNome() << endl;
    arquivo << jogador->getHabilidade() << endl;
    arquivo << jogador->getEnergia() << endl;
    arquivo << jogador->getEnergiaMaxima() << endl;
    arquivo << jogador->getSorte() << endl;
    arquivo << jogador->getPontosGuardados() << endl;
    arquivo << cenaAtual << endl;

    arquivo << cenasVisitadas.size() << endl;
    for (int i = 0; i < (int)cenasVisitadas.size(); i++)
    {
        arquivo << cenasVisitadas[i] << endl;
    }

    arquivo << monstrosDerrotados.size() << endl;
    for (int i = 0; i < (int)monstrosDerrotados.size(); i++)
    {
        arquivo << monstrosDerrotados[i] << endl;
    }

    jogador->getInventario()->salvar(arquivo);
    arquivo.close();
}

bool Jogo::carregarJogo()
{
    separador();
    cout << "Nome do personagem salvo: ";
    string nome;
    getline(cin, nome);

    ifstream arquivo;
    arquivo.open("data/" + nome + ".txt");
    if (!arquivo.is_open())
    {
        cout << "Nao existe jogo salvo com o nome \"" << nome << "\"." << endl;
        pausar();
        return false;
    }

    string linha;
    getline(arquivo, nome);
    getline(arquivo, linha);
    int habilidade = stoi(linha);
    getline(arquivo, linha);
    int energia = stoi(linha);
    getline(arquivo, linha);
    int energiaMaxima = stoi(linha);
    getline(arquivo, linha);
    int sorte = stoi(linha);
    getline(arquivo, linha);
    int pontosGuardados = stoi(linha);
    getline(arquivo, linha);
    cenaAtual = stoi(linha);

    cenasVisitadas.clear();
    getline(arquivo, linha);
    int quantidade = stoi(linha);
    for (int i = 0; i < quantidade; i++)
    {
        getline(arquivo, linha);
        cenasVisitadas.push_back(stoi(linha));
    }

    monstrosDerrotados.clear();
    getline(arquivo, linha);
    quantidade = stoi(linha);
    for (int i = 0; i < quantidade; i++)
    {
        getline(arquivo, linha);
        monstrosDerrotados.push_back(stoi(linha));
    }

    if (jogador != nullptr)
    {
        delete jogador;
    }
    // O construtor usa a energia como maxima; depois ajustamos a atual
    jogador = new Jogador(nome, habilidade, energiaMaxima, sorte);
    jogador->setEnergia(energia);
    jogador->setPontosGuardados(pontosGuardados);
    jogador->getInventario()->carregar(arquivo);

    arquivo.close();
    cout << "Jogo carregado! Bem-vindo de volta, " << nome << "." << endl;
    return true;
}
