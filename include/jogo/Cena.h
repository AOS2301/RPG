#ifndef CENA_H
#define CENA_H

#include <string>
#include <vector>
#include "../personagens/Monstro.h"

using namespace std;

// Le um arquivo de cena (cenas/N.txt) e guarda o conteudo.
// A primeira linha diz o tipo da cena:
//   "#N" -> narrativa: texto, itens (I:) e opcoes (#destino: texto)
//   "m"  -> monstro: texto, dados do monstro (N, M, H, S, E, T, P, I) e "vitoria;derrota"
//   "s"  -> teste de sorte: texto, dano se falhar (X) e "sucesso;falha"
// A Cena so guarda os dados: quem mostra na tela e decide e o Jogo.
class Cena
{
private:
    char tipo;    // 'n' narrativa, 'm' monstro, 's' teste de sorte
    string texto; // texto da historia

    vector<string> itens;         // linhas "nome;tipo;combate;FA;dano"
    vector<string> textosOpcoes;  // texto de cada escolha
    vector<int> destinosOpcoes;   // numero da cena de cada escolha

    // Monstro (so na cena 'm')
    string nomeMonstro;
    bool podeFugir;
    int habilidade;
    int sorte;
    int energia;
    int tesouro;
    int provisoes;

    // Cenas 'm' e 's': para onde ir se der certo ou errado
    int destinoSucesso;
    int destinoFalha;
    int danoFalha; // X: energia perdida se falhar no teste de sorte

    string semEspacos(string texto); // tira espacos do inicio do texto

public:
    Cena();

    // Le cenas/<numero>.txt. Retorna false se o arquivo nao abrir.
    bool carregar(int numero);

    char getTipo();
    string getTexto();

    int getQuantidadeItens();
    string getItem(int indice);

    int getQuantidadeOpcoes(); // 0 = final da historia
    string getTextoOpcao(int indice);
    int getDestinoOpcao(int indice);

    // Cria o monstro da cena com new (quem chama faz o delete).
    Monstro *criarMonstro();

    int getDestinoSucesso();
    int getDestinoFalha();
    int getDanoFalha();
};

#endif
