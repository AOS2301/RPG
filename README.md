# A Lenda de Valdora — RPG em Texto (C++)

Trabalho Prático (Grau A) da disciplina de Programação Orientada a Objetos — UNISINOS.

Um RPG narrativo, jogado inteiramente pelo terminal, onde suas escolhas e sua sorte decidem o destino do personagem em busca do pai desaparecido nas ruínas da Fortaleza de Auren.

**Autores:** Arthur · Luiz Augusto Rodrigues

## Sobre o projeto

O jogo é guiado por cenas: cada cena é lida de um arquivo de texto simples e pode ser uma cena de narrativa (com escolhas), uma cena de combate contra um monstro, ou uma cena de teste de sorte fora de combate (ex: atravessar uma ponte em ruínas). O personagem evolui, coleta itens, armas e magias ao longo da jornada, e o progresso pode ser salvo e retomado a qualquer momento.

### Funcionalidades

- Criação de personagem com distribuição de pontos entre Habilidade, Energia e Sorte
- Evolução por níveis, com pontos de atributo para distribuir (inclusive os pontos não usados na criação)
- Combate por rodadas (1d10 + Habilidade de cada lado, quem tiver a maior Força de Ataque fere o outro), com ataque normal e ataque testando a Sorte
- Armas e armaduras com efeito real no combate: a arma soma FA e dano; a armadura reduz a FA do oponente e o dano recebido
- Teste de Sorte fora de combate (ex: atravessar uma ponte, passar por um túnel apertado), decrementando a Sorte a cada uso, como definido no enunciado do trabalho
- Inventário: armas, armaduras, itens mágicos (usáveis em combate), itens comuns, ouro e provisões, com opção de trocar a arma e a armadura equipadas pela ficha do personagem
- Sistema de magia: itens mágicos consomem-se ao serem usados, causando dano garantido
- Salvar/Carregar: cada personagem tem seu próprio arquivo em `data/<nome>.txt`; salvar novamente com o mesmo nome sobrescreve a partida anterior
- Cenas ramificadas, com múltiplos finais possíveis

## Como compilar e rodar

O projeto compila em um único comando, sem necessidade de Makefile:

```
g++ main.cpp -o jogo.exe
./jogo.exe
```

Requer g++ com suporte a C++17 (recomendado: MSYS2/MinGW no Windows, ou g++ nativo no Linux/Mac).

## Estrutura do projeto

```
main.cpp            # ponto de entrada (inclui os .cpp diretamente)
include/            # headers (.h) de cada classe
  itens/            # Item (base), Arma, Armadura e ItemMagico
  inventario/       # Inventario
  personagens/      # Personagem (abstrata), Jogador e Monstro
  jogo/             # Jogo (telas e fluxo) e Cena (le os arquivos de cena)
  util/             # funcoes de texto compartilhadas (dividir, aparar...)
src/                # implementacao (.cpp) de cada classe, mesma organizacao do include/
cenes/              # arquivos de cena (uma por numero, ex: cenes/1.txt)
telas/              # textos fixos (abertura, criacao de personagem, creditos)
data/               # saves gerados durante o jogo (um arquivo por personagem)
```

A divisão de responsabilidades segue uma regra simples: `Jogo` é a camada de interface (menus, leitura do teclado, mensagens na tela e fluxo entre cenas), e as classes de personagem, inventário e itens são a camada de regras (calculam e alteram o próprio estado, sem imprimir nada).

## Orientação a Objetos

- **Herança**: `Jogador` e `Monstro` herdam de `Personagem` (classe abstrata); `Arma`, `Armadura` e `ItemMagico` herdam de `Item`
- **Polimorfismo**: `calcularForcaAtaque()` e `calcularDano()` são virtuais puros em `Personagem`. A rodada de combate (`Personagem::disputarRodada`) chama esses métodos através de `Personagem*`, então o `Jogador` soma os bônus da arma e da armadura e o `Monstro` luta só com a própria habilidade. Os itens também sobrescrevem `imprimeInfo()`
- **Composição**: `Jogador` possui um `Inventario`, que guarda armas, armaduras, itens mágicos e itens comuns
- **Ponteiros**: gerenciamento manual de memória (`new`/`delete`) para os itens e o personagem atual
- **Arquivos**: leitura das cenas (`cenes/*.txt`) e persistência do save (`data/*.txt`), ambos usando `ifstream`/`ofstream`

## Formato dos arquivos de cena

Cada arquivo em `cenes/` começa com um marcador de tipo:

- `#N` — cena de narrativa
- `m` — cena de monstro (combate)
- `s` — teste de sorte fora de combate

Seguido de linhas de texto, itens (`I: nome;tipo;combate;FA;dano`), opções (`#N: texto`), dados do monstro (`N:`, `H:`, `S:`, `E:`, `T:`, `P:`) ou dados do teste de sorte (`D:` dificuldade, `X:` dano na falha), e uma linha de destino (`sucesso;falha`).

O arquivo de save (`data/<nome>.txt`) grava os itens nesse mesmo formato e começa com uma linha de versão; saves de versões antigas do jogo são recusados com uma mensagem, em vez de serem lidos errado.

## Status

Projeto funcional e jogável do início ao fim, com múltiplos caminhos narrativos. Ajustes finos e balanceamento seguem em andamento.
