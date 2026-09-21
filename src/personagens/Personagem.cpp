#include "../../include/personagens/Personagem.h"
#include <iostream>
#include <cstdlib> // rand()

Personagem::Personagem(string nome, int habilidade, int energia, int sorte) {
    this->nome = nome;
    this->habilidade = habilidade;
    this->energia = energia;
    this->energiaMaxima = energia; // a energia inicial vira o limite
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

void Personagem::recuperarEnergia(int quantidade) {
    energia += quantidade;
    if (energia > energiaMaxima) {
        energia = energiaMaxima;
    }
}

bool Personagem::testarSorte() {
    if (sorte <= 0) {
        return false; // sem sorte nao ha o que testar (e evita sorte negativa)
    }
    int sorteio = (rand() % 10) + 1;
    sorte--; // regra do enunciado: cada uso de sorte decrementa o valor atual
    return sorteio <= sorte;
}

void Personagem::decrementarSorte() {
    if (sorte > 0) {
        sorte--;
    }
}

string Personagem::getNome(){ 
    return nome; 
}

int Personagem::getHabilidade(){ 
    return habilidade; 
}

int Personagem::getEnergia(){
    return energia; 
}

int Personagem::getEnergiaMaxima(){
    return energiaMaxima;
}

int Personagem::getSorte(){ 
    return sorte; 
}

void Personagem::setNome(string nome) {
    this->nome = nome;
}

void Personagem::setHabilidade(int habilidade) {
    this->habilidade = habilidade;
}

void Personagem::setEnergia(int energia) {
    this->energia = energia;
}

void Personagem::setEnergiaMaxima(int energiaMaxima) {
    this->energiaMaxima = energiaMaxima;
}

void Personagem::setSorte(int sorte) {
    this->sorte = sorte;
}
