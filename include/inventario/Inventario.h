#ifndef INVENTARIO_H
#define INVENTARIO_H

#include <string>
#include <vector>
#include <fstream>
#include "../armas/Arma.h"

using namespace std;

// Guarda tudo que o Jogador vai adquirindo durante a aventura: armas,
// ouro, provisoes e os demais itens (armaduras e itens comuns). Tambem
// sabe salvar e carregar os proprios dados num arquivo (chamado de
// dentro de Jogo::salvarJogo/carregarJogo, no mesmo arquivo de save
// do personagem).
class Inventario
{
public:
    Inventario();
    ~Inventario();
    void adicionarItem(string linha);

    void equiparArma(Arma *arma);
    Arma *getArmaEquipada();
    int getQuantidadeArmas();
    Arma *getArma(int indice);

    int getQuantidadeItensMagicos();
    Arma *getItemMagico(int indice);
    Arma *consumirItemMagico(int indice); 

    int getQuantidadeOutrosItens();
    string getOutroItem(int indice);

    void adicionarOuro(int quantidade);
    int getOuro();

    void adicionarProvisoes(int quantidade);
    int getProvisoes();
    bool consumirProvisao();

    void salvar(ofstream &arquivo);
    bool carregar(ifstream &arquivo);

private:
    vector<Arma *> armas;
    Arma *armaEquipada;
    vector<Arma *> itensMagicos;
    vector<string> outrosItens;
    int ouro;
    int provisoes;
};

#endif // INVENTARIO_H
