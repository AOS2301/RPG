#include "../../include/itens/ItemMagico.h"

ItemMagico::ItemMagico(string nome, int fa, int dano)
    : Item(nome, 'c', true, fa, dano)
{
}

int ItemMagico::getDanoMagico()
{
    return DANO_BASE_MAGIA + dano;
}

string ItemMagico::imprimeInfo()
{
    return nome + " (item magico) | Dano magico: " + to_string(getDanoMagico());
}
