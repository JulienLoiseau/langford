#include <iostream>

#include "instance.hpp"

int main(int argc, char *argv[]) {
    instance<8, long long> t;
    auto res = t.solve();
    std::cout << res << std::endl;
    return 0;
}