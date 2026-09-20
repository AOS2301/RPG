#include <iostream>
#include "src/personagens/Personagem.cpp"
#include "src/personagens/Jogador.cpp"
#include "src/personagens/Monstro.cpp"
#include "src/armas/Arma.cpp"
#include "src/jogo/Jogo.cpp"
#include "src/jogo/Cena.cpp"

using namespace std;

int main() {
    Jogo jogo;
    jogo.iniciar();

    return 0;
}