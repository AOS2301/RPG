#include "../../include/inventario/Inventario.h"
#include "../../include/util/Texto.h"

Inventario::Inventario()
{
    armaEquipada = nullptr;
    armaduraEquipada = nullptr;
    ouro = 0;
    provisoes = 0;
}

Inventario::~Inventario()
{
    liberarItens();
}

void Inventario::liberarItens()
{
    for (size_t i = 0; i < armas.size(); i++)
    {
        delete armas[i];
    }
    for (size_t i = 0; i < armaduras.size(); i++)
    {
        delete armaduras[i];
    }
    for (size_t i = 0; i < itensMagicos.size(); i++)
    {
        delete itensMagicos[i];
    }
    for (size_t i = 0; i < itensComuns.size(); i++)
    {
        delete itensComuns[i];
    }
    armas.clear();
    armaduras.clear();
    itensMagicos.clear();
    itensComuns.clear();
    armaEquipada = nullptr;
    armaduraEquipada = nullptr;
}

// Le "nome;tipo;combate;FA;dano", cria o objeto do tipo certo e guarda na
// lista correspondente. Usado tanto para itens de cena quanto no carregar.
Item *Inventario::guardarItem(string linha)
{
    vector<string> campos = dividir(linha, ';');
    if (campos.size() != 5)
    {
        return nullptr; // linha mal formatada
    }

    string nome = aparar(campos[0]);
    string tipo = aparar(campos[1]);
    bool combate = (aparar(campos[2]) == "1");
    int fa;
    int dano;
    try
    {
        fa = stoi(campos[3]);
        dano = stoi(campos[4]);
    }
    catch (...)
    {
        return nullptr; // FA ou dano nao eram numeros
    }

    if (tipo == "w")
    {
        Arma *arma = new Arma(nome, combate, fa, dano);
        armas.push_back(arma);
        return arma;
    }
    if (tipo == "r")
    {
        Armadura *armadura = new Armadura(nome, combate, fa, dano);
        armaduras.push_back(armadura);
        return armadura;
    }
    if (tipo == "c" && combate)
    {
        ItemMagico *magico = new ItemMagico(nome, fa, dano);
        itensMagicos.push_back(magico);
        return magico;
    }

    Item *comum = new Item(nome, 'c', combate, fa, dano);
    itensComuns.push_back(comum);
    return comum;
}

Item *Inventario::adicionarItem(string linha)
{
    Item *item = guardarItem(linha);
    if (item == nullptr)
    {
        return nullptr;
    }

    // Equipa automaticamente se ainda nao ha nada equipado ou se o item
    // novo for melhor (soma de FA + dano maior). O jogador pode trocar
    // depois pela ficha do personagem.
    if (item->getTipo() == 'w')
    {
        Arma *nova = armas.back();
        if (armaEquipada == nullptr ||
            nova->getFA() + nova->getDano() > armaEquipada->getFA() + armaEquipada->getDano())
        {
            armaEquipada = nova;
        }
    }
    else if (item->getTipo() == 'r')
    {
        Armadura *nova = armaduras.back();
        if (armaduraEquipada == nullptr ||
            nova->getFA() + nova->getDano() > armaduraEquipada->getFA() + armaduraEquipada->getDano())
        {
            armaduraEquipada = nova;
        }
    }

    return item;
}

// ---- Armas ----

int Inventario::getQuantidadeArmas()
{
    return static_cast<int>(armas.size());
}

Arma *Inventario::getArma(int indice)
{
    return armas.at(indice);
}

Arma *Inventario::getArmaEquipada()
{
    return armaEquipada;
}

bool Inventario::equiparArma(int indice)
{
    if (indice < 0 || indice >= static_cast<int>(armas.size()))
    {
        return false;
    }
    armaEquipada = armas[indice];
    return true;
}

// ---- Armaduras ----

int Inventario::getQuantidadeArmaduras()
{
    return static_cast<int>(armaduras.size());
}

Armadura *Inventario::getArmadura(int indice)
{
    return armaduras.at(indice);
}

Armadura *Inventario::getArmaduraEquipada()
{
    return armaduraEquipada;
}

bool Inventario::equiparArmadura(int indice)
{
    if (indice < 0 || indice >= static_cast<int>(armaduras.size()))
    {
        return false;
    }
    armaduraEquipada = armaduras[indice];
    return true;
}

