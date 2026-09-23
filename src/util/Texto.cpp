#include "../../include/util/Texto.h"

vector<string> dividir(string texto, char separador)
{
    vector<string> partes;
    string atual = "";

    for (size_t i = 0; i < texto.size(); i++)
    {
        if (texto[i] == separador)
        {
            partes.push_back(atual);
            atual = "";
        }
        else
        {
            atual += texto[i];
        }
    }
    partes.push_back(atual);
    return partes;
}

string aparar(string texto)
{
    while (!texto.empty() && (texto[texto.size() - 1] == '\r' || texto[texto.size() - 1] == ' '))
    {
        texto.erase(texto.size() - 1);
    }
    while (!texto.empty() && texto[0] == ' ')
    {
        texto.erase(0, 1);
    }
    return texto;
}

bool lerLinhaInteira(ifstream &arquivo, int &valor)
{
    string linha;
    if (!getline(arquivo, linha))
    {
        return false;
    }
    try
    {
        valor = stoi(linha);
    }
    catch (...)
    {
        return false; // a linha nao era um numero (arquivo corrompido)
    }
    return true;
}

string comSinal(int valor)
{
    if (valor >= 0)
    {
        return "+" + to_string(valor);
    }
    return to_string(valor);
}
