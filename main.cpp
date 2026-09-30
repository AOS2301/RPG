#include <cstdlib>
#include <ctime>
#include "include/jogo/Jogo.h"

int main()
{
    srand(time(nullptr)); // sem isso o rand() sorteia sempre os mesmos numeros

    Jogo jogo;
    jogo.executar();

    return 0;
}
