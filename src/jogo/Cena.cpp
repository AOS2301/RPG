#include <fstream>
#include "../../include/jogo/Cena.h"
#include "../../include/itens/CriarItem.h"

Cena::Cena()
{
    tipo = 'n';
    texto = "";
    nomeMonstro = "";
    podeFugir = true;
    habilidade = 0;
    sorte = 0;
    energia = 0;
    tesouro = 0;   // se a cena nao tiver T, o monstro nao da tesouro
    provisoes = 0; // se a cena nao tiver P, o monstro nao da provisoes
    destinoSucesso = 0;
    destinoFalha = 0;
    danoFalha = 0;
}

string Cena::semEspacos(string texto)
{
    while (texto.size() > 0 && texto[0] == ' ')
    {
        texto = texto.substr(1);
    }
    return texto;
}

bool Cena::carregar(int numero)
{
    ifstream arquivo;
    arquivo.open("cenas/" + to_string(numero) + ".txt"); // to_string: int -> texto
    if (!arquivo.is_open())
    {
        return false;
    }

    string linha;

    // Primeira linha: tipo da cena
    getline(arquivo, linha);
    if (linha.size() > 0 && linha[0] == 'm')
    {
        tipo = 'm';
    }
    else if (linha.size() > 0 && linha[0] == 's')
    {
        tipo = 's';
    }
    else
    {
        tipo = 'n';
    }

    // Demais linhas
    while (getline(arquivo, linha))
    {
        // Arquivo salvo no Windows termina a linha com '\r': tiramos para
        // o jogo funcionar igual no Linux e no Windows.
        if (linha.size() > 0 && linha[linha.size() - 1] == '\r')
        {
            linha = linha.substr(0, linha.size() - 1);
        }

        string inicio = linha.substr(0, 2); // 2 primeiras letras, ex: "I:"

        if (inicio == "I:")
        {
            itens.push_back(semEspacos(linha.substr(2)));
        }
        else if (linha.size() > 1 && linha[0] == '#' && linha.find(':') != string::npos)
        {
            // Opcao: "#2: Investigar a capela" -> destino 2, texto "Investigar a capela"
            int doisPontos = linha.find(':');
            destinosOpcoes.push_back(stoi(linha.substr(1, doisPontos - 1)));
            textosOpcoes.push_back(semEspacos(linha.substr(doisPontos + 1)));
        }
        else if (tipo == 'm' && inicio == "N:")
        {
            nomeMonstro = semEspacos(linha.substr(2));
        }
        else if (tipo == 'm' && inicio == "M:")
        {
            podeFugir = (semEspacos(linha.substr(2)) != "N"); // "N" = nao pode fugir
        }
        else if (tipo == 'm' && inicio == "H:")
        {
            habilidade = stoi(linha.substr(2));
        }
        else if (tipo == 'm' && inicio == "S:")
        {
            sorte = stoi(linha.substr(2));
        }
        else if (tipo == 'm' && inicio == "E:")
        {
            energia = stoi(linha.substr(2));
        }
        else if (tipo == 'm' && inicio == "T:")
        {
            tesouro = stoi(linha.substr(2));
        }
        else if ((tipo == 'm' && inicio == "P:") || (tipo == 'n' && inicio == "P:"))
        {
            provisoes = stoi(linha.substr(2));
        }
        else if (tipo == 's' && inicio == "X:")
        {
            danoFalha = stoi(linha.substr(2));
        }
        else if (tipo != 'n' && linha.size() > 0 && linha[0] >= '0' && linha[0] <= '9' && linha.find(';') != string::npos)
        {
            // Linha "12;13": sucesso;falha
            int pontoVirgula = linha.find(';');
            destinoSucesso = stoi(linha.substr(0, pontoVirgula));
            destinoFalha = stoi(linha.substr(pontoVirgula + 1));
        }
        else
        {
            texto = texto + linha + "\n"; // qualquer outra linha faz parte do texto
        }
    }

    arquivo.close();
    return true;
}

char Cena::getTipo()
{
    return tipo;
}

string Cena::getTexto()
{
    return texto;
}

int Cena::getQuantidadeItens()
{
    return (int)itens.size();
}

string Cena::getItem(int indice)
{
    return itens[indice];
}

int Cena::getQuantidadeOpcoes()
{
    return (int)textosOpcoes.size();
}

string Cena::getTextoOpcao(int indice)
{
    return textosOpcoes[indice];
}

int Cena::getDestinoOpcao(int indice)
{
    return destinosOpcoes[indice];
}

Monstro *Cena::criarMonstro()
{
    // O monstro carrega o primeiro item da cena (se houver), para entregar ao morrer
    Item *item = nullptr;
    if (itens.size() > 0)
    {
        item = criarItem(itens[0]);
    }
    return new Monstro(nomeMonstro, habilidade, energia, sorte, tesouro, provisoes, item, podeFugir);
}

int Cena::getDestinoSucesso()
{
    return destinoSucesso;
}

int Cena::getDestinoFalha()
{
    return destinoFalha;
}

int Cena::getDanoFalha()
{
    return danoFalha;
}

int Cena::getProvisoes()
{
    return provisoes;
}
