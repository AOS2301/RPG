#ifndef PERSONAGEM_H
#define PERSONAGEM_H
#include <iostream>
#include <string>

using namespace std;
class Personagem
{
public:
    Personagem(string nome, int habilidade, int energia, int sorte);

    virtual ~Personagem();

    // Métodos
    virtual void atacar(Personagem* alvo) = 0;

    // Getters
    string getNome();
    int getHabilidade();
    int getEnergia();
    int getEnergiaMaxima();
    int getSorte();

    // Setters
    void setNome(string nome);
    void setHabilidade(int habilidade);
    void setEnergia(int energia);
    void setEnergiaMaxima(int energiaMaxima);
    void setSorte(int sorte);

    void receberDano(int dano);
    void recuperarEnergia(int quantidade); // nunca passa da energia maxima
    bool estaVivo();
    bool testarSorte();
    void decrementarSorte(); 

protected:
    string nome;
    int habilidade; //Representa a destreza em combate. O valor influência diretamente nos resultados dos combates. 
    int energia; //Representa sua constituição, ou seja, pontos de vida.
    int energiaMaxima; // Limite da energia (a energia inicial, que cresce quando o jogador evolui).
    int sorte; // Como descrito sua sorte pode ser testada em determinados eventos e também pode mudar valores do dano recebido ou causado.
};

#endif // PERSONAGEM_H
