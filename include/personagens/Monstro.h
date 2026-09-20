#ifndef MONSTRO_H
#define MONSTRO_H

#include "Personagem.h"

class Monstro : public Personagem
{
public:
    Monstro(string nome, int habilidade, int energia, int sorte);

    ~Monstro() override;

    void atacar(Personagem* alvo) override;
};

#endif // MONSTRO_H
