#include "exp.h"

#include <iomanip>
#include <iostream>

constexpr double min = -50;
constexpr double max = 50;
constexpr double step = 0.001;
constexpr auto max_precision{std::numeric_limits<double>::digits10 + 1};

int main()
{
    std::cout << "x_float,exp_float,exp_fast_float,exp_very_fast_float,"
                 "x_double,exp_double,exp_fast_double,exp_very_fast_double\n";
    for (double x = min; x < max; x += step) {
        float fx = x;
        std::cout << fx << ',' << std::exp(fx) << ',' << fast_exp(fx) << ','
                  << very_fast_exp(fx) << ',';
        std::cout << std::setprecision(max_precision) << x << ',' << std::exp(x)
                  << ',' << static_cast<double>(fast_exp(x)) << ','
                  << static_cast<double>(very_fast_exp(x));
        std::cout << '\n';
    }
    std::cout << std::flush;
    return 0;
}
