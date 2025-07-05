import heapq

class Item:
    __slots__ = ('value', 'weight', 'ratio')
    def __init__(self, value, weight):
        self.value = value
        self.weight = weight
        self.ratio = value / weight

def bound(level, value, weight, capacity, items, n):
    if weight >= capacity:
        return 0

    i = level + 1
    if i >= n:
        return value

    # bound = v + (W - w) * (v_{i+1} / w_{i+1})
    return value + (capacity - weight) * items[i].ratio

class Node:
    __slots__ = ('level', 'value', 'weight', 'bound')
    def __init__(self, level, value, weight):
        self.level = level      # current index in items
        self.value = value      # current total value
        self.weight = weight    # current total weight
        self.bound = 0

    def __lt__(self, other):
        return self.bound > other.bound  # max-heap by bound

def knapsack(instance):
    n, capacity, item_list = instance
    items = sorted([Item(v, w) for v, w in item_list], key=lambda x: x.ratio, reverse=True)

    queue = []
    root = Node(-1, 0, 0)
    root.bound = bound(root.level, root.value, root.weight, capacity, items, n)
    max_value = 0

    heapq.heappush(queue, root)

    while queue:
        node = heapq.heappop(queue)

        if node.bound <= max_value or node.level == n - 1:
            continue

        # branch: include next item
        next_level = node.level + 1
        new_value = node.value + items[next_level].value
        new_weight = node.weight + items[next_level].weight

        if new_weight <= capacity:
            if new_value > max_value:
                max_value = new_value

            temp_bound = bound(next_level, new_value, new_weight, capacity, items, n)

            if temp_bound > max_value:
                include = Node(next_level, new_value, new_weight)
                include.bound = temp_bound
                heapq.heappush(queue, include)

        # branch: exclude next item
        temp_bound = bound(next_level, node.value, node.weight, capacity, items, n)
        if temp_bound > max_value:
            exclude = Node(next_level, node.value, node.weight)
            exclude.bound = temp_bound
            heapq.heappush(queue, exclude)

    return max_value