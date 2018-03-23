#include <iostream>
//#include <omp.h>

#include "instance.hpp"

int main() {
#pragma omp parallel
#pragma omp single
//    std::cout << omp_get_num_threads() << std::endl;

    instance<8, long long> t;
    auto res = t.solve();
    std::cout << res << std::endl;
    return 0;
}