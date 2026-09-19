#include "../../include/personagens/Jogador.h"
#include <iostream>

Jogador::Jogador(string nome, int habilidade, int energia, int sorte)
    : Personagem(nome, habilidade, energia, sorte) {}

void Jogador::atacar(Personagem* alvo) {
    int bonusFA = (armaEquipada != nullptr) ? armaEquipada->getFA() : 0;
    int bonusDano = (armaEquipada != nullptr) ? armaEquipada->getDano() : 0;

    int faAtacante = getHabilidade() + (rand() % 10 + 1) + bonusFA;
    int faAlvo = alvo->getHabilidade() + (rand() % 10 + 1);

    if (faAtacante > faAlvo) {
        alvo->receberDano(2 + bonusDano); // vence a rodada E causa mais dano
    } else if (faAtacante < faAlvo) {
        this->receberDano(2); // perde a rodada, sem bônus de arma na defesa
    }
    // empate: ninguém acerta
}

Jogador::~Jogador() {
}