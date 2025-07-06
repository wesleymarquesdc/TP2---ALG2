# TP2 — ALG2

Este projeto implementa três abordagens para resolução do problema da mochila binária:  
- **Branch and Bound (bb)** - algoritmo exato  
- **Aproximativo (apx)** - FPTAS com parâmetro epsilon  
- **2-aproximativo (apx2)** - algoritmo com fator de aproximação 2 

---

## Como Executar os Testes

### Executar Todos os Testes

Para rodar todos os testes e salvar os resultados em arquivo:

```bash
bash run_tests.sh bb         # Executa todos os testes com Branch and Bound
bash run_tests.sh apx 0.5    # Executa todos os testes com Aproximativo (epsilon obrigatório)
bash run_tests.sh apx2       # Executa todos os testes com 2-aproximativo
```

Os resultados serão salvos na pasta: `tests/logs/`

---

### Executar um Único Teste

Você pode executar um teste específico com o comando `make run`, passando os argumentos:

```bash
make run ARGS="tests/inputs/test-name bb"            # Branch and Bound
make run ARGS="tests/inputs/test-name apx 0.5"       # Aproximativo (epsilon obrigatório)
make run ARGS="tests/inputs/test-name apx2"          # 2-aproximativo
```

O terminal exibirá, nesta ordem:  
- Valor da solução  
- Tempo gasto (ms)  
- Memória máxima utilizada (KB)  

---

## Requisitos

- **g++** com suporte a C++17  
- **make**  
- **bash**

---

## Observações

- Para o algoritmo `apx`, o parâmetro `epsilon` deve ser informado e estar no intervalo (0,1).  
- Os algoritmos `bb` e `apx2` não recebem parâmetro `epsilon`.  
- O script `run_tests.sh` está configurado para validar os resultados conforme o tipo de algoritmo, aplicando as tolerâncias corretas.