#ifndef INVENTARIO_H
#define INVENTARIO_H

#include <vector>
#include <fstream>
#include "../itens/Item.h"

// Guarda tudo que o jogador carrega: itens (armas, armaduras e itens comuns),
// o que esta equipado, ouro (tesouro) e provisoes.
// O inventario e DONO dos itens: quando ele e destruido, apaga todos.
class Inventario
{
private:
    vector<Item *> itens;

    Item *armaEquipada;     // aponta para uma arma do vetor (ou nullptr)
    Item *armaduraEquipada; // aponta para uma armadura do vetor (ou nullptr)

    int ouro;      // tesouro (T das cenas de monstro)
    int provisoes; // cada uma recupera 4 de energia

public:
    Inventario();
    ~Inventario();

    // Guarda o item (o inventario passa a ser o dono dele).
    // Se for arma ou armadura e nao houver nenhuma equipada, ja equipa.
    void adicionarItem(Item *item);

    int getQuantidade();
    Item *getItem(int indice); // nullptr se o indice nao existir

    // Equipa a arma ou armadura da posicao "indice".
    // Retorna false se o indice nao existir ou se o item for comum.
    bool equipar(int indice);

    // Apaga o item da posicao "indice" (ex: pergaminho usado em combate)
    // e puxa os seguintes uma posicao para tras.
    void removerItem(int indice);

    Item *getArmaEquipada();
    Item *getArmaduraEquipada();

    void adicionarOuro(int quantidade);
    int getOuro();

    void adicionarProvisoes(int quantidade);
    int getProvisoes();
    bool gastarProvisao(); // false se nao houver provisao

    void mostrar(); // lista itens, equipados, ouro e provisoes

    // Grava/le a parte do inventario no arquivo de save que o Jogo ja abriu.
    // O & passa o proprio arquivo (um arquivo nao pode ser copiado).
    void salvar(ofstream &arquivo);
    void carregar(ifstream &arquivo);
};

#endif
