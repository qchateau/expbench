#include <generator>
#include <memory_resource>
#include <random>
#include <vector>

#include <benchmark/benchmark.h>

auto const seed = 42;

struct Node {
    std::vector<std::unique_ptr<Node>> children;

    std::generator<Node*> visit_naive()
    {
        co_yield this;
        for (auto& child : children) {
            for (auto desc : child->visit()) {
                co_yield desc;
            }
        }
    }

    std::generator<Node*> visit()
    {
        co_yield this;
        for (auto& child : children) {
            co_yield std::ranges::elements_of(child->visit());
        }
    }

    void visit(auto const& callback)
    {
        callback(this);
        for (auto& child : children) {
            child->visit(callback);
        }
    }
};

Node make_tree(std::size_t n)
{
    std::mt19937 gen(seed);

    Node root;
    std::vector<Node*> nodes;
    nodes.reserve(n);
    nodes.push_back(&root);

    for (auto i = 0ul; i < n; ++i) {
        auto new_node = std::make_unique<Node>();
        std::uniform_int_distribution<std::size_t> dis(0, nodes.size() - 1);
        auto parent_index = dis(gen);
        nodes.at(parent_index)->children.push_back(std::move(new_node));
        nodes.push_back(nodes[parent_index]->children.back().get());
    }

    return root;
}

auto const n_nodes = 1000;

void BM_tree_callback(benchmark::State& state)
{
    auto tree = make_tree(n_nodes);
    std::size_t count = 0;
    for (auto _ : state) {
        tree.visit([&](Node* n) {
            benchmark::DoNotOptimize(n);
            count++;
        });
    }
    state.SetItemsProcessed(count);
}
BENCHMARK(BM_tree_callback);

void BM_tree_coro_naive(benchmark::State& state)
{
    auto tree = make_tree(n_nodes);
    std::size_t count = 0;
    for (auto _ : state) {
        for (Node* n : tree.visit_naive()) {
            benchmark::DoNotOptimize(n);
            count++;
        }
    }
    state.SetItemsProcessed(count);
}
BENCHMARK(BM_tree_coro_naive);

void BM_tree_coro(benchmark::State& state)
{
    auto tree = make_tree(n_nodes);
    std::size_t count = 0;
    for (auto _ : state) {
        for (Node* n : tree.visit()) {
            benchmark::DoNotOptimize(n);
            count++;
        }
    }
    state.SetItemsProcessed(count);
}
BENCHMARK(BM_tree_coro);
