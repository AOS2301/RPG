#ifndef JOGO_H
#define JOGO_H

#include <string>
#include <vector>
#include "../personagens/Jogador.h"
#include "../personagens/Monstro.h"
#include "Cena.h"

using namespace std;

// Controla o fluxo do jogo: tela de abertura, criacao do personagem
// e o loop que anda de cena em cena.
class Jogo
{
public:
    Jogo();
    ~Jogo();

    void iniciar(); // tela de abertura + menu principal
    
    // Telas e entrada
    void exibirArquivo(string caminho);
    int lerInteiro();
    void mostrarCreditos();
    void criarPersonagem();
    void mostrarFicha();
    void menuFicha();          // ficha + comer provisao + distribuir pontos
    void distribuirPontos();   // tela de evolucao do personagem

    // Loop da aventura
    void jogar();
    void executarCenaNormal(Cena& cena, bool& fim, bool primeiraVez);
    void executarCenaMonstro(Cena& cena);
    bool batalha(Cena& cena);
    void receberItem(string linha);

    // Controle do que ja aconteceu na aventura
    bool foiVisitada(int numeroCena);
    bool jaDerrotou(int numeroCena);
private:
    Jogador* jogador;
    int cenaAtual;
    vector<int> cenasVisitadas;     // cenas que o jogador ja viu
    vector<int> monstrosDerrotados; // cenas de monstro ja vencidas
};

#endif // JOGO_H
