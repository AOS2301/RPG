#include "../../include/personagens/Personagem.h"
#include <cstdlib> // rand()

Personagem::Personagem(string nome, int habilidade, int energia, int sorte)
{
    this->nome = nome;
    this->habilidade = habilidade;
    this->energia = energia;
    this->energiaMaxima = energia; // a energia inicial vira o limite
    this->sorte = sorte;
}

Personagem::~Personagem()
{
}

// ------------------------------------------------------------------
// Combate
// ------------------------------------------------------------------

int Personagem::getProtecaoFA()
{
    return 0;
}

int Personagem::getProtecaoDano()
{
    return 0;
}

int Personagem::disputarRodada(Personagem *oponente)
{
    // A armadura de cada um diminui a FA do outro.
    int minhaFA = calcularForcaAtaque() - oponente->getProtecaoFA();
    int faOponente = oponente->calcularForcaAtaque() - getProtecaoFA();

    if (minhaFA > faOponente)
    {
        int dano = calcularDano() - oponente->getProtecaoDano();
        if (dano < DANO_MINIMO)
        {
            dano = DANO_MINIMO;
        }
        oponente->receberDano(dano);
        return 1;
    }
    if (faOponente > minhaFA)
    {
        int dano = oponente->calcularDano() - getProtecaoDano();
        if (dano < DANO_MINIMO)
        {
            dano = DANO_MINIMO;
        }
        receberDano(dano);
        return -1;
    }
    return 0; // empate: ninguem acerta
}

// ------------------------------------------------------------------
// Sorte
// ------------------------------------------------------------------

bool Personagem::testarSorte()
{
    if (sorte <= 0)
    {
        return false; // sem sorte nao ha o que testar
    }
    int sorteio = rolarDado(10);
    bool sucesso = (sorteio <= sorte); // compara com a Sorte ANTES de gastar
    gastarSorte();
    return sucesso;
}

bool Personagem::testarSorteContra(int dificuldade, int &dado)
{
    dado = rolarDado(6);
    bool sucesso = (dado + sorte > dificuldade); // compara com a Sorte ANTES de gastar
    gastarSorte();
    return sucesso;
}

int Personagem::rolarDado(int lados)
{
    return (rand() % lados) + 1;
}

void Personagem::gastarSorte()
{
    if (sorte > 0)
    {
        sorte--;
    }
}

// ------------------------------------------------------------------
// Energia
// ------------------------------------------------------------------

bool Personagem::estaVivo()
{
    return energia > 0;
}

void Personagem::receberDano(int dano)
{
    energia -= dano;
    if (energia < 0)
    {
        energia = 0;
    }
}

void Personagem::recuperarEnergia(int quantidade)
{
    energia += quantidade;
    if (energia > energiaMaxima)
    {
        energia = energiaMaxima;
    }
}

// ------------------------------------------------------------------
// Getters e setters
// ------------------------------------------------------------------

string Personagem::getNome()
{
    return nome;
}

int Personagem::getHabilidade()
{
    return habilidade;
}

int Personagem::getEnergia()
{
    return energia;
}

int Personagem::getEnergiaMaxima()
{
    return energiaMaxima;
}

int Personagem::getSorte()
{
    return sorte;
}

void Personagem::setNome(string nome)
{
    this->nome = nome;
}

void Personagem::setHabilidade(int habilidade)
{
    this->habilidade = habilidade;
}

void Personagem::setEnergia(int energia)
{
    this->energia = energia;
}

void Personagem::setEnergiaMaxima(int energiaMaxima)
{
    this->energiaMaxima = energiaMaxima;
}

void Personagem::setSorte(int sorte)
{
    this->sorte = sorte;
}
