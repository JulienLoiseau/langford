//
// Created by Julien Loiseau on 3/20/18.
//

#ifndef LANGFORD_GODFREY_INSTANCE_HPP
#define LANGFORD_GODFREY_INSTANCE_HPP

#include <cmath>
#include <string>

#include "big_integers.hpp"
#include "task.hpp"

template<
        size_t NC,
        typename T
>
class instance {
    using type_t = T;

    const static size_t nb_colors = NC;

    using task_t = task<type_t, 6, nb_colors, nb_colors * 2>;
    using big_int = big_integer<type_t, 6>;

private:
    big_int result;

public:
    instance() = default;

    ~instance() = default;

    big_int &solve() {
        task_t t;
        t.compute();
        result = t.get_result();
        result.division(nb_colors * 2 - 1);
        return result;
    }

};

#endif //LANGFORD_GODFREY_INSTANCE_HPP
