#include <iostream>
#include "../../include/inventario/Inventario.h"
#include "../../include/itens/CriarItem.h"

using namespace std;

Inventario::Inventario()
{
    armaEquipada = nullptr;
    armaduraEquipada = nullptr;
    ouro = 0;
    provisoes = 0;
}

// Apaga cada item. O vector em si se apaga sozinho, mas ele so guarda
// os enderecos: os itens criados com new precisam do delete.
Inventario::~Inventario()
{
    for (int i = 0; i < getQuantidade(); i++)
    {
        delete itens[i];
    }
}

void Inventario::adicionarItem(Item *item)
{
    itens.push_back(item);

    // Primeira arma/armadura recebida ja fica equipada
    if (item->getTipo() == 'w' && armaEquipada == nullptr)
    {
        armaEquipada = item;
    }
    else if (item->getTipo() == 'r' && armaduraEquipada == nullptr)
    {
        armaduraEquipada = item;
    }
}

int Inventario::getQuantidade()
{
    return (int)itens.size(); // size() devolve um numero sem sinal; convertemos para int
}

Item *Inventario::getItem(int indice)
{
    if (indice < 0 || indice >= getQuantidade())
    {
        return nullptr;
    }
    return itens[indice];
}

bool Inventario::equipar(int indice)
{
    Item *item = getItem(indice);
    if (item == nullptr)
    {
        return false;
    }

    if (item->getTipo() == 'w')
    {
        armaEquipada = item;
        return true;
    }
    if (item->getTipo() == 'r')
    {
        armaduraEquipada = item;
        return true;
    }
    return false; // item comum nao se equipa
}

void Inventario::removerItem(int indice)
{
    Item *item = getItem(indice);
    if (item == nullptr)
    {
        return;
    }

    // Se o item removido estava equipado, fica sem nada equipado
    if (item == armaEquipada)
    {
        armaEquipada = nullptr;
    }
    if (item == armaduraEquipada)
    {
        armaduraEquipada = nullptr;
    }

    delete item;

    // Tira o endereco da lista: begin() e a posicao 0, + indice anda ate o item.
    // O erase ja puxa os seguintes uma posicao para tras.
    itens.erase(itens.begin() + indice);
}

Item *Inventario::getArmaEquipada()
{
    return armaEquipada;
}

Item *Inventario::getArmaduraEquipada()
{
    return armaduraEquipada;
}

void Inventario::adicionarOuro(int quantidade)
{
    ouro = ouro + quantidade;
}

int Inventario::getOuro()
{
    return ouro;
}

void Inventario::adicionarProvisoes(int quantidade)
{
    provisoes = provisoes + quantidade;
}

int Inventario::getProvisoes()
{
    return provisoes;
}

bool Inventario::gastarProvisao()
{
    if (provisoes <= 0)
    {
        return false;
    }
    provisoes--;
    return true;
}

void Inventario::mostrar()
{
    cout << "Itens:" << endl;
    if (getQuantidade() == 0)
    {
        cout << "  (nenhum)" << endl;
    }
    for (int i = 0; i < getQuantidade(); i++)
    {
        cout << "  " << i + 1 << " - ";
        itens[i]->mostrar(); // polimorfismo: cada item se mostra do seu jeito
        if (itens[i] == armaEquipada || itens[i] == armaduraEquipada)
        {
            cout << "      ^ equipado" << endl;
        }
    }
    cout << "Ouro: " << ouro << " | Provisoes: " << provisoes << endl;
}

// Formato (um valor por linha):
//   ouro
//   provisoes
//   quantidade de itens
//   um item por linha, no formato das cenas: nome;tipo;combate;FA;dano
//   posicao da arma equipada (-1 se nenhuma)
//   posicao da armadura equipada (-1 se nenhuma)
void Inventario::salvar(ofstream &arquivo)
{
    arquivo << ouro << endl;
    arquivo << provisoes << endl;
    arquivo << getQuantidade() << endl;

    int posicaoArma = -1;
    int posicaoArmadura = -1;

    for (int i = 0; i < getQuantidade(); i++)
    {
        Item *item = itens[i];
        arquivo << item->getNome() << ";" << item->getTipo() << ";" << item->getCombate()
                << ";" << item->getFA() << ";" << item->getDano() << endl;

        if (item == armaEquipada)
        {
            posicaoArma = i;
        }
        if (item == armaduraEquipada)
        {
            posicaoArmadura = i;
        }
    }

    arquivo << posicaoArma << endl;
    arquivo << posicaoArmadura << endl;
}

// Le na mesma ordem do salvar. Cada linha e lida com getline e,
// quando e numero, convertida com stoi.
void Inventario::carregar(ifstream &arquivo)
{
    string linha;

    getline(arquivo, linha);
    ouro = stoi(linha);
    getline(arquivo, linha);
    provisoes = stoi(linha);

    getline(arquivo, linha);
    int quantidade = stoi(linha);
    for (int i = 0; i < quantidade; i++)
    {
        getline(arquivo, linha);
        adicionarItem(criarItem(linha));
    }

    getline(arquivo, linha);
    int posicaoArma = stoi(linha);
    getline(arquivo, linha);
    int posicaoArmadura = stoi(linha);

    if (posicaoArma >= 0)
    {
        equipar(posicaoArma);
    }
    if (posicaoArmadura >= 0)
    {
        equipar(posicaoArmadura);
    }
}
