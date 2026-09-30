#include <cstdlib>
#include "../../include/personagens/Jogador.h"

// Os 4 atributos vao para o construtor de Personagem.
// O inventario e criado aqui e apagado no destrutor.
Jogador::Jogador(string nome, int habilidade, int energia, int sorte)
    : Personagem(nome, habilidade, energia, sorte)
{
    inventario = new Inventario();
    energiaMaxima = energia;
    pontosGuardados = 0;
}

// Apagar o inventario tambem apaga todos os itens dele (destrutor do Inventario).
Jogador::~Jogador()
{
    delete inventario;
}

Inventario *Jogador::getInventario()
{
    return inventario;
}

int Jogador::getEnergiaMaxima()
{
    return energiaMaxima;
}

int Jogador::forcaAtaque()
{
    int fa = rand() % 10 + 1 + habilidade;

    Item *arma = inventario->getArmaEquipada();
    if (arma != nullptr)
    {
        fa = fa + arma->getFA();
    }
    return fa;
}

int Jogador::danoAtaque()
{
    int dano = 2; // dano padrao de quem vence a rodada (enunciado)

    Item *arma = inventario->getArmaEquipada();
    if (arma != nullptr)
    {
        dano = dano + arma->getDano();
    }
    return dano;
}

int Jogador::protecaoFA()
{
    Item *armadura = inventario->getArmaduraEquipada();
    if (armadura == nullptr)
    {
        return 0;
    }
    return armadura->getFA();
}

int Jogador::protecaoDano()
{
    Item *armadura = inventario->getArmaduraEquipada();
    if (armadura == nullptr)
    {
        return 0;
    }
    return armadura->getDano();
}

bool Jogador::usarProvisao()
{
    if (!inventario->gastarProvisao())
    {
        return false; // nao tinha provisao
    }

    energia = energia + 4;
    if (energia > energiaMaxima)
    {
        energia = energiaMaxima;
    }
    return true;
}

void Jogador::setEnergia(int energia)
{
    this->energia = energia;
}

int Jogador::getPontosGuardados()
{
    return pontosGuardados;
}

void Jogador::setPontosGuardados(int pontos)
{
    pontosGuardados = pontos;
}

bool Jogador::usarPonto(char atributo)
{
    if (pontosGuardados <= 0)
    {
        return false;
    }

    // Maximos do enunciado: HABILIDADE 12, ENERGIA 24, SORTE 12
    if (atributo == 'h' && habilidade < 12)
    {
        habilidade++;
    }
    else if (atributo == 'e' && energiaMaxima < 24)
    {
        energiaMaxima++;
        energia++; // o ponto novo de energia ja vem "cheio"
    }
    else if (atributo == 's' && sorte < 12)
    {
        sorte++;
    }
    else
    {
        return false; // atributo ja esta no maximo
    }

    pontosGuardados--;
    return true;
}
