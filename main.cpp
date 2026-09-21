#include <iostream>
#include <cstdlib>
#include <ctime>
#include "src/armas/Arma.cpp"
#include "src/inventario/Inventario.cpp"
#include "src/personagens/Personagem.cpp"
#include "src/personagens/Jogador.cpp"
#include "src/personagens/Monstro.cpp"
#include "src/jogo/Jogo.cpp"
#include "src/jogo/Cena.cpp"

using namespace std;

int main() {
    srand(static_cast<unsigned int>(time(nullptr))); // semeia o gerador de numeros aleatorios (rand()) com o horario atual, senao todo jogo sorteia sempre a mesma sequencia

    Jogo jogo;
    jogo.iniciar();

    return 0;
}