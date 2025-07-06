import heapq

class Item:
    def __init__(self, value, weight):
        self.value = value
        self.weight = weight
        self.ratio = value / weight

def bound(node, capacity, items, n):
    if node.weight >= capacity:
        return 0

    i = node.level + 1
    if i >= n:
        return node.value

    # bound = v + (W - w) * (v_{i+1} / w_{i+1})
    return node.value + (capacity - node.weight) * items[i].ratio

class Node:
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
    root.bound = bound(root, capacity, items, n)
    max_value = 0

    heapq.heappush(queue, root)

    while queue:
        node = heapq.heappop(queue)

        if node.bound <= max_value or node.level == n - 1:
            continue

        # Branch: include next item
        next_level = node.level + 1
        include = Node(next_level,
                       node.value + items[next_level].value,
                       node.weight + items[next_level].weight)

        if include.weight <= capacity:
            if include.value > max_value:
                max_value = include.value
            include.bound = bound(include, capacity, items, n)
            if include.bound > max_value:
                heapq.heappush(queue, include)

        # Branch: exclude next item
        exclude = Node(next_level, node.value, node.weight)
        exclude.bound = bound(exclude, capacity, items, n)
        if exclude.bound > max_value:
            heapq.heappush(queue, exclude)

    return max_value
