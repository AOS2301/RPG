#include "../include/personagens/Personagem.h"
#include <iostream>
#include <cstdlib> // rand()

Personagem::Personagem(string nome, int habilidade, int energia, int sorte) {
    this->nome = nome;
    this->habilidade = habilidade;
    this->energia = energia;
    this->sorte = sorte;
}

Personagem::~Personagem() {
}

bool Personagem::estaVivo(){
    return energia > 0;
}

void Personagem::receberDano(int dano) {
    energia -= dano;
    if (energia < 0) {
        energia = 0;
    }
}

bool Personagem::testarSorte() {
    int sorteio = (rand() % 10) + 1;
    sorte--; // regra do enunciado: cada uso de sorte decrementa o valor atual
    return sorteio <= sorte;
}

string Personagem::getNome(){ 
    return nome; 
}

int Personagem::getHabilidade() const { 
    return habilidade; 
}

int Personagem::getEnergia() const {
    return energia; 
}

int Personagem::getSorte() const { 
    return sorte; 
}