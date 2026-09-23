#ifndef ITEM_H
#define ITEM_H

#include <string>

using namespace std;

// Classe base de todos os itens do jogo. Guarda os 5 campos do formato
// de item do enunciado: nome;tipo;combate;FA;dano
//   tipo: 'w' arma, 'r' armadura, 'c' item comum
//   combate: se pode ser usado em combate
//
// Um item comum sem uso em combate (ex: uma chave) eh um Item "puro".
// Arma, Armadura e ItemMagico herdam daqui e mudam o que FA/dano significam.
class Item
{
public:
    Item(string nome, char tipo, bool combate, int fa, int dano);
    virtual ~Item();

    // Descricao para a ficha do personagem. Cada tipo de item descreve
    // seus bonus do seu jeito (polimorfismo).
    virtual string imprimeInfo();

    // Linha no mesmo formato das cenas ("nome;tipo;combate;FA;dano"),
    // usada para gravar o item no arquivo de save.
    string paraLinha();

    string getNome();
    char getTipo();
    bool getCombate();
    int getFA();
    int getDano();

protected:
    string nome;
    char tipo;
    bool combate;
    int fa;
    int dano;
};

#endif // ITEM_H
