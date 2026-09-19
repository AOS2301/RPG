#ifndef JOGADOR_H
#define JOGADOR_H

#include "Personagem.h"
#include "../armas/Arma.h"


class Jogador : public Personagem
{
public:
    Jogador(string nome, int habilidade, int energia, int sorte);

    ~Jogador() override;

    void atacar(Personagem* alvo) override;

protected:
    Arma* armaEquipada;
};

#endif // JOGADOR_H