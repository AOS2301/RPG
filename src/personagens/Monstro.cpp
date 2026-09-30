#include <cstdlib>
#include "../../include/personagens/Monstro.h"

// Os 4 primeiros dados vao para o construtor de Personagem.
Monstro::Monstro(string nome, int habilidade, int energia, int sorte,
                 int tesouro, int provisoes, Item *item, bool podeFugir)
    : Personagem(nome, habilidade, energia, sorte)
{
    this->tesouro = tesouro;
    this->provisoes = provisoes;
    this->item = item;
    this->podeFugir = podeFugir;
}

// Se o monstro ainda tem o item (nao foi derrotado), libera a memoria.
Monstro::~Monstro()
{
    if (item != nullptr)
    {
        delete item;
    }
}

int Monstro::forcaAtaque()
{
    return rand() % 10 + 1 + habilidade;
}

int Monstro::getTesouro()
{
    return tesouro;
}

int Monstro::getProvisoes()
{
    return provisoes;
}

bool Monstro::getPodeFugir()
{
    return podeFugir;
}

Item *Monstro::entregarItem()
{
    Item *entregue = item;
    item = nullptr; // o monstro nao e mais dono do item
    return entregue;
}
