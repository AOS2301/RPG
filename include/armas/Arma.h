#ifndef ARMA_H
#define ARMA_H
#include <iostream>
#include <string>

using namespace std;

class Arma
{
public:
    Arma(string nome, bool combate, int fa, int dano);

    // Metodos
    string imprimeInfo();

    // Getters
    string getNome();
    bool getCombate();
    int getFA();
    int getDano();

    // Setters
    void setNome(string nome);
    void setCombate(bool combate);
    void setFA(int fa);
    void setDano(int dano);

protected:
    string nome;
    bool combate; // se pode ser usada em combate (campo "combate" do formato de item)
    int fa;       // bonus de Forca de Ataque (campo "FA")
    int dano;     // bonus de dano (campo "dano")
};

#endif // ARMA_H