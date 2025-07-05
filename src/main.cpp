#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <filesystem>
#include <sys/resource.h>

#include "../include/branch_and_bound.h"
#include "../include/approximative.h"

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
    if (argc < 2) {
        std::cerr << "Uso: " << argv[0] << " <arquivo_entrada> [--alg bb|apx]" << std::endl;
        return EXIT_FAILURE;
    }

    std::string filename = argv[1];
    std::string alg = "apx";

    if (argc == 4 && std::string(argv[2]) == "--alg") {
        alg = argv[3];
        if (alg != "bb" && alg != "apx") {
            std::cerr << "Algoritmo inválido. Use 'bb' ou 'apx'." << std::endl;
            return EXIT_FAILURE;
        }
    }

    if (!std::filesystem::exists(filename)) {
        std::cerr << "Erro: Arquivo '" << filename << "' não encontrado." << std::endl;
        return EXIT_FAILURE;
    }

    Instance instance = read_input(filename);

    struct rusage usage_before, usage_after;
    getrusage(RUSAGE_SELF, &usage_before);

    int max_value;
    if (alg == "bb") {
        max_value = branch_and_bound::knapsack(instance);
    } else {
        max_value = approximative::knapsack(instance, 0.5);
    }

    getrusage(RUSAGE_SELF, &usage_after);

    double elapsed_ms = time_in_ms(usage_before, usage_after);
    double memory_kb = static_cast<double>(usage_after.ru_maxrss);

    std::cout << max_value << std::endl;
    std::cout << elapsed_ms << std::endl;
    std::cout << memory_kb << std::endl;
    std::cout << memory_kb << std::endl;

    return 0;
}