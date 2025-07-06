#!/bin/bash

alg=${1:-apx}    # algorithm to run
epsilon=$2       # epsilon value for 'apx' algorithm

mkdir -p tests/logs

# redirect all output (stdout and stderr) to a log file
exec > tests/logs/test_log_${alg}.txt 2>&1

# loop through all input files in tests/inputs
for filename in tests/inputs/*; do
    name=$(basename "$filename")
    echo "Rodando teste: $name"

    # run the program via 'make run' passing arguments
    output=$(make run ARGS="$filename $alg $epsilon")

    # extract output lines (value, time, memory)
    output_value=$(echo "$output" | sed -n '1p' | tr -d '[:space:]')
    output_time=$(echo "$output" | sed -n '2p' | tr -d '[:space:]')
    output_memory=$(echo "$output" | sed -n '3p' | tr -d '[:space:]')

    # read expected value from output file (remove whitespace)
    expected_value=$(cat tests/outputs/"$name" | tr -d '[:space:]')

    # check if output and expected values are extracted correctly
    if [[ -z "$output_value" || -z "$expected_value" ]]; then
        echo "Erro: não foi possível extrair valores para comparação."
        continue
    fi

    # print results
    echo "Valor obtido: $output_value"
    echo "Valor esperado: $expected_value"
    echo "Tempo: ${output_time} ms"
    echo "Pico de memoria: ${output_memory} kb"

    # evaluation depending on algorithm
    if [[ "$alg" == "bb" ]]; then
        # branch and bound: exact match required
        if [[ "$output_value" -eq "$expected_value" ]]; then
            echo "✅ Teste $name passou."
        else
            echo "❌ Teste $name falhou."
        fi
    elif [[ "$alg" == "apx" ]]; then
        # (1 - epsilon)-approximation for approximate algorithm
        limit_min=$(echo "$expected_value * (1 - $epsilon)" | bc -l)
        comp_min=$(echo "$output_value >= $limit_min" | bc -l)
        comp_max=$(echo "$output_value <= $expected_value" | bc -l)
        
        ratio=$(echo "scale=4; $output_value / $expected_value * 100" | bc -l)
        dist=$(echo "scale=4; (1 - $output_value / $expected_value) * 100" | bc -l)

        if [[ "$comp_min" -eq 1 && "$comp_max" -eq 1 ]]; then
            echo "Aproximação do ótimo: $ratio %"
            echo "Erro percentual: $dist %"
            echo "✅ Teste $name passou."
        else
            echo "Aproximação do ótimo: $ratio %"
            echo "Erro percentual: $dist %"
            echo "❌ Teste $name falhou."
        fi
    else
        # apx2: 2-approximation, at least 50% of optimum
        limit_min=$(echo "$expected_value / 2" | bc -l)
        comp_min=$(echo "$output_value >= $limit_min" | bc -l)
        comp_max=$(echo "$output_value <= $expected_value" | bc -l)

        ratio=$(echo "scale=4; $output_value / $expected_value * 100" | bc -l)
        dist=$(echo "scale=4; (1 - $output_value / $expected_value) * 100" | bc -l)

        if [[ "$comp_min" -eq 1 && "$comp_max" -eq 1 ]]; then
            echo "Aproximação do ótimo: $ratio %"
            echo "Erro percentual: $dist %"
            echo "✅ Teste $name passou."
        else
            echo "Aproximação do ótimo: $ratio %"
            echo "Erro percentual: $dist %"
            echo "❌ Teste $name falhou."
        fi
    fi

    echo "------------------------------"
done