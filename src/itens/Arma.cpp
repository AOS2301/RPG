#include "../../include/itens/Arma.h"
#include "../../include/util/Texto.h"

Arma::Arma(string nome, bool combate, int fa, int dano)
    : Item(nome, 'w', combate, fa, dano)
{
}

string Arma::imprimeInfo()
{
    return nome + " (arma) | FA " + comSinal(fa) + " | Dano " + comSinal(dano);
}
