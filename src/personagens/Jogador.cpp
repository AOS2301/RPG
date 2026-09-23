#include "../../include/personagens/Jogador.h"

Jogador::Jogador(string nome, int habilidade, int energia, int sorte)
    : Personagem(nome, habilidade, energia, sorte),
      nivel(1), experiencia(0), pontosEvolucao(0)
{
}

Jogador::~Jogador()
{
    // O Inventario eh membro por valor: seu proprio destrutor ja libera
    // os itens guardados (ver Inventario::~Inventario).
}

// ------------------------------------------------------------------
// Combate
// ------------------------------------------------------------------

int Jogador::calcularForcaAtaque()
{
    int forca = habilidade + rolarDado(10);
    Arma *arma = inventario.getArmaEquipada();
    if (arma != nullptr)
    {
        forca += arma->getFA();
    }
    return forca;
}

int Jogador::calcularDano()
{
    int dano = DANO_BASE;
    Arma *arma = inventario.getArmaEquipada();
    if (arma != nullptr)
    {
        dano += arma->getDano();
    }
    return dano;
}

int Jogador::getProtecaoFA()
{
    Armadura *armadura = inventario.getArmaduraEquipada();
    if (armadura == nullptr)
    {
        return 0;
    }
    return armadura->getFA();
}

int Jogador::getProtecaoDano()
{
    Armadura *armadura = inventario.getArmaduraEquipada();
    if (armadura == nullptr)
    {
        return 0;
    }
    return armadura->getDano();
}

bool Jogador::usarSorteEmCombate(Personagem *oponente, bool venceuRodada)
{
    if (sorte <= 0)
    {
        return false; // sem sorte o teste nem acontece
    }

    bool sucesso = testarSorte(); // gasta 1 ponto de Sorte

    if (venceuRodada)
    {
        if (sucesso)
        {
            oponente->receberDano(2); // golpe mais forte
        }
        else
        {
            oponente->recuperarEnergia(1); // golpe perdeu forca
        }
    }
    else
    {
        if (sucesso)
        {
            recuperarEnergia(1); // amorteceu o golpe
        }
        else
        {
            receberDano(1); // golpe pior do que parecia
        }
    }

    return sucesso;
}

int Jogador::usarItemMagico(int indice, Personagem *alvo)
{
    ItemMagico *item = inventario.removerItemMagico(indice);
    if (item == nullptr)
    {
        return 0;
    }

    int dano = item->getDanoMagico();
    alvo->receberDano(dano);

    delete item; // uso unico: o item foi consumido
    return dano;
}

void Jogador::recuperarAposDerrota()
{
    energia = (energiaMaxima + 1) / 2;
}

// ------------------------------------------------------------------
// Inventario
// ------------------------------------------------------------------

Inventario &Jogador::getInventario()
{
    return inventario;
}

// Uma provisao recupera sempre 4 pontos de energia (enunciado).
// Retorna false se nao ha provisao ou se a energia ja esta cheia.
bool Jogador::usarProvisao()
{
    if (energia >= energiaMaxima)
    {
        return false;
    }
    if (!inventario.consumirProvisao())
    {
        return false;
    }

    recuperarEnergia(4);
    return true;
}

// ------------------------------------------------------------------
// Evolucao
// ------------------------------------------------------------------

int Jogador::getNivel()
{
    return nivel;
}

int Jogador::getExperiencia()
{
    return experiencia;
}

int Jogador::getPontosEvolucao()
{
    return pontosEvolucao;
}

// Cada nivel pede mais experiencia que o anterior: 15, 30, 45...
int Jogador::getExperienciaParaProximoNivel()
{
    return 15 * nivel;
}

int Jogador::ganharExperiencia(int quantidade)
{
    if (quantidade <= 0)
    {
        return 0;
    }

    experiencia += quantidade;
    int niveisGanhos = 0;

    while (experiencia >= getExperienciaParaProximoNivel())
    {
        experiencia -= getExperienciaParaProximoNivel();
        nivel++;
        pontosEvolucao += 2; // cada nivel rende 2 pontos para distribuir
        niveisGanhos++;
    }

    if (niveisGanhos > 0)
    {
        energia = energiaMaxima; // subir de nivel restaura a energia
    }

    return niveisGanhos;
}

bool Jogador::podeEvoluir()
{
    if (pontosEvolucao <= 0)
    {
        return false;
    }
    return habilidade < MAX_HABILIDADE || energiaMaxima < MAX_ENERGIA || sorte < MAX_SORTE;
}

// Gasta 1 ponto no atributo escolhido. Retorna false se nao foi possivel.
bool Jogador::evoluirAtributo(char atributo)
{
    if (pontosEvolucao <= 0)
    {
        return false;
    }

    if (atributo == 'h' && habilidade < MAX_HABILIDADE)
    {
        habilidade++;
    }
    else if (atributo == 'e' && energiaMaxima < MAX_ENERGIA)
    {
        energiaMaxima++;
        energia++;
    }
    else if (atributo == 's' && sorte < MAX_SORTE)
    {
        sorte++;
    }
    else
    {
        return false; // atributo invalido ou ja no limite
    }

    pontosEvolucao--;
    return true;
}

void Jogador::setNivel(int nivel)
{
    this->nivel = nivel;
}

void Jogador::setExperiencia(int experiencia)
{
    this->experiencia = experiencia;
}

void Jogador::setPontosEvolucao(int pontosEvolucao)
{
    this->pontosEvolucao = pontosEvolucao;
}
