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
    ~Inventario(); // libera as armas guardadas

    // Recebe uma linha "nome;tipo;combate;FA;dano" (mesmo formato usado
    // nos arquivos de cena). Armas (tipo 'w') entram na lista de armas e
    // podem ser equipadas automaticamente; armaduras ('r') e itens
    // comuns ('c') entram na lista de outros itens.
    void adicionarItem(string linha);

    // ---- Armas ----
    void equiparArma(Arma* arma); // adiciona a lista (se necessario) e equipa
    Arma* getArmaEquipada();
    int getQuantidadeArmas();
    Arma* getArma(int indice);

    // ---- Outros itens (armaduras e itens comuns) ----
    int getQuantidadeOutrosItens();
    string getOutroItem(int indice);

    // ---- Ouro ----
    void adicionarOuro(int quantidade);
    int getOuro();

    // ---- Provisoes ----
    void adicionarProvisoes(int quantidade);
    int getProvisoes();
    bool consumirProvisao(); // decrementa 1 provisao; retorna false se nao havia

    // ---- Persistencia ----
    void salvar(ofstream& arquivo);
    bool carregar(ifstream& arquivo); // false = arquivo incompleto/corrompido

private:
    vector<Arma*> armas;
    Arma* armaEquipada;
    vector<string> outrosItens;
    int ouro;
    int provisoes;
};

#endif // INVENTARIO_H
