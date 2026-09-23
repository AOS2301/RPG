#ifndef INVENTARIO_H
#define INVENTARIO_H

#include <string>
#include <vector>
#include <fstream>
#include "../itens/Item.h"
#include "../itens/Arma.h"
#include "../itens/Armadura.h"
#include "../itens/ItemMagico.h"

using namespace std;

// Guarda tudo que o Jogador adquire durante a aventura: armas, armaduras,
// itens magicos, itens comuns, ouro e provisoes. Tambem sabe salvar e
// carregar os proprios dados (chamado de dentro de Jogo::salvarJogo e
// Jogo::carregarJogo, no mesmo arquivo de save do personagem).
//
// So guarda e organiza os dados: nao imprime nada na tela.
class Inventario
{
public:
    Inventario();
    ~Inventario();

    // Recebe uma linha de item de cena ("nome;tipo;combate;FA;dano"), cria o
    // item certo e guarda na lista certa. Se for arma/armadura melhor que a
    // equipada, ja equipa. Retorna o item criado (nullptr se a linha for invalida).
    Item *adicionarItem(string linha);

    // Armas
    int getQuantidadeArmas();
    Arma *getArma(int indice);
    Arma *getArmaEquipada();
    bool equiparArma(int indice); // false se o indice nao existir

    // Armaduras
    int getQuantidadeArmaduras();
    Armadura *getArmadura(int indice);
    Armadura *getArmaduraEquipada();
    bool equiparArmadura(int indice); // false se o indice nao existir

    // Itens magicos (usados na opcao "Usar Magia" da batalha)
    int getQuantidadeItensMagicos();
    ItemMagico *getItemMagico(int indice);
    ItemMagico *removerItemMagico(int indice); // tira da lista; quem chama faz o delete

    // Itens comuns (sem efeito em combate, ex: chaves)
    int getQuantidadeItensComuns();
    Item *getItemComum(int indice);

    // Ouro e provisoes
    void adicionarOuro(int quantidade);
    int getOuro();
    void adicionarProvisoes(int quantidade);
    int getProvisoes();
    bool consumirProvisao(); // false se nao houver provisao

    // Persistencia
    void salvar(ofstream &arquivo);
    bool carregar(ifstream &arquivo); // false se os dados estiverem corrompidos

private:
    Item *guardarItem(string linha); // cria e guarda o item, sem equipar
    void liberarItens();             // delete em todos os itens e limpa as listas

    vector<Arma *> armas;
    vector<Armadura *> armaduras;
    vector<ItemMagico *> itensMagicos;
    vector<Item *> itensComuns;
    Arma *armaEquipada;
    Armadura *armaduraEquipada;
    int ouro;
    int provisoes;
};

#endif // INVENTARIO_H
