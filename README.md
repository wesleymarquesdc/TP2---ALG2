# TP2 — ALG2

Este projeto implementa duas abordagens para resolução do problema da mochila binária: **Branch and Bound (bb)** e **Aproximativo (apx)**.

---

## Como Executar os Testes

### Executar Todos os Testes

Para rodar todos os testes de uma vez e salvar os resultados:

```bash
bash run_tests.sh bb   # Executa com o algoritmo Branch and Bound
bash run_tests.sh apx  # Executa com o algoritmo Aproximativo
```

Os resultados serão salvos na pasta: `tests/logs/`

---

### Executar um Único Teste

Você pode executar um teste específico com o comando `make run`, passando os argumentos desejados:

```bash
make run ARGS="tests/inputs/test-name --alg bb"   # Branch and Bound
make run ARGS="tests/inputs/test-name --alg apx"  # Aproximativo
```

O terminal exibirá:
- Valor da solução
- Tempo gasto
- Memória utilizada
- Memória máxima utilizada

---

## Requisitos

Para compilar e executar o projeto, é necessário ter os seguintes componentes instalados:

- **g++** (compilador C++ com suporte a C++17)
- **make** (para compilar e rodar o projeto)
- **bash** (para executar o script `run_tests.sh`)