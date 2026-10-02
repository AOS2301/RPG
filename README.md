# A Lenda de Valdora — A Chama de Auren

> *Há três noites, o Farol de Âmbar se apagou.*

Um livro-jogo interativo em modo texto, no estilo das antigas **Aventuras Fantásticas** (*Fighting Fantasy*), escrito em **C++ orientado a objetos**. Você lê a história, faz escolhas, enfrenta monstros e testa a sorte — e cada decisão muda o caminho até o fim.

Trabalho Prático do Grau A da disciplina **Programação Orientada a Objetos** — UNISINOS, 2026.

---

## 📜 A história

Seu pai era um Guardião da Ordem, os protetores dos selos que mantinham as criaturas do norte longe de Valdora. Há três anos ele partiu para as ruínas da **Fortaleza de Auren** e nunca mais voltou.

Numa noite de tempestade, **Lira**, uma mensageira ferida da Ordem, bate à sua porta com o medalhão de ferro dele nas mãos. Naquela mesma noite, pela primeira vez em cem anos, a luz do farol se apaga. Antes de desmaiar, ela sussurra um nome esquecido: **Vaelthar**.

Agora você segue pela antiga Estrada dos Guardiões, com o medalhão pulsando no peito, rumo à fortaleza. Capelas abandonadas, minas infestadas, criptas e o topo do farol esperam por você — e nem todos os caminhos levam ao mesmo final.

- **37 cenas** cheias de escolhas, caminhos que se cruzam e atalhos arriscados
- **3 finais** diferentes
- Batalhas, testes de sorte, itens mágicos e um chefe final

---

## ▶️ Como compilar e jogar

É preciso ter o **g++** instalado. O jogo usa só a biblioteca padrão do C++, então funciona no **Linux** e no **Windows**.

> ⚠️ Rode o jogo sempre **a partir da pasta do projeto**: ele lê as cenas em `cenas/`, as telas em `telas/` e grava os saves em `data/`.

### Linux

```bash
./compilar.sh
./jogo
```

(ou diretamente: `g++ main.cpp src/*/*.cpp -o jogo`)

### Windows (MinGW)

```bat
g++ main.cpp src/itens/*.cpp src/personagens/*.cpp src/inventario/*.cpp src/jogo/*.cpp -static -o jogo.exe
jogo.exe
```

O `-static` coloca as bibliotecas do C++ dentro do `.exe`, para ele abrir mesmo em computadores sem o MinGW instalado.

---

## 🎮 Como se joga

### Criação do personagem
Todo personagem começa com os valores mínimos e recebe **12 pontos extras**:

| Atributo     | Mínimo | Máximo | Para que serve                      |
|--------------|:------:|:------:|-------------------------------------|
| **HABILIDADE** | 6    | 12     | Destreza em combate                 |
| **ENERGIA**    | 12   | 24     | Pontos de vida                      |
| **SORTE**      | 6    | 12     | Testes de sorte e golpes decisivos  |

Há dois modos de criação: **distribuir os pontos** à mão ou **sortear aleatoriamente**. Pontos não usados ficam **guardados** e podem ser gastos depois, na tela de inventário.

Você começa com uma **Espada curta**, um **Gibão de couro** e **2 provisões**.

### Telas
- **Abertura** — novo jogo, carregar jogo, créditos e sair
- **Inventário** — atributos, itens, o que está equipado, ouro e provisões; aqui você equipa armas e armaduras, come provisões e gasta pontos guardados
- **Narrativa** — o texto da cena e as suas escolhas (a opção `0` abre o inventário a qualquer momento)
- **Batalha** — o inimigo e a energia dele, com as opções *Atacar*, *Atacar testando a sorte*, *Usar item / magia* e *Fugir*

### Combate
A cada rodada, você e o monstro calculam a **Força de Ataque (FA)**: um número de 1 a 10 somado à habilidade. Quem tiver a maior FA acerta e tira **2 pontos de energia** do outro; no empate, ninguém acerta.

- A **arma** equipada soma no seu FA e no dano que você causa
- A **armadura** equipada reduz o FA do inimigo e o dano que você recebe
- **Atacar testando a sorte** gasta 1 ponto de sorte: se der certo, seu golpe fere mais (ou o do monstro fere menos); se falhar, o contrário
- **Itens mágicos** (como pergaminhos e dardos rúnicos) causam dano direto e são gastos ao usar
- Alguns monstros **não deixam você fugir**
- Monstros derrotados entregam **tesouro, provisões e itens**, além de **pontos de evolução**

### Sorte
Ao testar a sorte, é sorteado um número de 1 a 12: se for menor ou igual à sua SORTE, deu certo. Com sucesso ou não, a sorte **diminui 1 ponto**.

