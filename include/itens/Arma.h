#ifndef ARMA_H
#define ARMA_H

#include "Item.h"

// Arma (tipo 'w'): FA soma na Forca de Ataque de quem ataca
// e dano aumenta o dano causado no oponente.
class Arma : public Item
{
public:
    Arma(string nome, bool combate, int fa, int dano);

    void mostrar();
};

#endif
