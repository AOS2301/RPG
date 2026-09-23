#ifndef MONSTRO_H
#define MONSTRO_H

#include "Personagem.h"

// Inimigo lido de uma cena de monstro. Luta so com a propria habilidade:
// nao usa arma nem armadura (as protecoes ficam no padrao de Personagem, zero).
class Monstro : public Personagem
{
public:
    Monstro(string nome, int habilidade, int energia, int sorte);
    ~Monstro() override;

    int calcularForcaAtaque() override; // 1d10 + habilidade
    int calcularDano() override;        // dano base
};

#endif // MONSTRO_H
