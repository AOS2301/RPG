#ifndef ITEM_H
#define ITEM_H

#include <string>
using namespace std;

// Superclasse de todos os itens do jogo.
// Guarda os 5 campos do formato do enunciado: nome;tipo;combate;FA;dano
// E abstrata: nao existe "um Item qualquer", so Arma, Armadura ou ItemComum.
class Item
{
protected:
    string nome;  // ex: "Espada larga"
    char tipo;    // 'w' = arma, 'r' = armadura, 'c' = item comum
    bool combate; // true (1) se pode ser usado em combate
    int fa;       // bonus na Forca de Ataque
    int dano;     // bonus no dano

public:
    Item(string nome, char tipo, bool combate, int fa, int dano);
    virtual ~Item(); // virtual para o delete de um Item* chamar o destrutor certo

    string getNome();
    char getTipo();
    bool getCombate();
    int getFA();
    int getDano();

    // Cada tipo de item se mostra de um jeito (polimorfismo).
    // "= 0" torna o metodo virtual puro: as subclasses sao obrigadas a implementar.
    virtual void mostrar() = 0;
};

#endif
