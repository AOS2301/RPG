#include "../../include/itens/CriarItem.h"
#include "../../include/itens/Arma.h"
#include "../../include/itens/Armadura.h"
#include "../../include/itens/ItemComum.h"

// Devolve o texto ate o primeiro ';' e tira esse pedaco da linha.
// O & faz a funcao alterar a propria string de quem chamou.
// Ex: linha = "Adaga;w;1;1;1" -> devolve "Adaga" e linha vira "w;1;1;1"
string tirarCampo(string &linha)
{
    string campo;

    // string::npos quer dizer "nao encontrou"
    if (linha.find(';') == string::npos)
    {
        campo = linha; // ultimo campo: nao tem mais ';'
        linha = "";
    }
    else
    {
        int posicao = linha.find(';');       // posicao do ';'
        campo = linha.substr(0, posicao);    // do inicio ate antes do ';'
        linha = linha.substr(posicao + 1);   // do depois do ';' ate o fim
    }
    return campo;
}

Item *criarItem(string linha)
{
    string nome = tirarCampo(linha);
    char tipo = tirarCampo(linha)[0];            // primeira letra: w, r ou c
    bool combate = (tirarCampo(linha) == "1");   // "1" = pode usar em combate
    int fa = stoi(tirarCampo(linha));            // stoi: converte texto em int
    int dano = stoi(tirarCampo(linha));

    if (tipo == 'w')
    {
        return new Arma(nome, combate, fa, dano);
    }
    if (tipo == 'r')
    {
        return new Armadura(nome, combate, fa, dano);
    }
    return new ItemComum(nome, combate, fa, dano);
}
