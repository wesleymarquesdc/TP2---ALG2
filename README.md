# TP2 — ALG2

## Integrantes
- Lucas Paulo de Oliveira Silva - 2022043469
- Wesley Marques Daniel Chaves - 2020053246

Este projeto implementa três abordagens para resolução do problema da mochila binária:  
- **Branch and Bound (bb)** - algoritmo exato  
- **Aproximativo (apx)** - FPTAS com parâmetro epsilon  
- **2-aproximativo (apx2)** - algoritmo com fator de aproximação 2 

---
## Relatório e planilhas
- As planilhas `resultados_apx.csv`, `resultados_apx2.csv` e `resultados_bb.csv` representam os resultados da execução dos algoritmos aproximativo (versão vista em sala de aula), 2-aproximativo (proposto aqui neste projeto) e branch-and-bound, respectivamente.
- O relatório do projeto é o arquivo `Relatorio.pdf`

## Como Compilar

Para compilar todos os executáveis, rode:

```bash
make all
```

## Como Executar os Testes
Você deve estar na raiz do projeto.

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

Você pode executar um teste específico com o comando `make run`, passando os argumentos (substitua `test-name` pela instância que você quer executar):

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
