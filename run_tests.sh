#!/bin/bash

ALG=${1:-apx}
TOL=2

mkdir -p tests/logs
exec > tests/logs/test_log_${ALG}.txt 2>&1

for filename in tests/inputs/*; do
    name=$(basename "$filename")   # extract filename without extension
    echo "Rodando teste: $name"

    # run program and capture its output
    output=$(python3 main.py "$filename" --alg "$ALG")

    # remove whitespace from output and expected value
    output_value=$(echo "$output" | tr -d '[:space:]')
    expected_value=$(cat tests/outputs/"$name" | tr -d '[:space:]')

    # check if values were successfully extracted
    if [[ -z "$output_value" || -z "$expected_value" ]]; then
        echo "Erro: não foi possível extrair valores para comparação."
        continue
    fi

    echo "Valor obtido: $output_value"
    echo "Valor esperado: $expected_value"

    if [[ "$ALG" == "bb" ]]; then
        # exact comparison for branch-and-bound
        if [[ "$output_value" -eq "$expected_value" ]]; then
            echo "✅ Teste $name passou."
        else
            echo "❌ Teste $name falhou."
        fi
    else
        # approximate comparison: output_value must be at least (expected_value / TOL)
        limit=$(echo "$expected_value / $TOL" | bc -l)
        comp=$(echo "$output_value >= $limit" | bc -l)

        if [[ "$comp" -eq 1 ]]; then
            echo "✅ Teste $name passou."
        else
            echo "❌ Teste $name falhou."
        fi
    fi

    echo "------------------------------"
done