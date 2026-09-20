#ifndef CENA_H
#define CENA_H

#include <string>
#include <vector>

using namespace std;

// Le um arquivo de cena (cenes/N.txt) e guarda o conteudo em memoria.
// Serve tanto para cenas de narrativa quanto para cenas de monstro.
class Cena
{
public:
    Cena();

    // Retorna false se o arquivo nao puder ser aberto.
    bool carregar(string caminho);

    bool ehMonstro();
    string getTexto();

    // Opcoes de escolha (linhas "#N: texto")
    int getQuantidadeOpcoes();
    string getTextoOpcao(int indice);
    int getDestinoOpcao(int indice);

    // Itens (linhas "I: nome;tipo;combate;FA;dano"), guardados como texto puro
    int getQuantidadeItens();
    string getItem(int indice);

    // Dados do monstro (so fazem sentido se ehMonstro() for true)
    string getNomeMonstro();
    int getHabilidadeMonstro();
    int getSorteMonstro();
    int getEnergiaMonstro();
    int getOuro();
    int getProvisoes();
    int getDestinoSucesso();
    int getDestinoDerrota();

private:
    bool monstro;
    string texto;
    vector<string> textosOpcoes;
    vector<int> destinosOpcoes;
    vector<string> itens;

    string nomeMonstro;
    int habilidadeMonstro;
    int sorteMonstro;
    int energiaMonstro;
    int ouro;
    int provisoes;
    int destinoSucesso;
    int destinoDerrota;

    void limpar();
};

#endif // CENA_H
