# RPG
Trabalho Grau A sobre Programação orientada a objetos

## Como compilar e rodar

O projeto usa um `Makefile`: cada `.cpp` é compilado separadamente e depois
linkado num executável só.

```
make        # compila (so recompila o que mudou)
make run    # compila e ja executa o jogo
make clean  # apaga os .o e o executavel
```

Requer g++ com suporte a C++17.