// ---- Itens magicos ----

int Inventario::getQuantidadeItensMagicos()
{
    return static_cast<int>(itensMagicos.size());
}

ItemMagico *Inventario::getItemMagico(int indice)
{
    return itensMagicos.at(indice);
}

ItemMagico *Inventario::removerItemMagico(int indice)
{
    if (indice < 0 || indice >= static_cast<int>(itensMagicos.size()))
    {
        return nullptr;
    }
    ItemMagico *item = itensMagicos[indice];
    itensMagicos.erase(itensMagicos.begin() + indice);
    return item;
}

// ---- Itens comuns ----

int Inventario::getQuantidadeItensComuns()
{
    return static_cast<int>(itensComuns.size());
}

Item *Inventario::getItemComum(int indice)
{
    return itensComuns.at(indice);
}

// ---- Ouro e provisoes ----

void Inventario::adicionarOuro(int quantidade)
{
    if (quantidade > 0)
    {
        ouro += quantidade;
    }
}

int Inventario::getOuro()
{
    return ouro;
}

void Inventario::adicionarProvisoes(int quantidade)
{
    if (quantidade > 0)
    {
        provisoes += quantidade;
    }
}

int Inventario::getProvisoes()
{
    return provisoes;
}

bool Inventario::consumirProvisao()
{
    if (provisoes <= 0)
    {
        return false;
    }
    provisoes--;
    return true;
}

// ------------------------------------------------------------------
// Persistencia: parte do arquivo de save que pertence ao Inventario.
// Os itens sao gravados no MESMO formato das cenas, entao o carregar
// reaproveita o guardarItem. Formato:
//
//   ouro
//   provisoes
//   quantidade de itens
//   uma linha por item -> nome;tipo;combate;FA;dano
//   indice da arma equipada dentro das armas (-1 se nenhuma)
//   indice da armadura equipada dentro das armaduras (-1 se nenhuma)
// ------------------------------------------------------------------

void Inventario::salvar(ofstream &arquivo)
{
    arquivo << ouro << "\n";
    arquivo << provisoes << "\n";

    int quantidade = static_cast<int>(armas.size() + armaduras.size() + itensMagicos.size() + itensComuns.size());
    arquivo << quantidade << "\n";

    // Armas e armaduras sao gravadas na ordem das listas, entao os indices
    // de equipados continuam validos quando forem lidos de volta.
    int indiceArma = -1;
    for (size_t i = 0; i < armas.size(); i++)
    {
        arquivo << armas[i]->paraLinha() << "\n";
        if (armas[i] == armaEquipada)
        {
            indiceArma = static_cast<int>(i);
        }
    }

    int indiceArmadura = -1;
    for (size_t i = 0; i < armaduras.size(); i++)
    {
        arquivo << armaduras[i]->paraLinha() << "\n";
        if (armaduras[i] == armaduraEquipada)
        {
            indiceArmadura = static_cast<int>(i);
        }
    }

    for (size_t i = 0; i < itensMagicos.size(); i++)
    {
        arquivo << itensMagicos[i]->paraLinha() << "\n";
    }
    for (size_t i = 0; i < itensComuns.size(); i++)
    {
        arquivo << itensComuns[i]->paraLinha() << "\n";
    }

    arquivo << indiceArma << "\n";
    arquivo << indiceArmadura << "\n";
}

bool Inventario::carregar(ifstream &arquivo)
{
    liberarItens();

    int quantidade;
    if (!lerLinhaInteira(arquivo, ouro) ||
        !lerLinhaInteira(arquivo, provisoes) ||
        !lerLinhaInteira(arquivo, quantidade))
    {
        return false;
    }

    for (int i = 0; i < quantidade; i++)
    {
        string linha;
        if (!getline(arquivo, linha) || guardarItem(linha) == nullptr)
        {
            return false;
        }
    }

    int indiceArma;
    int indiceArmadura;
    if (!lerLinhaInteira(arquivo, indiceArma) ||
        !lerLinhaInteira(arquivo, indiceArmadura))
    {
        return false;
    }

    // Indice -1 (ou invalido) simplesmente deixa nada equipado.
    equiparArma(indiceArma);
    equiparArmadura(indiceArmadura);
    return true;
}
