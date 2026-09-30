#ifndef ARMADURA_H
#define ARMADURA_H

#include "Item.h"

// Armadura (tipo 'r'): FA reduz a Forca de Ataque do oponente
// e dano reduz o dano recebido.
class Armadura : public Item
{
public:
    Armadura(string nome, bool combate, int fa, int dano);

    void mostrar();
};

#endif
