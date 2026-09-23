#include <iostream>
#include <cstdlib>
#include <ctime>

// O projeto compila com um unico comando (g++ main.cpp -o jogo.exe):
// cada .cpp eh incluido aqui, na ordem das dependencias.
#include "src/util/Texto.cpp"
#include "src/itens/Item.cpp"
#include "src/itens/Arma.cpp"
#include "src/itens/Armadura.cpp"
#include "src/itens/ItemMagico.cpp"
#include "src/inventario/Inventario.cpp"
#include "src/personagens/Personagem.cpp"
#include "src/personagens/Jogador.cpp"
#include "src/personagens/Monstro.cpp"
#include "src/jogo/Cena.cpp"
#include "src/jogo/Jogo.cpp"

using namespace std;

int main()
{
    srand(static_cast<unsigned int>(time(nullptr))); // semeia o rand() com o horario atual, senao todo jogo sorteia sempre a mesma sequencia

    Jogo jogo;
    jogo.iniciar();

    return 0;
}
