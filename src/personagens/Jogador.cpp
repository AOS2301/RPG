#include "../../include/personagens/Jogador.h"
#include <iostream>
#include <cstdlib> // rand()

Jogador::Jogador(string nome, int habilidade, int energia, int sorte)
    : Personagem(nome, habilidade, energia, sorte),
      nivel(1), experiencia(0), pontosEvolucao(0) {}

Jogador::~Jogador() {
    // O Inventario eh membro por valor: seu proprio destrutor ja libera
    // as armas guardadas (ver Inventario::~Inventario).
}

Inventario& Jogador::getInventario() {
    return inventario;
}

void Jogador::equiparArma(Arma* arma) {
    inventario.equiparArma(arma);
}

Arma* Jogador::getArmaEquipada() {
    return inventario.getArmaEquipada();
}

void Jogador::atacar(Personagem* alvo) {
    Arma* armaEquipada = inventario.getArmaEquipada();
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

// Setters usados so para restaurar um jogo salvo (carregarJogo).
void Jogador::setNivel(int nivel) { this->nivel = nivel; }
void Jogador::setExperiencia(int experiencia) { this->experiencia = experiencia; }
void Jogador::setPontosEvolucao(int pontosEvolucao) { this->pontosEvolucao = pontosEvolucao; }

// ------------------------------------------------------------------
// Recursos (atalhos para o Inventario, que eh quem guarda os valores)
// ------------------------------------------------------------------

void Jogador::adicionarOuro(int quantidade) { inventario.adicionarOuro(quantidade); }
int Jogador::getOuro() { return inventario.getOuro(); }

void Jogador::adicionarProvisoes(int quantidade) { inventario.adicionarProvisoes(quantidade); }
int Jogador::getProvisoes() { return inventario.getProvisoes(); }

// Uma provisao recupera sempre 4 pontos de energia (enunciado).
// Retorna false se nao ha provisao ou se a energia ja esta cheia.
bool Jogador::usarProvisao() {
    if (energia >= energiaMaxima) {
        return false;
    }
    if (!inventario.consumirProvisao()) {
        return false;
    }

    recuperarEnergia(4);
    return true;
}
