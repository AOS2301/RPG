#ifndef JOGADOR_H
#define JOGADOR_H

#include "Personagem.h"
#include "../inventario/Inventario.h"

// Personagem controlado pelo usuario.
// Alem do que todo Personagem tem, possui um inventario (criado com new)
// e uma energia maxima (limite para recuperar energia com provisoes).
class Jogador : public Personagem
{
private:
    Inventario *inventario; // cada jogador tem o seu proprio inventario
    int energiaMaxima;      // energia escolhida na criacao do personagem
    int pontosGuardados;    // pontos da criacao que o jogador deixou para depois

public:
    Jogador(string nome, int habilidade, int energia, int sorte);
    ~Jogador();

    Inventario *getInventario();
    int getEnergiaMaxima();

    // Combate: todos consideram o que esta equipado no inventario
    int forcaAtaque();  // 1 a 10 + habilidade + FA da arma
    int danoAtaque();   // 2 + dano da arma
    int protecaoFA();   // FA da armadura: diminui a Forca de Ataque do monstro
    int protecaoDano(); // dano da armadura: diminui o dano que o jogador recebe

    // Come uma provisao: +4 de energia, sem passar da energia maxima.
    // Retorna false se nao houver provisao.
    bool usarProvisao();

    // Usado ao carregar um jogo salvo e ao se recuperar de uma derrota em combate.
    void setEnergia(int energia);

    // Pontos que sobraram na criacao do personagem
    int getPontosGuardados();
    void setPontosGuardados(int pontos);

    // Gasta 1 ponto guardado em 'h' (habilidade), 'e' (energia) ou 's' (sorte).
    // Retorna false se nao houver pontos ou se o atributo ja estiver no maximo.
    bool usarPonto(char atributo);
};

#endif
