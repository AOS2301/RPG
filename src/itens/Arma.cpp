#include <iostream>
#include "../../include/itens/Arma.h"

// Chama o construtor de Item passando o tipo 'w' fixo.
Arma::Arma(string nome, bool combate, int fa, int dano) : Item(nome, 'w', combate, fa, dano)
{
}

void Arma::mostrar()
{
    cout << "[Arma] " << nome << " (FA +" << fa << ", dano +" << dano << ")" << endl;
}
