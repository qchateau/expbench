#include <random>
#include <benchmark/benchmark.h>

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

constexpr int K_TUNED_MINIMAX = 366393; // Max relative error: 2.98%
constexpr int K_TUNED_L1 = 545948; // Mean relative error: 1.48%
constexpr int K_TUNED_L2 = 486412; // Root mean squared relative error: 1.77%

// See https://specbranch.com/posts/fast-exp/
constexpr float very_fast_exp(float x, int K = K_TUNED_MINIMAX)
{
    float x_base = x * 12102203;
    int i_base = x_base;
    int i_result = i_base + (127 << 23) - K;
    return std::bit_cast<float>(i_result);
}

constexpr float P0 = 1.469318866729736328125;
constexpr float P1 = -0.671999752521514892578125;
constexpr float P2 = 0.22670517861843109130859375;

// See https://specbranch.com/posts/fast-exp/
constexpr float fast_exp(float x, int K = K_TUNED_MINIMAX)
{
    float x_base = x * 12102203;
    int i_base = x_base;
    int i_result = i_base + (127 << 23) - K;
    float first_order = std::bit_cast<float>(i_result);

    int correction_xi = (i_result & 0x7fffff) | (127 << 23);
    float correction_x = std::bit_cast<float>(correction_xi);

    float correction = std::fmaf(correction_x, P2, P1);
    correction = std::fmaf(correction, correction_x, P0);
    return first_order * correction;
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
