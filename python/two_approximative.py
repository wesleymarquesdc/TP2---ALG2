def knapsack_2approx(instance):
    n, capacity, item_list = instance
    values = [item[0] for item in item_list]
    weights = [item[1] for item in item_list]
    
    if n == 0 or capacity <= 0:
        return 0
    
    # Candidato 1: Maior item individual que cabe na mochila
    candidate1 = 0
    for i in range(n):
        if weights[i] <= capacity and values[i] > candidate1:
            candidate1 = values[i]
    
    # Candidato 2: Solução gulosa por densidade (valor/peso)
    items = []
    for i in range(n):
        if weights[i] > 0:
            density = values[i] / weights[i]
        else:
            density = float('inf')
        items.append((density, values[i], weights[i]))
    
    items.sort(key=lambda x: x[0], reverse=True)
    
    current_value = 0
    current_weight = 0
    for _, val, weight in items:
        if current_weight + weight <= capacity:
            current_value += val
            current_weight += weight
    candidate2 = current_value
    
    return max(candidate1, candidate2)
