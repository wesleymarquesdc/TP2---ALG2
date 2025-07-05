#include "../include/approximative.h"
#include <vector>
#include <cmath>
#include <algorithm>
#include <limits>

namespace approximative {

int knapsack(const Instance& instance, double epsilon) {
    int n = instance.n;
    int capacity = instance.wmax;
    const std::vector<int>& values = instance.values;
    const std::vector<int>& weights = instance.weights;

    int v_max = *std::max_element(values.begin(), values.end());

    // scale and round values
    double mu = (epsilon * v_max) / n;

    std::vector<int> v_prime(n);
    int v_prime_sum = 0;
    for (int i = 0; i < n; ++i) {
        v_prime[i] = static_cast<int>(std::floor(values[i] / mu));
        v_prime_sum += v_prime[i];
    }

    // initialize DP table with infinity
    const int INF = std::numeric_limits<int>::max();
    std::vector<std::vector<int>> dp(n + 1, std::vector<int>(v_prime_sum + 1, INF));
    dp[0][0] = 0;

    // fill DP table
    for (int i = 1; i <= n; ++i) {
        int vi_prime = v_prime[i - 1];
        int wi = weights[i - 1];
        for (int v = 0; v <= v_prime_sum; ++v) {
            // do not take item i
            dp[i][v] = dp[i - 1][v];
            // take item i if possible
            if (v >= vi_prime && dp[i - 1][v - vi_prime] != INF && dp[i - 1][v - vi_prime] + wi <= capacity) {
                dp[i][v] = std::min(dp[i][v], dp[i - 1][v - vi_prime] + wi);
            }
        }
    }

    // find maximum rounded value
    int max_v_prime = 0;
    for (int v = 0; v <= v_prime_sum; ++v) {
        if (dp[n][v] <= capacity) {
            max_v_prime = v;
        }
    }

    int approx_value = static_cast<int>(std::floor(mu * max_v_prime));
    return approx_value;
}

}