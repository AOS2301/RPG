#include "../../include/personagens/Jogador.h"
#include <iostream>
#include <cstdlib> // rand()

Jogador::Jogador(string nome, int habilidade, int energia, int sorte)
    : Personagem(nome, habilidade, energia, sorte), armaEquipada(nullptr),
      nivel(1), experiencia(0), pontosEvolucao(0), ouro(0), provisoes(0) {}

Jogador::~Jogador() {
    delete armaEquipada;
}

void Jogador::equiparArma(Arma* arma) {
    delete armaEquipada;
    armaEquipada = arma;
}

Arma* Jogador::getArmaEquipada() {
    return armaEquipada;
}

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

// ------------------------------------------------------------------
// Evolucao
// ------------------------------------------------------------------

int Jogador::getNivel() { return nivel; }
int Jogador::getExperiencia() { return experiencia; }
int Jogador::getPontosEvolucao() { return pontosEvolucao; }

// Cada nivel pede mais experiencia que o anterior: 15, 30, 45...
int Jogador::getExperienciaParaProximoNivel() {
    return 15 * nivel;
}

int Jogador::ganharExperiencia(int quantidade) {
    if (quantidade <= 0) {
        return 0;
    }

    experiencia += quantidade;
    int niveisGanhos = 0;

    while (experiencia >= getExperienciaParaProximoNivel()) {
        experiencia -= getExperienciaParaProximoNivel();
        nivel++;
        pontosEvolucao += 2; // cada nivel rende 2 pontos para distribuir
        niveisGanhos++;
    }

    if (niveisGanhos > 0) {
        energia = energiaMaxima; // subir de nivel restaura a energia
    }

    return niveisGanhos;
}

bool Jogador::podeEvoluir() {
    if (pontosEvolucao <= 0) {
        return false;
    }
    return habilidade < MAX_HABILIDADE || energiaMaxima < MAX_ENERGIA || sorte < MAX_SORTE;
}

// Gasta 1 ponto no atributo escolhido. Retorna false se nao foi possivel.
bool Jogador::evoluirAtributo(char atributo) {
    if (pontosEvolucao <= 0) {
        return false;
    }

    if (atributo == 'h' && habilidade < MAX_HABILIDADE) {
        habilidade++;
    }
    else if (atributo == 'e' && energiaMaxima < MAX_ENERGIA) {
        energiaMaxima++;
        energia++;
    }
    else if (atributo == 's' && sorte < MAX_SORTE) {
        sorte++;
    }
    else {
        return false; // atributo invalido ou ja no limite
    }

    pontosEvolucao--;
    return true;
}

// ------------------------------------------------------------------
// Recursos
// ------------------------------------------------------------------

void Jogador::adicionarOuro(int quantidade) {
    if (quantidade > 0) {
        ouro += quantidade;
    }
}

int Jogador::getOuro() { return ouro; }

void Jogador::adicionarProvisoes(int quantidade) {
    if (quantidade > 0) {
        provisoes += quantidade;
    }
}

int Jogador::getProvisoes() { return provisoes; }

// Uma provisao recupera sempre 4 pontos de energia (enunciado).
// Retorna false se nao ha provisao ou se a energia ja esta cheia.
bool Jogador::usarProvisao() {
    if (provisoes <= 0 || energia >= energiaMaxima) {
        return false;
    }

    provisoes--;
    recuperarEnergia(4);
    return true;
}
