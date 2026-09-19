#include "../../include/armas/Arma.h"

Arma::Arma(string nome, bool combate, int fa, int dano)
    : nome(nome), combate(combate), fa(fa), dano(dano) {}

string Arma::imprimeInfo() {
    string info = nome + " | FA: " + to_string(fa) + " | Dano: " + to_string(dano);
    info += combate ? " | Uso em combate: Sim" : " | Uso em combate: Nao";
    return info;
}

// Getters
string Arma::getNome(){
    return nome;
}

bool Arma::getCombate(){
    return combate;
}

int Arma::getFA(){
    return fa;
}

int Arma::getDano(){
    return dano;
}

// Setters
void Arma::setNome(string nome) {
    this->nome = nome;
}

void Arma::setCombate(bool combate) {
    this->combate = combate;
}

void Arma::setFA(int fa) {
    this->fa = fa;
}

void Arma::setDano(int dano) {
    this->dano = dano;
}