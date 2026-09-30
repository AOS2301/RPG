#include <cstdlib>
#include "../../include/personagens/Personagem.h"

Personagem::Personagem(string nome, int habilidade, int energia, int sorte)
{
    this->nome = nome;
    this->habilidade = habilidade;
    this->energia = energia;
    this->sorte = sorte;
}

Personagem::~Personagem()
{
}

string Personagem::getNome()
{
    return nome;
}

int Personagem::getHabilidade()
{
    return habilidade;
}

int Personagem::getEnergia()
{
    return energia;
}

int Personagem::getSorte()
{
    return sorte;
}

void Personagem::receberDano(int dano)
{
    energia = energia - dano;
    if (energia < 0)
    {
        energia = 0;
    }
}

bool Personagem::estaVivo()
{
    return energia > 0;
}

bool Personagem::testarSorte()
{
    int valor = rand() % 12 + 1; // numero de 1 a 12
    bool sucesso = (valor <= sorte);

    if (sorte > 0)
    {
        sorte--; // cada uso da sorte gasta 1 ponto
    }

    return sucesso;
}
