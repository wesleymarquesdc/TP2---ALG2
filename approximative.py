from math import floor

def knapsack(instance, epsilon):
    n, capacity, item_list = instance
    values = [v for v, _ in item_list]
    weights = [w for _, w in item_list]

    # scale and round values
    v_max = max(values)
    mu = (epsilon * v_max) / n
    v_prime = [floor(v / mu) for v in values]

    # sum of rounded values
    v_prime_sum = sum(v_prime)

    # initialize DP table with infinity
    dp = [[float('inf')] * (v_prime_sum + 1) for _ in range(n + 1)]
    dp[0][0] = 0

    # fill DP table
    for i in range(1, n + 1):
        vi_prime = v_prime[i - 1]
        wi = weights[i - 1]
        for v in range(v_prime_sum + 1):
            # do not take item i
            dp[i][v] = dp[i-1][v]
            # take item i if possible
            if v >= vi_prime and dp[i-1][v - vi_prime] + wi <= capacity:
                dp[i][v] = min(dp[i][v], dp[i-1][v - vi_prime] + wi)

    # find maximum rounded value
    max_v_prime = max(v for v in range(v_prime_sum + 1) if dp[n][v] <= capacity)
    approx_value = mu * max_v_prime

    return approx_value