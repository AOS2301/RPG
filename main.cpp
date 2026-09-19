#include <iostream>
#include "src/personagens/Personagem.cpp"
#include "src/personagens/Jogador.cpp"
#include "src/armas/Arma.cpp"

using namespace std;

int main() {
    cout << "===== TESTE DE Personagem / Jogador =====" << endl << endl;

    // Personagem eh abstrata -> nao da pra instanciar direto.
    // Personagem p("Teste", 10, 20, 8); // <-- isso NAO compila, e eh esperado

    // Criando dois Jogadores atraves de ponteiro de Personagem*
    // (isso ja exercita o polimorfismo: p1 e p2 "sao" Jogador por baixo dos panos)
    Personagem* p1 = new Jogador("Gandalf", 10, 20, 8);
    Personagem* p2 = new Jogador("Aragorn", 9, 22, 7);

    // ---- Testando getters ----
    cout << "-- Status inicial --" << endl;
    cout << p1->getNome() << " | Habilidade: " << p1->getHabilidade()
         << " | Energia: " << p1->getEnergia()
         << " | Sorte: " << p1->getSorte() << endl;
    cout << p2->getNome() << " | Habilidade: " << p2->getHabilidade()
         << " | Energia: " << p2->getEnergia()
         << " | Sorte: " << p2->getSorte() << endl << endl;

    // ---- Testando estaVivo() ----
    cout << "-- estaVivo() --" << endl;
    cout << p1->getNome() << " esta vivo? " << (p1->estaVivo() ? "Sim" : "Nao") << endl << endl;

    // ---- Testando receberDano() ----
    cout << "-- receberDano(15) em " << p2->getNome() << " --" << endl;
    p2->receberDano(15);
    cout << p2->getNome() << " agora tem " << p2->getEnergia() << " de energia" << endl << endl;

    // ---- Testando testarSorte() ----
    cout << "-- testarSorte() de " << p1->getNome() << " --" << endl;
    bool sucesso = p1->testarSorte();
    cout << "Resultado: " << (sucesso ? "SUCESSO" : "FALHA")
         << " | Sorte apos o teste: " << p1->getSorte() << endl << endl;

    // ---- Testando atacar() (polimorfismo) ----
    // Chamado atraves de Personagem*, mas quem executa de verdade
    // eh Jogador::atacar(), por causa do virtual
    cout << "-- Combate: " << p1->getNome() << " ataca " << p2->getNome() << " --" << endl;
    p1->atacar(p2);
    cout << p2->getNome() << " ficou com " << p2->getEnergia() << " de energia" << endl << endl;

    // ---- Liberando a memoria ----
    // delete atraves de Personagem* so funciona certo (chamando ~Jogador() e ~Personagem())
    // porque o destrutor de Personagem eh virtual
    delete p1;
    delete p2;

    cout << "===== FIM DO TESTE =====" << endl;
    return 0;
}