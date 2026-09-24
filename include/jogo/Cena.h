#ifndef CENA_H
#define CENA_H

#include <string>
#include <vector>

using namespace std;

// Le um arquivo de cena (cenes/N.txt) e guarda o conteudo em memoria.
// Serve para os tres tipos de cena: narrativa, monstro e teste de sorte.
class Cena
{
public:
    Cena();

    // Retorna false se o arquivo nao puder ser aberto.
    bool carregar(string caminho);

    bool ehMonstro();
    bool ehTesteDeSorte();
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

    // Dados do teste de sorte (so fazem sentido se ehTesteDeSorte() for true).
    // Uso da Sorte fora de combate, conforme o enunciado: sorteia-se um valor
    // (1d10) e compara com a Sorte. A dificuldade da cena (D) soma no dado:
    // passa se 1d10 + D <= Sorte. Se falhar, sofre o dano da cena (X).
    // Esse teste nao gasta Sorte (so o de combate gasta).
    int getDificuldadeSorte();
    int getDanoFalhaSorte();

    void limpar();

private:
    bool monstro;
    bool testeSorte;
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

    int dificuldadeSorte;
    int danoFalhaSorte;
};

#endif // CENA_H
