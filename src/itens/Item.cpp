#include "../../include/itens/Item.h"

Item::Item(string nome, char tipo, bool combate, int fa, int dano)
{
    this->nome = nome;
    this->tipo = tipo;
    this->combate = combate;
    this->fa = fa;
    this->dano = dano;
}

Item::~Item()
{
}

string Item::getNome()
{
    return nome;
}

char Item::getTipo()
{
    return tipo;
}

bool Item::getCombate()
{
    return combate;
}

int Item::getFA()
{
    return fa;
}

int Item::getDano()
{
    return dano;
}
