#ifndef JOGADOR_H
#define JOGADOR_H

#include "Personagem.h"
#include "../armas/Arma.h"
#include "../inventario/Inventario.h"


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

    // ---- Inventario (armas, itens, ouro, provisoes) ----
    Inventario& getInventario();
    void equiparArma(Arma* arma); // atalho para getInventario().equiparArma
    Arma* getArmaEquipada();      // atalho para getInventario().getArmaEquipada

    // ---- Evolucao ----
    int getNivel();
    int getExperiencia();
    int getExperienciaParaProximoNivel();
    int getPontosEvolucao();
    int ganharExperiencia(int quantidade); // retorna quantos niveis subiu
    bool podeEvoluir();                    // ha pontos E algum atributo abaixo do limite?
    bool evoluirAtributo(char atributo);   // 'h' habilidade, 'e' energia, 's' sorte

    // Setters usados so para restaurar um jogo salvo (carregarJogo).
    void setNivel(int nivel);
    void setExperiencia(int experiencia);
    void setPontosEvolucao(int pontosEvolucao);

    // ---- Recursos (atalhos para o Inventario) ----
    void adicionarOuro(int quantidade);
    int getOuro();
    void adicionarProvisoes(int quantidade);
    int getProvisoes();
    bool usarProvisao();                   // +4 de energia (so fora de combate)

protected:
    Inventario inventario;
    int nivel;
    int experiencia;     // experiencia acumulada dentro do nivel atual
    int pontosEvolucao;  // pontos ainda nao distribuidos
};

#endif // JOGADOR_H
