#ifndef MONSTRO_H
#define MONSTRO_H

#include "Personagem.h"
#include "../itens/Item.h"

// Inimigo lido de uma cena de monstro. Alem dos atributos de Personagem,
// guarda o que entrega ao ser derrotado (T, P e I no arquivo da cena)
// e se o jogador pode fugir dele (M no arquivo: S ou N).
class Monstro : public Personagem
{
private:
    int tesouro;    // moedas de ouro (0 se a cena nao tem T)
    int provisoes;  // provisoes (0 se a cena nao tem P)
    Item *item;     // item que entrega (nullptr se a cena nao tem I)
    bool podeFugir; // false: o jogador tenta fugir mas nao consegue

public:
    Monstro(string nome, int habilidade, int energia, int sorte,
            int tesouro, int provisoes, Item *item, bool podeFugir);
    ~Monstro();

    int forcaAtaque(); // 1 a 10 + habilidade (sem bonus)

    int getTesouro();
    int getProvisoes();
    bool getPodeFugir();

    // Passa o item para quem chamou (o jogador) e fica sem ele.
    // Assim o destrutor do monstro nao apaga um item que agora e do jogador.
    Item *entregarItem();
};

#endif
