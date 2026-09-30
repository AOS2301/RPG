#include <iostream>
#include "../../include/itens/ItemComum.h"

// Chama o construtor de Item passando o tipo 'c' fixo.
ItemComum::ItemComum(string nome, bool combate, int fa, int dano) : Item(nome, 'c', combate, fa, dano)
{
}

void ItemComum::mostrar()
{
    cout << "[Item] " << nome;
    if (combate)
    {
        cout << " (usavel em combate: FA +" << fa << ", dano +" << dano << ")";
    }
    cout << endl;
}
