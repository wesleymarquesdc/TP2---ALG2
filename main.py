import argparse
import os
import sys
import time
import tracemalloc

import branch_and_bound
import approximative

def read_input(filename):
    with open(filename, 'r') as f:
        lines = f.readlines()

    # remove whitespaces
    lines = [line.strip() for line in lines if line.strip()]

    # read input data
    n, wmax = map(int, lines[0].split())
    items = []
    for line in lines[1:n+1]:
        v, w = map(int, line.split())
        items.append((v, w))

    return n, wmax, items

def parse_args():
    parser = argparse.ArgumentParser(description='Algoritmos que solucionam o problema da mochila.')

    parser.add_argument(
        'filename',
        type=str,
        help='Caminho para o arquivo de entrada.'
    )

    parser.add_argument(
        '--alg',
        choices=['bb', 'apx'],
        default='apx',
        help='Algoritmo a ser usado: bb (branch-and-bound) ou apx (aproximativo). Padrão: apx'
    )

    args = parser.parse_args()

    if not os.path.isfile(args.filename):
        print(f"Erro: Arquivo '{args.filename}' não encontrado.")
        sys.exit(1)

    args.filename = os.path.abspath(args.filename)
    return args

def main():
    args = parse_args()
    instance = read_input(args.filename)

    # time measurement (ms)
    start = time.perf_counter()
    # space measurement (kB)
    tracemalloc.start()

    if args.alg == 'bb':
        max_value = branch_and_bound.knapsack(instance)
    else:
        max_value = approximative.knapsack(instance, 0.5)

    end = time.perf_counter() 
    elapsed_ms = (end - start) * 1000
    current, peak = tracemalloc.get_traced_memory()
    tracemalloc.stop()

    print(max_value)
    print(elapsed_ms)
    print(f"{current / 1024}")
    print(f"{peak / 1024}")

if __name__ == '__main__':
    main()