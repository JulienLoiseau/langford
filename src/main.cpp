#include <iostream>

#include "big_integers.hpp"
#include "task.hpp"

int main(int argc, char* argv[]) {
    task<long long, 6> t;
    t.compute();
    auto res = t.get_result();
    std::cout<<res<<std::endl;
    bool ok = res.division((nb_colors>>1)*3-1);
    assert(ok == true);
    std::cout<<res<<std::endl;
    return 0;
}