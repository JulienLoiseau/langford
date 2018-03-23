//
// Created by Julien Loiseau on 3/20/18.
//

#ifndef LANGFORD_GODFREY_INSTANCE_HPP
#define LANGFORD_GODFREY_INSTANCE_HPP

#include <cmath>
#include <string>

#include "big_integers.hpp"
#include "task.hpp"

/**
 * @brief Targeting a specific instance of the Langford problem.
 * @tparam NC The Number of Cubes
 * @tparam T The type for the big integers
 */
template<
        size_t NC,
        typename T
>
class instance {
    using type_t = T;

    const static size_t nb_colors = NC;
    const static long long total_tasks = 1LL << (nb_colors*2-4);

    using task_t = task<type_t, 6, nb_colors, nb_colors * 2>;
    using big_int = big_integer<type_t, 6>;

    big_int result;

public:
    instance() = default;

    ~instance() = default;

    /**
     * @brief Compute all the tasks of this instance
     * @return
     */
    big_int &solve() {

#pragma omp parallel for
        for(long long i = 0 ; i < total_tasks; ++i) {
            task_t t(i);
            t.compute();
            // Add up to result
#pragma omp critical
            result.add_big_integer(t.get_result());
        }

        if(nb_colors == 3){
            result.division(nb_colors * 2 - 4);
        }else if(nb_colors == 4){
            result.division(nb_colors * 2 - 3);
        }else if(nb_colors == 7){
            result.division(nb_colors * 2 - 6);
        }else if(nb_colors == 8){
            result.division(nb_colors * 2 - 5);
        }else if(nb_colors == 11){
            result.division(nb_colors * 2 - 8);
        }else if(nb_colors == 12){
            result.division(nb_colors * 2 - 7);
        }
        return result;
    }

};

#endif //LANGFORD_GODFREY_INSTANCE_HPP
