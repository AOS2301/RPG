#include "../../include/personagens/Monstro.h"
#include <cstdlib> // rand()

Monstro::Monstro(string nome, int habilidade, int energia, int sorte)
    : Personagem(nome, habilidade, energia, sorte) {}

// Mesma regra de rodada do Jogador, mas o monstro nao usa arma.
// No jogo, a rodada eh disparada pelo jogador (Jogador::atacar);
// este metodo existe porque atacar() eh virtual puro em Personagem.
void Monstro::atacar(Personagem* alvo) {
    int faAtacante = getHabilidade() + (rand() % 10 + 1);
    int faAlvo = alvo->getHabilidade() + (rand() % 10 + 1);

    if (faAtacante > faAlvo) {
        alvo->receberDano(2);
    } else if (faAtacante < faAlvo) {
        this->receberDano(2);
    }
    // empate: ninguem acerta
}

Monstro::~Monstro() {
}
