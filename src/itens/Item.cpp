#include "../../include/itens/Item.h"

Item::Item(string nome, char tipo, bool combate, int fa, int dano)
    : nome(nome), tipo(tipo), combate(combate), fa(fa), dano(dano)
{
}

Item::~Item()
{
}

string Item::imprimeInfo()
{
    return nome + " (item)";
}

string Item::paraLinha()
{
    return nome + ";" + tipo + ";" + (combate ? "1" : "0") + ";" + to_string(fa) + ";" + to_string(dano);
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
