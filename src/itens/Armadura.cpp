#include "../../include/itens/Armadura.h"
#include "../../include/util/Texto.h"

Armadura::Armadura(string nome, bool combate, int fa, int dano)
    : Item(nome, 'r', combate, fa, dano)
{
}

// A armadura atua sobre o oponente: o FA dela sai da FA de quem ataca e o
// dano dela sai do dano recebido, por isso os valores aparecem negativos.
string Armadura::imprimeInfo()
{
    return nome + " (armadura) | FA do oponente " + comSinal(-fa) + " | Dano recebido " + comSinal(-dano);
}
