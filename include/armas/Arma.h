#ifndef ARMA_H
#define ARMA_H
#include <iostream>
#include <string>
class Arma
{
public:
     // Construtor padrão
    Arma();

    Arma(std::string nome, int dano) : nome(nome), dano(dano) {}
    virtual ~Arma() = default;

    virtual int calcularDano() const = 0; // cada arma calcula o dano do seu jeito

    // Métodos
    std::string imprimeInfo();

    // Getters
    string getNome();
    int getDano();

    // Setters
    void setNome(std::string nome);
    void setDano(int dano);

protected:
    string nome;
    int dano;
};

#endif // ARMA_H