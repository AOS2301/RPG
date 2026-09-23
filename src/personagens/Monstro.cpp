#include "../../include/personagens/Monstro.h"

Monstro::Monstro(string nome, int habilidade, int energia, int sorte)
    : Personagem(nome, habilidade, energia, sorte)
{
}

Monstro::~Monstro()
{
}

int Monstro::calcularForcaAtaque()
{
    return habilidade + rolarDado(10);
}

int Monstro::calcularDano()
{
    return DANO_BASE;
}
