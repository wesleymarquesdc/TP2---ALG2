#!/bin/bash

ALG=${1:-apx}
TOL_APX=1.5    # Tolerância mais rigorosa para FPTAS (1 + ε)
TOL_2APX=2.0   # Tolerância para 2-aproximativo (50% do ótimo)

mkdir -p tests/logs
exec > tests/logs/test_log_${ALG}.txt 2>&1

for filename in tests/inputs/*; do
    name=$(basename "$filename")
    echo "Rodando teste: $name"

    # run compiled C++ program from bin/ folder with args
    output=$(make run ARGS="$filename --alg $ALG")

    # remove whitespace from output lines
    output_value=$(echo "$output" | sed -n '1p' | tr -d '[:space:]')
    output_time=$(echo "$output" | sed -n '2p' | tr -d '[:space:]')
    output_current_mem=$(echo "$output" | sed -n '3p' | tr -d '[:space:]')
    output_peak_mem=$(echo "$output" | sed -n '4p' | tr -d '[:space:]')

    expected_value=$(cat tests/outputs/"$name" | tr -d '[:space:]')

    # check if values were successfully extracted
    if [[ -z "$output_value" || -z "$expected_value" ]]; then
        echo "Erro: não foi possível extrair valores para comparação."
        continue
    fi

    echo "Valor obtido: $output_value"
    echo "Valor esperado: $expected_value"
    echo "Tempo: ${output_time} ms"
    echo "Memoria atual: ${output_current_mem} KB"
    echo "Pico de memoria: ${output_peak_mem} KB"

    if [[ "$ALG" == "bb" ]]; then
        # exact comparison for branch-and-bound
        if [[ "$output_value" -eq "$expected_value" ]]; then
            echo "✅ Teste $name passou."
        else
            echo "❌ Teste $name falhou."
        fi
    elif [[ "$ALG" == "apx" ]]; then
        # FPTAS: (1-ε)-aproximação - tolerância mais rigorosa
        limit=$(echo "$expected_value / $TOL_APX" | bc -l)
        comp_min=$(echo "$output_value >= $limit" | bc -l)
        comp_max=$(echo "$output_value <= $expected_value" | bc -l)

        if [[ "$comp_min" -eq 1 && "$comp_max" -eq 1 ]]; then
            echo "✅ Teste $name passou."
        else
            echo "❌ Teste $name falhou."
        fi
    elif [[ "$ALG" == "2apx" ]]; then
        # 2-aproximativo: ≥ 50% do ótimo
        limit=$(echo "$expected_value / $TOL_2APX" | bc -l)
        comp_min=$(echo "$output_value >= $limit" | bc -l)
        comp_max=$(echo "$output_value <= $expected_value" | bc -l)

        if [[ "$comp_min" -eq 1 && "$comp_max" -eq 1 ]]; then
            echo "✅ Teste $name passou."
        else
            echo "❌ Teste $name falhou."
        fi
    fi

    echo "------------------------------"
done
