#include <generator>

#include <benchmark/benchmark.h>

void generate_even(std::size_t limit, auto const& callback)
{
#pragma GCC novector
#pragma GCC unroll 1
    for (std::size_t i = 0; i < limit; i += 2) {
        callback(i);
    }
}

std::generator<std::size_t> generate_even(std::size_t limit)
{
#pragma GCC novector
#pragma GCC unroll 1
    for (std::size_t i = 0; i < limit; i += 2) {
        co_yield i;
    }
}

auto const N = 1000;

void BM_even_callback(benchmark::State& state)
{
    std::size_t count = 0;
    for (auto _ : state) {
        generate_even(N, [&](std::size_t x) {
            benchmark::DoNotOptimize(x);
            count++;
        });
    }
    state.SetItemsProcessed(count);
}
BENCHMARK(BM_even_callback);

void BM_even_coro(benchmark::State& state)
{
    std::size_t count = 0;
    for (auto _ : state) {
        for (std::size_t x : generate_even(N)) {
            benchmark::DoNotOptimize(x);
            count++;
        }
    }
    state.SetItemsProcessed(count);
}
BENCHMARK(BM_even_coro);
