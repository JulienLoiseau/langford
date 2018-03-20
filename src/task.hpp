//
// Created by Julien Loiseau on 3/19/18.
//

#ifndef LANGFORD_GODFREY_TASK_HPP
#define LANGFORD_GODFREY_TASK_HPP

#include <cassert>

#include "big_integers.hpp"
#include "initial_data.hpp"
#include "gray.h"

template<
        typename T,
        size_t D
>
class task{
    using big_int = big_integer<T,D>;

private:
    int sign_;
    int evaluation_[nb_cubes+1];
    int terms_[nb_colors+1];
    big_int product_;
    big_int sumTask_;
    //big_int sumProduct_;

    gray g_;

public:

    explicit task(
            long long num_task = 0LL)
    {
        /* Set x1 = x2 = 1 */
        for(int i = 1; i <= 2 ; ++i){
            evaluation_[i] = 1;
        }

        /* Init the member of the tasks */
        init_task(num_task);

        /* Init the terms of this task */
        init_terms();

        /* Reset gray */
        g_.reset();
        std::cout << 0 << "=" << g_.get_current() << ": " << *this << std::endl;

    }

    ~task() = default;


    void
    init_task(
            long long num_task )
    {
        /* Others values */
        for (int i = 3; i < gray_shift; ++i) {
            evaluation_[i] = (num_task % 2 == 1) ? -1 : 1;
            num_task >>= 1;
        }
        /* For outside the grey shift */
        for (int i = 3 + gray_shift; i <= nb_cubes; ++i) {
            evaluation_[i] = 1;
        }
        /* Reset the big_integers */
        product_.reset();
        sumTask_.reset();
        //sumProduct_.reset();
    }

    /**
     * @Brief solve a task iterating over gray sequence
     */
    void compute()
    {
        for(long long i = 0 ; i < total_tasks-1 ; ++i){
            evolve_evaluation(g_.nextbit2flip()+3);
            std::cout << i << "=" << g_.get_current() << ": " << *this << std::endl;

            /* Change the evaluation */
        }
    }

    big_int get_result() const
    {
        return sumTask_;
    }

    /**
     * @brief Change the current evaluation, index target by gray term
     * @param index The index to change in current evaluation
     */
    void evolve_evaluation(
            int index)
    {

        bool must_multiply = true;
        int max1, max2;

        max2 = nb_colors;

        evaluation_[index] *= -1;

        /* First term
         * x1.x3 + ... + x(n-1).x(n+1) */
        if(index <= nb_colors - 1){
            terms_[1] += (2-nb_colors % 2) * evaluation_[index] *
                    (evaluation_[index-2] + evaluation_[index+2]);
        }else{
            if(index <= nb_colors + 1){
                terms_[1] += (2 - nb_colors % 2) *
                        evaluation_[index-2]*evaluation_[index];
            }
        }
        if(nb_colors % 2){
            must_multiply = terms_[1] != 0;
        }

        if(index <= nb_colors) {
            max1 = index - 2;
            if (index >= nb_colors - 1) {
                max2 = 2 * nb_colors - index - 1;
            }

            /* Other terms */
            /* Even indices */
            for (int i = 2; i <= max1; i += 2) {
                terms_[i] += 2 * evaluation_[index] *
                            (evaluation_[index - i - 1] +
                             evaluation_[index + i + 1]);
            }
            for (int i = max1 + 1; i <= max2; i += 2) {
                terms_[i] += 2 *
                        evaluation_[index] * evaluation_[index + i + 1];
            }
            /* Odd indices */
            for (int i = 3; i <= max1; i += 2) {
                terms_[i] += evaluation_[index] *
                            (evaluation_[index - i - 1] +
                             evaluation_[index + i + 1]);
                must_multiply = must_multiply && (terms_[i] != 0);
            }
            for (int i = max1 + 1; i <= max2; i += 2) {
                terms_[i] += evaluation_[index] * evaluation_[index + i + 1];
                must_multiply = must_multiply && (terms_[i] != 0);
            }

            for (int i = max2 + 1; i <= nb_colors; i += 2) {
                must_multiply = must_multiply && (terms_[i] != 0);
            }

        }else{ /* index >  nb_colors */
            max1 = 2 * nb_colors - index - 1;
            if(index <= nb_colors + 2){
                max2 = index -2 ;
            }else if(index >= 2 * nb_colors - 1){
                max1 = 1;
            }

            /* Other terms */
            /* Even terms */
            for(int i = 2; i <= max1; i+= 2){
                terms_[i] += 2 * evaluation_[index] *
                        (evaluation_[index-i-1] + evaluation_[index +i + 1]);
            }
            for(int i = max1+1; i <= max2; i += 2){
                terms_[i] += 2*evaluation_[index-i-1]*evaluation_[index];
            }
            /* Odd terms */
            for (int i = 3; i <= max1; i += 2) {
                terms_[i] += evaluation_[index] *
                            (evaluation_[index - i - 1] +
                             evaluation_[index + i + 1]);
                must_multiply = must_multiply && (terms_[i] != 0);
            }
            for (int i = max1 + 1; i <= max2; i += 2) {
                terms_[i] += evaluation_[index-i-1]
                             * evaluation_[index];
                must_multiply = must_multiply && (terms_[i] != 0);
            }

            for (int i = max2 + 1; i <= nb_colors; i += 2) {
                must_multiply = must_multiply && (terms_[i] != 0);
            }
        }

        /* Product with the sign and adding */
        sign_ = -sign_;
        if(must_multiply){
            product_.init_unit(sign_);
            for(int i = 1; i <= nb_colors; ++i){
                product_.mul_big_integer(terms_[i]);
            }
            product_.propagate_carries();
            sumTask_.add_big_integer(product_);
        }
    }

private:

    void init_terms()
    {
        bool must_multiply;

        /* First term, we stop in middle for symmetry */
        terms_[1] = 0;
        for(int i = 1; i < nb_colors - 1; ++i){
            terms_[1] += evaluation_[i] * evaluation_[i+2];
        }
        if(nb_colors % 2){
            terms_[1] >>= 1;
            must_multiply = (terms_[1] != 0);
        }else{
            must_multiply = true;
        }

        /* Next terms, continue all over */
        for(int i = 2; i <= nb_colors; ++i){
            terms_[i] = 0;
            for(int j = 1; j <= nb_cubes - i - 1; ++j){
                terms_[i] += evaluation_[j] * evaluation_[j+i+1];
            }
            if(i%2){
                terms_[i] >>= 1;
                must_multiply = must_multiply && (terms_[i] != 0);
            }
        }

        /* Handle the sign */
        sign_ = 0;
        for(int i = 3; i < gray_shift; ++i){
            if(evaluation_[i] < 0){
                sign_++;
            }
        }
        sign_ = (sign_ % 2 == 0) ? 1 : -1;

        /* Init the sum */
        if(must_multiply){
            sumTask_.init_unit(sign_);
            for(int i = 1; i <= nb_colors ; ++i){
                sumTask_.mul_big_integer(terms_[i]);
            }
        }else{
            sumTask_.reset();
        }
    };

    friend std::ostream& operator<<(
            std::ostream& os, const task& ts)
    {
        os << " evaluation [ ";
        for(auto v: ts.evaluation_){
            if(v == -1) {
                os << "0 ";
            }else{
                os << v << " ";
            }
        }
        os << " ] ";
        os << " terms [ ";
        for(auto v: ts.terms_){
            os << v << " ";
        }
        os << " ] ";
        os << " prod="<<ts.sumTask_;
        return os;
    }

};

#endif //LANGFORD_GODFREY_TASK_HPP
