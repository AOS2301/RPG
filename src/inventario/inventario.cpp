#include "../../include/inventario/Inventario.h"

// Quebra "Adaga de prata;w;1;1;1" em {"Adaga de prata","w","1","1","1"}.
// Mesma logica de Jogo::dividir, mas com nome proprio: como main.cpp
// inclui os .cpp diretamente, duas funcoes "static" de mesmo nome em
// arquivos diferentes acabam colidindo na mesma unidade de compilacao.
static vector<string> dividirCampos(string texto, char separador) {
    vector<string> partes;
    string atual = "";

    for (size_t i = 0; i < texto.size(); i++) {
        if (texto[i] == separador) {
            partes.push_back(atual);
            atual = "";
        } else {
            atual += texto[i];
        }
    }
    partes.push_back(atual);
    return partes;
}

Inventario::Inventario() {
    armaEquipada = nullptr;
    ouro = 0;
    provisoes = 0;
}

Inventario::~Inventario() {
    for (size_t i = 0; i < armas.size(); i++) {
        delete armas[i];
    }
}

// Recebe uma linha "nome;tipo;combate;FA;dano" vinda de uma cena.
void Inventario::adicionarItem(string linha) {
    vector<string> campos = dividirCampos(linha, ';');
    if (campos.size() < 5) {
        return; // linha mal formatada: ignora
    }

    string nome = campos[0];
    string tipo = campos[1];

    if (tipo == "w") {
        bool combate = (campos[2] == "1");
        int fa = stoi(campos[3]);
        int dano = stoi(campos[4]);

        Arma* nova = new Arma(nome, combate, fa, dano);
        armas.push_back(nova);

        // Equipa automaticamente se ainda nao ha arma ou se a nova for melhor
        // (soma de FA + dano maior que a equipada atualmente).
        if (armaEquipada == nullptr || (fa + dano) > (armaEquipada->getFA() + armaEquipada->getDano())) {
            armaEquipada = nova;
        }
    }
    else if (tipo == "r") {
        outrosItens.push_back(nome + " (armadura)");
    }
    else {
        outrosItens.push_back(nome + " (item)");
    }
}

void Inventario::equiparArma(Arma* arma) {
    if (arma == nullptr) {
        return;
    }

    // Garante que a arma faca parte da lista antes de equipar.
    bool jaEstaNaLista = false;
    for (size_t i = 0; i < armas.size(); i++) {
        if (armas[i] == arma) {
            jaEstaNaLista = true;
            break;
        }
    }
    if (!jaEstaNaLista) {
        armas.push_back(arma);
    }

    armaEquipada = arma;
}

Arma* Inventario::getArmaEquipada() { return armaEquipada; }
int Inventario::getQuantidadeArmas() { return static_cast<int>(armas.size()); }
Arma* Inventario::getArma(int indice) { return armas.at(indice); }

int Inventario::getQuantidadeOutrosItens() { return static_cast<int>(outrosItens.size()); }
string Inventario::getOutroItem(int indice) { return outrosItens.at(indice); }

void Inventario::adicionarOuro(int quantidade) {
    if (quantidade > 0) {
        ouro += quantidade;
    }
}

int Inventario::getOuro() { return ouro; }

void Inventario::adicionarProvisoes(int quantidade) {
    if (quantidade > 0) {
        provisoes += quantidade;
    }
}

int Inventario::getProvisoes() { return provisoes; }

bool Inventario::consumirProvisao() {
    if (provisoes <= 0) {
        return false;
    }
    provisoes--;
    return true;
}

// ------------------------------------------------------------------
// Persistencia: escreve/le a fatia do arquivo de save que pertence ao
// Inventario. Formato:
//
//   ouro
//   provisoes
//   quantidade de armas
//   uma linha por arma -> nome;combate;fa;dano
//   indice da arma equipada na lista acima (-1 se nenhuma)
//   quantidade de outros itens (armaduras/itens comuns)
//   uma linha por item (texto livre, ja com o tipo escrito, ex: "Escudo (armadura)")
// ------------------------------------------------------------------

void Inventario::salvar(ofstream& arquivo) {
    arquivo << ouro << "\n";
    arquivo << provisoes << "\n";

    arquivo << armas.size() << "\n";
    int indiceEquipada = -1;
    for (size_t i = 0; i < armas.size(); i++) {
        Arma* a = armas[i];
        arquivo << a->getNome() << ";" << (a->getCombate() ? 1 : 0)
                << ";" << a->getFA() << ";" << a->getDano() << "\n";
        if (a == armaEquipada) {
            indiceEquipada = static_cast<int>(i);
        }
    }
    arquivo << indiceEquipada << "\n";

    arquivo << outrosItens.size() << "\n";
    for (size_t i = 0; i < outrosItens.size(); i++) {
        arquivo << outrosItens[i] << "\n";
    }
}

bool Inventario::carregar(ifstream& arquivo) {
    string linha;

    if (!getline(arquivo, linha)) return false;
    try { ouro = stoi(linha); } catch (...) { return false; }

    if (!getline(arquivo, linha)) return false;
    try { provisoes = stoi(linha); } catch (...) { return false; }

    if (!getline(arquivo, linha)) return false;
    int quantidadeArmas;
    try { quantidadeArmas = stoi(linha); } catch (...) { return false; }

    for (size_t i = 0; i < armas.size(); i++) {
        delete armas[i];
    }
    armas.clear();
    armaEquipada = nullptr;

    for (int i = 0; i < quantidadeArmas; i++) {
        if (!getline(arquivo, linha)) return false;

        vector<string> campos = dividirCampos(linha, ';');
        if (campos.size() != 4) return false;

        try {
            bool combate = (campos[1] == "1");
            int fa = stoi(campos[2]);
            int dano = stoi(campos[3]);
            armas.push_back(new Arma(campos[0], combate, fa, dano));
        } catch (...) {
            return false;
        }
    }

    if (!getline(arquivo, linha)) return false;
    int indiceEquipada;
    try { indiceEquipada = stoi(linha); } catch (...) { return false; }

    if (indiceEquipada >= 0 && indiceEquipada < static_cast<int>(armas.size())) {
        armaEquipada = armas[indiceEquipada];
    }

    if (!getline(arquivo, linha)) return false;
    int quantidadeOutros;
    try { quantidadeOutros = stoi(linha); } catch (...) { return false; }

    outrosItens.clear();
    for (int i = 0; i < quantidadeOutros; i++) {
        if (!getline(arquivo, linha)) return false;
        outrosItens.push_back(linha);
    }

    return true;
}
