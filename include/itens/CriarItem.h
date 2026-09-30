#ifndef CRIARITEM_H
#define CRIARITEM_H

#include <string>
#include "Item.h"

using namespace std;

// Funcao comum (nao e metodo de classe).
// Recebe uma linha no formato do enunciado: "nome;tipo;combate;FA;dano"
// (ex: "Adaga de prata;w;1;1;1") e cria o objeto certo com new:
//   'w' -> Arma, 'r' -> Armadura, qualquer outro -> ItemComum
// Quem recebe o ponteiro vira o dono do item.
Item *criarItem(string linha);

#endif
