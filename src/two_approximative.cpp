#include <vector>
#include <algorithm>
#include <utility>

#include "../include/two_approximative.h"

namespace two_approximative {

    int knapsack(const Instance& instance) {
        int n = instance.n;
        int capacity = instance.wmax;
        const std::vector<int>& values = instance.values;
        const std::vector<int>& weights = instance.weights;

        if (n == 0 || capacity <= 0) 
            return 0;

        // Candidate 1: item with the highest value that fits alone
        int candidate1 = 0;
        for (int i = 0; i < n; ++i) {
            if (weights[i] <= capacity && values[i] > candidate1) {
                candidate1 = values[i];
            }
        }

        // Candidate 2: greedy by density (value/weight)
        std::vector<std::pair<double, int>> items; // pair: density, index
        for (int i = 0; i < n; ++i) {
            if (weights[i] > 0) { // avoid division by zero
                double density = static_cast<double>(values[i]) / weights[i];
                items.push_back(std::make_pair(density, i));
            }
        }

        // Sort by density descending
        std::sort(items.begin(), items.end(), 
            [](const std::pair<double, int>& a, const std::pair<double, int>& b) {
                return a.first > b.first;
            });

        int current_value = 0;
        int current_weight = 0;
        for (const auto& item : items) {
            int idx = item.second;
            if (current_weight + weights[idx] <= capacity) {
                current_value += values[idx];
                current_weight += weights[idx];
            }
        }
        int candidate2 = current_value;

        return std::max(candidate1, candidate2);
    }

}
