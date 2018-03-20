#include <iostream>

#include "instance.hpp"

int main() {
    instance<8, long long> t;
    auto res = t.solve();
    std::cout << res << std::endl;
    return 0;
}