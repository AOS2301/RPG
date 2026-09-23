#ifndef TEXTO_H
#define TEXTO_H

#include <string>
#include <vector>
#include <fstream>

using namespace std;

// Funcoes auxiliares de texto usadas por mais de uma classe.
// Ficam num unico lugar para nao existirem copias da mesma funcao
// espalhadas pelos arquivos (como main.cpp inclui os .cpp diretamente,
// duas funcoes de mesmo nome em arquivos diferentes colidiriam).

// Quebra "Adaga de prata;w;1;1;1" em {"Adaga de prata","w","1","1","1"}.
vector<string> dividir(string texto, char separador);

// Remove espacos e o '\r' (que arquivos criados no Windows deixam) das pontas.
string aparar(string texto);

// Le a proxima linha do arquivo e converte para inteiro em "valor".
// Retorna false se a linha nao existir ou nao for um numero.
bool lerLinhaInteira(ifstream &arquivo, int &valor);

// Numero com sinal explicito para exibir bonus: 2 -> "+2", -1 -> "-1".
string comSinal(int valor);

#endif // TEXTO_H
