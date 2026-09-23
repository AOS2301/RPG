#ifndef ARMADURA_H
#define ARMADURA_H

#include "Item.h"

// Armadura (tipo 'r'): FA eh descontado da Forca de Ataque do oponente e
// dano eh quanto do dano recebido eh reduzido (enunciado).
class Armadura : public Item
{
public:
    Armadura(string nome, bool combate, int fa, int dano);

    string imprimeInfo() override;
};

#endif // ARMADURA_H
