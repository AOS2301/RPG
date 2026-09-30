#ifndef PERSONAGEM_H
#define PERSONAGEM_H

#include <string>
using namespace std;

// Superclasse de Jogador e Monstro.
// Guarda os atributos que os dois tem em comum (enunciado):
// HABILIDADE, ENERGIA e SORTE.
// E abstrata: forcaAtaque() e diferente para cada tipo de personagem.
class Personagem
{
protected:
    string nome;
    int habilidade; // destreza em combate
    int energia;    // pontos de vida
    int sorte;      // usada nos testes de sorte

public:
    Personagem(string nome, int habilidade, int energia, int sorte);
    virtual ~Personagem();

    string getNome();
    int getHabilidade();
    int getEnergia();
    int getSorte();

    // Forca de Ataque (FA): numero de 1 a 10 + habilidade (enunciado).
    // Virtual pura: o Jogador soma o bonus da arma, o Monstro nao.
    virtual int forcaAtaque() = 0;

    void receberDano(int dano); // tira energia, sem deixar ficar negativa
    bool estaVivo();            // true enquanto energia > 0

    // Sorteia um numero de 1 a 12: se for menor ou igual a sorte, deu certo.
    // Com sucesso ou nao, a sorte diminui 1 ponto (enunciado).
    bool testarSorte();
};

#endif
