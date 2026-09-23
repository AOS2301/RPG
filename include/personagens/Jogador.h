#ifndef JOGADOR_H
#define JOGADOR_H

#include "Personagem.h"
#include "../inventario/Inventario.h"

// Personagem controlado pelo usuario. Alem do que todo Personagem tem,
// possui inventario (arma e armadura equipadas contam no combate),
// usa itens magicos, testa a sorte em combate e evolui de nivel.
// Assim como Personagem, nao imprime nada: o Jogo mostra os resultados.
class Jogador : public Personagem
{
public:
    // Limites dos atributos: os mesmos da criacao do personagem (enunciado).
    static constexpr int MAX_HABILIDADE = 12;
    static constexpr int MAX_ENERGIA = 24;
    static constexpr int MAX_SORTE = 12;

    Jogador(string nome, int habilidade, int energia, int sorte);
    ~Jogador() override;

    // ---- Combate (versoes do Jogador, que consideram os equipamentos) ----
    int calcularForcaAtaque() override; // 1d10 + habilidade + FA da arma
    int calcularDano() override;        // dano base + dano da arma
    int getProtecaoFA() override;       // FA da armadura equipada
    int getProtecaoDano() override;     // dano da armadura equipada

    // Testa a Sorte depois de uma rodada em que alguem foi ferido (enunciado:
    // a sorte amplia o dano causado ou reduz o dano recebido).
    //   venceuRodada=true : sucesso +2 de dano no oponente, falha -1
    //   venceuRodada=false: sucesso -1 de dano recebido, falha +1
    // Retorna true se o teste teve sucesso. So deve ser chamado com Sorte > 0.
    bool usarSorteEmCombate(Personagem *oponente, bool venceuRodada);

    // Usa (e consome) o item magico da posicao "indice" contra o alvo.
    // A magia nunca erra. Retorna o dano causado (0 se o indice for invalido).
    int usarItemMagico(int indice, Personagem *alvo);

    // Derrotado (em combate ou numa queda), o jogador nao morre: a historia
    // continua com metade da energia maxima.
    void recuperarAposDerrota();

    // ---- Inventario (armas, armaduras, itens, ouro, provisoes) ----
    Inventario &getInventario();
    bool usarProvisao(); // +4 de energia (so fora de combate)

    // ---- Evolucao ----
    int getNivel();
    int getExperiencia();
    int getExperienciaParaProximoNivel();
    int getPontosEvolucao();
    int ganharExperiencia(int quantidade); // retorna quantos niveis subiu
    bool podeEvoluir();                    // ha pontos E algum atributo abaixo do limite?
    bool evoluirAtributo(char atributo);   // 'h' habilidade, 'e' energia, 's' sorte

    // Setters usados na criacao do personagem e ao restaurar um jogo salvo.
    void setNivel(int nivel);
    void setExperiencia(int experiencia);
    void setPontosEvolucao(int pontosEvolucao);

protected:
    Inventario inventario;
    int nivel;
    int experiencia;    // experiencia acumulada dentro do nivel atual
    int pontosEvolucao; // pontos ainda nao distribuidos
};

#endif // JOGADOR_H
