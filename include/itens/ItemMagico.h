#ifndef ITEMMAGICO_H
#define ITEMMAGICO_H

#include "Item.h"

// Item magico: item comum (tipo 'c') que pode ser usado em combate.
// O enunciado diz que personagens que nao sao magos usam magia atraves
// de itens. Cada item magico eh de uso unico e causa dano garantido
// (a magia nunca erra, diferente do ataque fisico que depende do FA).
class ItemMagico : public Item
{
public:
    static constexpr int DANO_BASE_MAGIA = 2; // mesma base de um acerto fisico

    ItemMagico(string nome, int fa, int dano);

    int getDanoMagico(); // DANO_BASE_MAGIA + bonus de dano do item
    string imprimeInfo() override;
};

#endif // ITEMMAGICO_H
