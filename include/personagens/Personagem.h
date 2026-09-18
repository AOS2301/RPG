#ifndef PERSONAGEM_H
#define PERSONAGEM_H
#include <iostream>
#include <string>

class Personagem
{
public:
     // Construtor padrão
    Personagem();

    Personagem(std::string nome, int vida, int forca, int sorte)
        : nome(nome), vida(vida), forca(forca), sorte(sorte) {}

    virtual ~Personagem() = default;

    // Métodos
    virtual void atacar(Personagem& alvo) = 0;

    // Getters
    string getNome();
    int getVida();
    float getForca();
    int getSorte();

    // Setters
    void setNome(string nome);
    void setVida(int vida);
    void setForca(float forca);
    void setSorte(int sorte);

protected:
    string nome;
    int vida;
    int forca;
    int sorte;
};

#endif // PERSONAGEM_H