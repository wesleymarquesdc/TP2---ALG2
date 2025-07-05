#include <vector>
#include <queue>
#include <algorithm>

#include "../include/branch_and_bound.h"

namespace branch_and_bound {

    struct Item {
        int value;
        int weight;
        double ratio;

        Item(int v, int w) : value(v), weight(w), ratio(static_cast<double>(v) / w) {}
    };

    double bound(int level, int value, int weight, int capacity, const std::vector<Item>& items, int n) {
        if (weight >= capacity) {
            return 0.0;
        }

        int i = level + 1;
        if (i >= n) {
            return static_cast<double>(value);
        }

        // bound = v + (W - w) * (v_{i+1} / w_{i+1})
        return value + (capacity - weight) * items[i].ratio;
    }

    struct Node {
        int level;      // current index in items
        int value;      // current total value
        int weight;     // current total weight
        double bound;

        Node(int lvl, int val, int wt) : level(lvl), value(val), weight(wt), bound(0.0) {}
    };

    // comparator for max-heap by bound
    struct CompareNode {
        bool operator()(const Node& a, const Node& b) {
            return a.bound < b.bound;  // max-heap by bound
        }
    };

    int knapsack(const Instance& instance) {
        int n = instance.n;
        int capacity = instance.wmax;
        std::vector<Item> items;
        for (int i = 0; i < n; ++i) {
            items.emplace_back(instance.values[i], instance.weights[i]);
        }

        // sorted by ratio descending
        std::sort(items.begin(), items.end(), [](const Item& a, const Item& b) {
            return a.ratio > b.ratio;
        });

        std::priority_queue<Node, std::vector<Node>, CompareNode> queue;
        Node root(-1, 0, 0);
        root.bound = bound(root.level, root.value, root.weight, capacity, items, n);
        int max_value = 0;

        queue.push(root);

        while (!queue.empty()) {
            Node node = queue.top();
            queue.pop();

            if (node.bound <= max_value || node.level == n - 1) {
                continue;
            }

            // branch: include next item
            int next_level = node.level + 1;
            int new_value = node.value + items[next_level].value;
            int new_weight = node.weight + items[next_level].weight;

            if (new_weight <= capacity) {
                if (new_value > max_value) {
                    max_value = new_value;
                }

                double temp_bound = bound(next_level, new_value, new_weight, capacity, items, n);

                if (temp_bound > max_value) {
                    Node include(next_level, new_value, new_weight);
                    include.bound = temp_bound;
                    queue.push(include);
                }
            }

            // branch: exclude next item
            double temp_bound = bound(next_level, node.value, node.weight, capacity, items, n);
            if (temp_bound > max_value) {
                Node exclude(next_level, node.value, node.weight);
                exclude.bound = temp_bound;
                queue.push(exclude);
            }
        }

        return max_value;
    }

}