### Provisões
Cada provisão recupera **4 de energia** (sem passar da sua energia máxima) e só pode ser usada **fora de combate**.

### Salvar e carregar
O jogo é **salvo automaticamente** sempre que uma nova cena é carregada. Cada personagem tem o seu próprio arquivo em `data/<nome>.txt`, então dá para manter **várias partidas** ao mesmo tempo. Para continuar, escolha *Carregar jogo* e digite o nome do personagem.

---

## 🧱 Estrutura do código

O projeto separa **interface** (`.h`, em `include/`) de **implementação** (`.cpp`, em `src/`), organizados por assunto:

```
RPG/
├── main.cpp
├── compilar.sh
├── include/  e  src/
│   ├── itens/         Item, Arma, Armadura, ItemComum, CriarItem
│   ├── personagens/   Personagem, Jogador, Monstro
│   ├── inventario/    Inventario
│   └── jogo/          Cena, Jogo
├── cenas/             1.txt ... 37.txt
├── telas/             abertura, criacao, creditos
└── data/              saves (gerados durante o jogo)
```

### Classes

```
Item (abstrata)              Personagem (abstrata)
 ├── Arma        (w)          ├── Jogador  ── tem um ──> Inventario ──> vector<Item*>
 ├── Armadura    (r)          └── Monstro  ── carrega ─> Item*
 └── ItemComum   (c)

Cena  ── lê cenas/N.txt e cria o Monstro da cena
Jogo  ── menus, telas, batalhas, salvar/carregar
```

| Classe | Responsabilidade |
|---|---|
| `Item` | Superclasse abstrata dos itens (nome, tipo, combate, FA, dano). `mostrar()` é virtual pura. |
| `Arma`, `Armadura`, `ItemComum` | Cada tipo de item se mostra do seu jeito (polimorfismo). |
| `Personagem` | Superclasse abstrata com HABILIDADE, ENERGIA e SORTE. `forcaAtaque()` é virtual pura. |
| `Jogador` | Soma arma e armadura no combate, come provisões e gasta pontos guardados. |
| `Monstro` | Guarda tesouro, provisões, item e se é possível fugir dele. |
| `Inventario` | Dono dos itens: guarda, equipa, remove e salva/carrega. |
| `Cena` | Lê um arquivo de cena e guarda o texto, as opções e os dados do monstro. |
| `Jogo` | Conversa com o usuário e conduz a aventura do início ao fim. |

### Conceitos de POO aplicados
- **Encapsulamento** — atributos `private`/`protected` acessados por métodos
- **Herança** — `Arma`, `Armadura` e `ItemComum` herdam de `Item`; `Jogador` e `Monstro` herdam de `Personagem`
- **Polimorfismo** — métodos `virtual` (`mostrar()`, `forcaAtaque()`) chamados através de ponteiros para a superclasse
- **Classes abstratas** — `Item` e `Personagem` têm métodos virtuais puros (`= 0`)
- **Ponteiros e memória dinâmica** — itens, monstros, jogador e inventário criados com `new` e liberados com `delete`; destrutores `virtual`
- **Arquivos** — cenas e telas lidas com `ifstream`/`getline`; saves gravados com `ofstream`

---

## 📄 Formato dos arquivos de cena

Cada cena fica em `cenas/N.txt`. A primeira linha indica o tipo.

**Cena narrativa**
```
#1
Texto da cena...

I: Medalhao dos Guardioes;c;0;0;0
#2: Ir ate a Capela de Santa Elara
#29: Atravessar a ponte em ruinas
```
`I:` dá um item ao jogador (só na primeira visita). Cada `#N: texto` é uma escolha que leva à cena `N`. Uma cena sem escolhas é um **final**.

**Cena de monstro**
```
m
Texto da cena...

N: Guardiao esquecido
M: N
H: 14
S: 9
E: 16
T: 40
P: 1
I: Pergaminho de Luz;c;1;0;4
25;37
```
`N` nome · `M` pode fugir (`S`/`N`) · `H` habilidade · `S` sorte · `E` energia · `T` tesouro · `P` provisões · `I` item · `25;37` = cena se vencer ; cena se perder.

**Cena de teste de sorte** *(extensão deste projeto)*
```
s
Texto da cena...

X: 4
3;31
```
`X` é a energia perdida se falhar · `3;31` = cena se tiver sucesso ; cena se falhar.

**Itens** seguem o formato `nome;tipo;combate;FA;dano`, onde o tipo é `w` (arma), `r` (armadura) ou `c` (item comum), e `combate` é `1` se o item pode ser usado na batalha.

---

## 👥 Autores

- **Arthur Osvaldo Schmitz**
- **Luiz Augusto Rodrigues**

Programação Orientada a Objetos — UNISINOS · 2026
