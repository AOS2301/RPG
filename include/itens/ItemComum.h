#ifndef ITEMCOMUM_H
#define ITEMCOMUM_H

#include "Item.h"

// Item comum (tipo 'c'). Se combate for 1, pode ser usado na batalha
// (eh assim que o jogador usa magia atraves de itens). Ex: chave, pocao, pergaminho.
class ItemComum : public Item
{
public:
    ItemComum(string nome, bool combate, int fa, int dano);

    void mostrar();
};

#endif
