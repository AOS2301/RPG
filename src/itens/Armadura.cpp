#include <iostream>
#include "../../include/itens/Armadura.h"

// Chama o construtor de Item passando o tipo 'r' fixo.
Armadura::Armadura(string nome, bool combate, int fa, int dano) : Item(nome, 'r', combate, fa, dano)
{
}

void Armadura::mostrar()
{
    cout << "[Armadura] " << nome << " (FA do oponente -" << fa << ", dano recebido -" << dano << ")" << endl;
}
