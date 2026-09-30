#ifndef JOGO_H
#define JOGO_H

#include <string>
#include <vector>
#include "../personagens/Jogador.h"
#include "../personagens/Monstro.h"
#include "Cena.h"

using namespace std;

// Controla o jogo inteiro: menus, telas, leitura do teclado,
// o caminho entre as cenas, as batalhas e o salvar/carregar.
// As outras classes guardam dados e regras; o Jogo conversa com o usuario.
class Jogo
{
private:
    Jogador *jogador;              // personagem da partida atual (nullptr se nenhuma)
    int cenaAtual;                 // numero da cena em que o jogador esta
    vector<int> cenasVisitadas;    // cenas ja vistas (itens so sao dados na 1a visita)
    vector<int> monstrosDerrotados; // cenas de monstro ja vencidas

    // ---- Auxiliares de tela e teclado ----
    void separador();
    void pausar();                          // espera o Enter
    int lerOpcao(int minimo, int maximo);   // le um numero dentro do intervalo
    void mostrarArquivo(string caminho);    // imprime um arquivo de telas/
    bool estaNaLista(vector<int> &lista, int numero);

    // ---- Telas ----
    void mostrarCreditos();
    void criarPersonagem();
    void telaInventario();
    void usarPontosGuardados();

    // ---- Aventura ----
    void jogar(); // loop das cenas ate o fim da historia ou a morte

    // Cada tipo de cena devolve o numero da proxima cena (0 = fim da historia)
    int cenaNarrativa(Cena &cena);
    int cenaMonstro(Cena &cena);
    int cenaSorte(Cena &cena);

    bool batalha(Monstro *monstro); // true se venceu o monstro
    void rodadaDeAtaque(Monstro *monstro, bool testarSorte); // uma troca de golpes
    void usarItemEmCombate(Monstro *monstro);

    // ---- Salvar / Carregar (um arquivo por personagem: data/<nome>.txt) ----
    void salvarJogo();
    bool carregarJogo();

public:
    Jogo();
    ~Jogo();

    void executar(); // tela de abertura e menu principal
};

#endif
