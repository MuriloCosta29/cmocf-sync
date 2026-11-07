# cmocf-sync

Atividade de Infraestrutura de Software: sincronização de carros e passageiros em uma estação de metrô, usando `pthread_mutex` e `pthread_cond`.

## Arquivos

- `sync_car.h`: declara `struct station` e os protótipos das funções.
- `sync_car.c`: implementa `station_init`, `station_load_car`, `station_wait_for_car` e `station_on_board`.
- `sync_car-runner.c`: programa de teste fornecido (cria 100 passageiros e vários carros).
- `Makefile`: compila e limpa o projeto.

## Como usar

Compilar (gera o executável `sync_car`):

```sh
make
```

Executar e testar (compila e roda o teste; o resultado esperado termina com `Looks good!`):

```sh
make test
```

Limpar o executável:

```sh
make clean
```

## Sistema operacional

Implementado e testado no macOS (Darwin, arm64), com `cc` (Apple clang). Funciona também em Linux.
