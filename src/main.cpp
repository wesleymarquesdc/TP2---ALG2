#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <filesystem>
#include <sys/resource.h>

#include "../include/branch_and_bound.h"
#include "../include/approximative.h"
#include "../include/two_approximative.h"

Instance read_input(const std::string& filename) {
    std::ifstream infile(filename);
    if (!infile.is_open()) {
        std::cerr << "Erro ao abrir o arquivo: " << filename << std::endl;
        exit(EXIT_FAILURE);
    }

    Instance instance;
    infile >> instance.n >> instance.wmax;

    for (int i = 0; i < instance.n; ++i) {
        int value, weight;
        infile >> value >> weight;
        instance.values.push_back(value);
        instance.weights.push_back(weight);
    }

    return instance;
}

double time_in_ms(const struct rusage& usage_before, const struct rusage& usage_after) {
    long user_sec  = usage_after.ru_utime.tv_sec  - usage_before.ru_utime.tv_sec;
    long user_usec = usage_after.ru_utime.tv_usec - usage_before.ru_utime.tv_usec;
    long sys_sec   = usage_after.ru_stime.tv_sec  - usage_before.ru_stime.tv_sec;
    long sys_usec  = usage_after.ru_stime.tv_usec - usage_before.ru_stime.tv_usec;

    long total_usec = (user_sec + sys_sec) * 1000000L + user_usec + sys_usec;
    return static_cast<double>(total_usec) / 1000.0;  // ms
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Uso: " << argv[0] << " <arquivo_entrada> <algoritmo> [epsilon]\n";
        std::cerr << "Algoritmos disponíveis:\n";
        std::cerr << "  bb         (exato)\n";
        std::cerr << "  apx <eps>  (aproximado, epsilon ∈ (0,1))\n";
        std::cerr << "  apx2       (2-aproximativo)\n";
        return EXIT_FAILURE;
    }

    std::string filename = argv[1];
    std::string alg = argv[2];

    if (!std::filesystem::exists(filename)) {
        std::cerr << "Erro: Arquivo '" << filename << "' não encontrado.\n";
        return EXIT_FAILURE;
    }

    if (alg == "bb" || alg == "apx2") {
        if (argc != 3) {
            std::cerr << "Erro: Algoritmo '" << alg << "' não requer valor de epsilon.\n";
            return EXIT_FAILURE;
        }
    } else if (alg == "apx") {
        if (argc != 4) {
            std::cerr << "Erro: Algoritmo 'apx' requer um valor de epsilon como terceiro argumento.\n";
            return EXIT_FAILURE;
        }
    } else {
        std::cerr << "Erro: Algoritmo inválido. Use 'bb', 'apx' ou 'apx2'.\n";
        return EXIT_FAILURE;
    }

    double epsilon = 0.0;
    if (alg == "apx") {
        try {
            epsilon = std::stod(argv[3]);
            if (epsilon <= 0.0 || epsilon >= 1.0) {
                std::cerr << "Erro: epsilon deve estar entre 0 e 1 (ex: 0.1, 0.5).\n";
                return EXIT_FAILURE;
            }
        } catch (...) {
            std::cerr << "Erro: epsilon inválido. Deve ser um número no intervalo (0, 1).\n";
            return EXIT_FAILURE;
        }
    }

    Instance instance = read_input(filename);

    struct rusage usage_before, usage_after;
    getrusage(RUSAGE_SELF, &usage_before);

    int max_value = 0;
    if (alg == "bb") {
        max_value = branch_and_bound::knapsack(instance);
    } else if (alg == "apx") {
        max_value = approximative::knapsack(instance, epsilon);
    } else if (alg == "apx2") {
        max_value = two_approximative::knapsack(instance);
    }

    getrusage(RUSAGE_SELF, &usage_after);

    double elapsed_ms = time_in_ms(usage_before, usage_after);
    double memory_kb = static_cast<double>(usage_after.ru_maxrss);

    std::cout << max_value << std::endl;
    std::cout << elapsed_ms << std::endl;
    std::cout << memory_kb << std::endl;

    return 0;
}