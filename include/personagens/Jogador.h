#ifndef JOGADOR_H
#define JOGADOR_H

#include "Personagem.h"
#include "../armas/Arma.h"


class Jogador : public Personagem
{
public:
    // Limites dos atributos: os mesmos da criacao do personagem (enunciado).
    static constexpr int MAX_HABILIDADE = 12;
    static constexpr int MAX_ENERGIA = 24;
    static constexpr int MAX_SORTE = 12;

    Jogador(string nome, int habilidade, int energia, int sorte);

    ~Jogador() override;

    void atacar(Personagem* alvo) override;
    void equiparArma(Arma* arma);
    Arma* getArmaEquipada();

    // ---- Evolucao ----
    int getNivel();
    int getExperiencia();
    int getExperienciaParaProximoNivel();
    int getPontosEvolucao();
    int ganharExperiencia(int quantidade); // retorna quantos niveis subiu
    bool podeEvoluir();                    // ha pontos E algum atributo abaixo do limite?
    bool evoluirAtributo(char atributo);   // 'h' habilidade, 'e' energia, 's' sorte

    // ---- Recursos ----
    void adicionarOuro(int quantidade);
    int getOuro();
    void adicionarProvisoes(int quantidade);
    int getProvisoes();
    bool usarProvisao();                   // +4 de energia (so fora de combate)

protected:
    Arma* armaEquipada;
    int nivel;
    int experiencia;     // experiencia acumulada dentro do nivel atual
    int pontosEvolucao;  // pontos ainda nao distribuidos
    int ouro;
    int provisoes;
};

#endif // JOGADOR_H
