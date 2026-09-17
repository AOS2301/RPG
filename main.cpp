#include "../src/pessoa.cpp"
#include <iostream>

int main() {     
    pessoa p;   

    String nome;
    int idade;
    float altura;
    int qtdIrmaos;
    String endereco;

    std::cout << "Informe um nome: ";
    std::cin >> nome;
    std::cout << std::endl;
    std::cout << "Informe a idade da pessoa: ";
    std::cin >> idade;
    std::cout << std::endl;
    std::cout << "Informe a altura da pessoa: ";
    std::cin >> altura;
    std::cout << std::endl;
    std::cout << "Informe a quantidade de irmãos da pessoa: ";
    std::cin >> qtdIrmaos;
    std::cout << std::endl;
    std::cout << "Informe o endereço da pessoa: ";
    std::cin >> endereco;

    std::cout << "As infos foram " << p.area(base, altura);

    return 0;                                                                
}                                                                   
