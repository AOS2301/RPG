#include "../../include/jogo/Cena.h"
#include "../../include/util/Texto.h"
#include <fstream>
#include <cctype>

Cena::Cena()
{
    limpar();
}

void Cena::limpar()
{
    monstro = false;
    testeSorte = false;
    texto = "";
    textosOpcoes.clear();
    destinosOpcoes.clear();
    itens.clear();
    nomeMonstro = "";
    habilidadeMonstro = 0;
    sorteMonstro = 0;
    energiaMonstro = 0;
    ouro = 0;
    provisoes = 0;
    destinoSucesso = 0;
    destinoDerrota = 0;
    dificuldadeSorte = 0;
    danoFalhaSorte = 0;
}

bool Cena::carregar(string caminho)
{
    limpar();

    ifstream arquivo;
    arquivo.open(caminho);

    if (arquivo.fail()){
        return false;
    }

    string linha;

    // 1a linha: "#N" (cena de narrativa), "m" (cena de monstro) ou
    // "s" (teste de sorte fora de combate, ex: atravessar uma ponte).
    getline(arquivo, linha);
    linha = aparar(linha);
    monstro = (!linha.empty() && linha[0] == 'm');
    testeSorte = (!linha.empty() && linha[0] == 's');

    // Demais linhas: cada uma eh texto, opcao, item ou dado do monstro.
    while (getline(arquivo, linha))
    {
        linha = aparar(linha);

        if (linha.substr(0, 2) == "I:")
        {
            itens.push_back(aparar(linha.substr(2)));
        }
        else if (!linha.empty() && linha[0] == '#')
        {
            // "#2: Investigar a capela" -> destino 2, texto "Investigar a capela"
            size_t doisPontos = linha.find(':');
            destinosOpcoes.push_back(stoi(linha.substr(1, doisPontos - 1)));
            textosOpcoes.push_back(aparar(linha.substr(doisPontos + 1)));
        }
        else if (monstro && linha.substr(0, 2) == "N:")
        {
            nomeMonstro = aparar(linha.substr(2));
        }
        else if (monstro && linha.substr(0, 2) == "H:")
        {
            habilidadeMonstro = stoi(linha.substr(2));
        }
        else if (monstro && linha.substr(0, 2) == "S:")
        {
            sorteMonstro = stoi(linha.substr(2));
        }
        else if (monstro && linha.substr(0, 2) == "E:")
        {
            energiaMonstro = stoi(linha.substr(2));
        }
        else if (monstro && linha.substr(0, 2) == "T:")
        {
            ouro = stoi(linha.substr(2));
        }
        else if (monstro && linha.substr(0, 2) == "P:")
        {
            provisoes = stoi(linha.substr(2));
        }
        else if (testeSorte && linha.substr(0, 2) == "D:")
        {
            // Dificuldade do teste: 1d6 + Sorte precisa ser MAIOR que esse valor.
            dificuldadeSorte = stoi(linha.substr(2));
        }
        else if (testeSorte && linha.substr(0, 2) == "X:")
        {
            // Dano sofrido se o jogador falhar no teste.
            danoFalhaSorte = stoi(linha.substr(2));
        }
        else if ((monstro || testeSorte) && !linha.empty() && isdigit(linha[0]))
        {
            // "12;13" -> cena se vencer/passar ; cena se perder/falhar
            // (mesma convencao usada nas cenas de monstro)
            size_t pontoEVirgula = linha.find(';');
            destinoSucesso = stoi(linha.substr(0, pontoEVirgula));
            destinoDerrota = stoi(linha.substr(pontoEVirgula + 1));
        }
        else
        {
            // Qualquer outra linha faz parte do texto da narrativa.
            texto += linha + "\n";
        }
    }

    arquivo.close();
    return true;
}

bool Cena::ehMonstro()
{
    return monstro;
}

bool Cena::ehTesteDeSorte()
{
    return testeSorte;
}

string Cena::getTexto()
{
    return texto;
}

int Cena::getQuantidadeOpcoes()
{
    return static_cast<int>(textosOpcoes.size());
}

string Cena::getTextoOpcao(int indice)
{
    return textosOpcoes.at(indice);
}

int Cena::getDestinoOpcao(int indice)
{
    return destinosOpcoes.at(indice);
}

int Cena::getQuantidadeItens()
{
    return static_cast<int>(itens.size());
}

string Cena::getItem(int indice)
{
    return itens.at(indice);
}

string Cena::getNomeMonstro()
{
    return nomeMonstro;
}

int Cena::getHabilidadeMonstro()
{
    return habilidadeMonstro;
}

int Cena::getSorteMonstro()
{
    return sorteMonstro;
}

int Cena::getEnergiaMonstro()
{
    return energiaMonstro;
}

int Cena::getOuro()
{
    return ouro;
}

int Cena::getProvisoes()
{
    return provisoes;
}

int Cena::getDestinoSucesso()
{
    return destinoSucesso;
}

int Cena::getDestinoDerrota()
{
    return destinoDerrota;
}

int Cena::getDificuldadeSorte()
{
    return dificuldadeSorte;
}

int Cena::getDanoFalhaSorte()
{
    return danoFalhaSorte;
}
