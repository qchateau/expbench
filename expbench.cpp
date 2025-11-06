#include <random>
#include <benchmark/benchmark.h>

#include "exp.h"

constexpr auto seed = 42;

template <std::floating_point T>
std::vector<T> make_data(std::size_t size, double min = -50, double max = 50)
{
    std::default_random_engine eng{seed};
    std::uniform_real_distribution<double> dis(min, max);
    std::vector<T> data(size);
    for (auto& x : data)
        x = dis(eng);
    return data;
}

template <typename T>
void BM_exp(benchmark::State& state, auto exp)
{
    auto width = state.range(0);
    auto data = make_data<T>(width);
    for (auto _ : state) {
        for (auto& x : data) {
            benchmark::DoNotOptimize(exp(x));
        }
    }
    state.SetItemsProcessed(state.iterations() * data.size());
}

void BM_exp_float(benchmark::State& state)
{
    BM_exp<float>(state, [](auto x) { return std::exp(x); });
}
BENCHMARK(BM_exp_float)->Range(1, 4096)->RangeMultiplier(8);

void BM_exp_fast_float(benchmark::State& state)
{
    BM_exp<float>(state, [](auto x) { return fast_exp(x); });
}
BENCHMARK(BM_exp_fast_float)->Range(1, 4096)->RangeMultiplier(8);

void BM_exp_very_fast_float(benchmark::State& state)
{
    BM_exp<float>(state, [](auto x) { return very_fast_exp(x); });
}
BENCHMARK(BM_exp_very_fast_float)->Range(1, 4096)->RangeMultiplier(8);

void BM_exp_double(benchmark::State& state)
{
    BM_exp<double>(state, [](auto x) { return std::exp(x); });
}
BENCHMARK(BM_exp_double)->Range(1, 4096)->RangeMultiplier(8);

void BM_exp_float_double(benchmark::State& state)
{
    BM_exp<double>(state, [](auto x) {
        return static_cast<double>(std::exp(static_cast<float>(x)));
    });
}
BENCHMARK(BM_exp_float_double)->Range(1, 4096)->RangeMultiplier(8);

void BM_exp_fast_double(benchmark::State& state)
{
    BM_exp<double>(state, [](auto x) { return fast_exp(x); });
}
BENCHMARK(BM_exp_fast_double)->Range(1, 4096)->RangeMultiplier(8);

void BM_exp_very_fast_double(benchmark::State& state)
{
    BM_exp<double>(state, [](auto x) { return very_fast_exp(x); });
}
BENCHMARK(BM_exp_very_fast_double)->Range(1, 4096)->RangeMultiplier(8);

BENCHMARK_MAIN();
