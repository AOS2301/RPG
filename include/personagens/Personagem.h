#ifndef PERSONAGEM_H
#define PERSONAGEM_H

#include <string>

using namespace std;

// Classe base (abstrata) de todos os personagens do jogo: Jogador e Monstro.
// Guarda os atributos do enunciado e as regras que valem para qualquer um.
// Nao imprime nada na tela: quem conversa com o usuario eh o Jogo.
class Personagem
{
public:
    static constexpr int DANO_BASE = 2;   // quem vence a rodada tira 2 de energia (enunciado)
    static constexpr int DANO_MINIMO = 1; // um golpe que acerta sempre fere, mesmo contra armadura

    Personagem(string nome, int habilidade, int energia, int sorte);
    virtual ~Personagem();

    // ---- Combate ----
    // Cada tipo de personagem calcula sua Forca de Ataque e seu dano do seu
    // jeito (o Jogador soma a arma, o Monstro nao). A rodada chama esses
    // metodos atraves de Personagem*, entao cada lado usa a sua versao.
    virtual int calcularForcaAtaque() = 0; // 1d10 + habilidade (+ bonus)
    virtual int calcularDano() = 0;        // dano causado ao vencer a rodada

    // Protecao contra o oponente (armadura). Por padrao nenhuma.
    virtual int getProtecaoFA();   // quanto reduz a FA do oponente
    virtual int getProtecaoDano(); // quanto reduz o dano recebido

    // Resolve uma rodada de combate (regra do enunciado): os dois calculam
    // a FA, quem tiver a maior tira dano do outro e no empate ninguem acerta.
    // Retorna 1 se este personagem venceu, -1 se o oponente venceu, 0 se empatou.
    int disputarRodada(Personagem *oponente);

    // ---- Sorte ----
    // Em qualquer teste a Sorte eh gasta em 1 ponto, com sucesso ou nao (enunciado).
    bool testarSorte();                                 // combate: sorteia 1d10 e compara com a Sorte atual
    bool testarSorteContra(int dificuldade, int &dado); // cenas: passa se 1d10 + dificuldade <= Sorte (nao gasta Sorte)

    // ---- Energia ----
    void receberDano(int dano);
    void recuperarEnergia(int quantidade); // nunca passa da energia maxima
    bool estaVivo();

    // Getters
    string getNome();
    int getHabilidade();
    int getEnergia();
    int getEnergiaMaxima();
    int getSorte();

    // Setters
    void setNome(string nome);
    void setHabilidade(int habilidade);
    void setEnergia(int energia);
    void setEnergiaMaxima(int energiaMaxima);
    void setSorte(int sorte);

protected:
    int rolarDado(int lados); // numero aleatorio de 1 ate "lados"
    void gastarSorte();       // tira 1 ponto de Sorte (nunca fica negativa)

    string nome;
    int habilidade;    // destreza em combate: influencia diretamente os combates
    int energia;       // constituicao, ou seja, pontos de vida
    int energiaMaxima; // limite da energia (cresce quando o jogador evolui)
    int sorte;         // testada em eventos e em combate, muda o dano causado ou recebido
};

#endif // PERSONAGEM_H
