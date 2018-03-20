//
// Created by Julien Loiseau on 3/19/18.
//

#ifndef LANGFORD_GODFREY_TASK_HPP
#define LANGFORD_GODFREY_TASK_HPP

#include <cassert>

#include "big_integers.hpp"
#include "gray.hpp"

template<
        typename T,
        size_t D,
        size_t NBC,
        size_t TS
>
class task{
    using big_int = big_integer<T,D>;
    const static size_t nb_colors = NBC;
    const static size_t nb_cubes = nb_colors*2;
    const static size_t size_task = TS -2; // All tasks = all bits - 2 for x1 x2

    const static int64_t total_tasks = 1LL << size_task;
    const static size_t gray_shift = size_task + 2;
    //const size_t gray_shift = size_task + 2;

#ifdef CUDA
    const size_t size_gray_gou = 20;
        const int64_t gray_gpu = (1LL << size_gray) + 1;
#endif

private:
    int sign_;
    int evaluation_[nb_cubes];
    int terms_[nb_colors];
    big_int product_;
    big_int sumTask_;
    //big_int sumProduct_;

    gray g_;

public:

    explicit task(
            long long num_task = 0LL)
    {

        /* Set x1 = x2 = 1 */
        evaluation_[0] = 1;
        evaluation_[1] = 1;

        /* Init the member of the tasks */
        init_task(num_task);

        /* Init the terms of this task */
        init_terms();

        /* Reset gray */
        g_.reset();
        //std::cout << 0 << "=" << g_.get_current() << ": " << *this << std::endl;

    }

    ~task() = default;


    void
    init_task(
            long long num_task )
    {
        /* Others values */
        for (int i = 2; i < gray_shift; ++i) {
            evaluation_[i] = (num_task % 2 == 1) ? -1 : 1;
            num_task >>= 1;
        }

        /* For outside the gray shift */
        for (int i = 2 + gray_shift; i < nb_cubes; ++i) {
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
            evolve_evaluation(g_.nextbit2flip()+2);
            //std::cout << i << "=" << g_.get_current() << ": " << *this << std::endl;

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

        //std::cout<<"EVAL: index="<<index<<std::endl;
        /* The index cannot addresses the two first positions due to symmetry
         * optimization */
        assert(index > 1);

        bool must_multiply = true;
        int max1, max2;

        max2 = nb_colors;

        evaluation_[index] *= -1;


#if 0
        /* First term
         * x1.x3 + ... + x(n-1).x(n+1) */
        if(index < nb_colors - 2){
            terms_[0] += (2-nb_colors % 2) * evaluation_[index] *
                    (evaluation_[index-2] + evaluation_[index+2]);
            std::cout<<"AA term["<<0<<"]="<<"("<<index-2<<"-"<<index<<") && ("<<index<<"-"<<index+2<<")"<<std::endl;
        }else{
            /* Do not consider the right side for symmetry */
            if(index < nb_colors){
                terms_[0] += (2 - nb_colors % 2) *
                        evaluation_[index-2]*evaluation_[index];
                std::cout<<"BB term["<<0<<"]="<<"("<<index-2<<"-"<<index<<")"<<std::endl;
            }
        }
        if(nb_colors % 2 == 0){
            must_multiply = terms_[0] != 0;
        }
#endif

        /* Even terms */
        for(int i = 0; i < nb_colors; i+=1){
            int min = index - i - 2;
            int max = index + i + 2;
            if(min > -1){
                terms_[i] += 2 * evaluation_[index] * evaluation_[min];
                //std::cout<<"EMin term["<<i<<"]="<<"("<<min<<"-"<<index<<")"<<std::endl;
            }
            if(max < nb_cubes){
                terms_[i] += 2 * evaluation_[index] * evaluation_[max];
                //std::cout<<"EMax term["<<i<<"]="<<"("<<index<<"-"<<max<<")"<<std::endl;

            }
            must_multiply = must_multiply && (terms_[i] != 0);
        }

#if 0
        /* Odd terms */
        for(int i = 1; i < nb_colors; i+=2){
            int min = index - i - 2;
            int max = index + i + 2;
            if(min > -1){
                terms_[i] += evaluation_[index] * evaluation_[min];
                std::cout<<"OMin term["<<i<<"]="<<"("<<min<<"-"<<index<<")"<<std::endl;

            }
            if(max < nb_cubes){
                terms_[i] += evaluation_[index] * evaluation_[max];
                std::cout<<"OMax term["<<i<<"]="<<"("<<index<<"-"<<max<<")"<<std::endl;
            }
            must_multiply = must_multiply && (terms_[i] != 0);
        }
#endif

#if 0

        /* If we are in the low part of positions */
        /* nb_colors = 4
         * nb_cubes = 8
        /* evaluation =  0 1 2 3 4 5 6 7
        /* terms =       0 1 2 3 */
        if(index < nb_colors) {
            max1 = index - 2;
            if (index >= nb_colors - 2) {
                max2 = 2 * nb_colors - index - 3;
            }

            std::cout<<"index<nb_colors"<<std::endl<<"max1="<<max1<<" max2="<<max2<<std::endl;

            /* Other terms */
            /* Even pairs */
            /* Case 1: Left and Right */
            for (int i = 1; i <= max1; i += 2) {
                terms_[i] += 2 * evaluation_[index] *
                            (evaluation_[index - i - 2] +
                             evaluation_[index + i + 2]);
                std::cout<<"A term["<<i<<"]="<<"("<<index-i-2<<"-"<<index<<") && ("<<index<<"-"<<index+i+2<<")"<<std::endl;
            }
            for (int i = max1 + 1; i <= max2; i += 2) {
                terms_[i] += 2 *
                        evaluation_[index] * evaluation_[index + i + 2];
                std::cout<<"B term["<<i<<"]="<<index<<"-"<<index+i+2<<")"<<std::endl;
            }

            /* Odd pairs */
            for (int i = 2; i <= max1; i += 2) {
                terms_[i] += evaluation_[index] *
                            (evaluation_[index - i - 2] +
                             evaluation_[index + i + 2]);
                must_multiply = must_multiply && (terms_[i] != 0);
                std::cout<<"C term["<<i<<"]="<<"("<<index-i-2<<"-"<<index<<") && ("<<index<<"-"<<index+i+2<<")"<<std::endl;
            }
            for (int i = max1 + 2; i <= max2; i += 2) {
                terms_[i] += evaluation_[index] * evaluation_[index + i + 2];
                std::cout<<"D term["<<i<<"]="<<index<<"-"<<index+i+2<<")"<<std::endl;
                must_multiply = must_multiply && (terms_[i] != 0);
            }

            for (int i = max2 + 1; i <= nb_colors; i += 2) {
                must_multiply = must_multiply && (terms_[i] != 0);
            }

        }else{ /* index >=  nb_colors */
            max1 = nb_cubes - index - 3;
            if(index <= nb_colors + 2){
                max2 = index - 3;
            }else if(index >= 2 * nb_colors - 1){
                max1 = 0;
            }

            std::cout<<"index>nb_colors"<<std::endl<<"max1="<<max1<<" max2="<<max2<<std::endl;

            /* Other terms */
            /* Even terms */
            for(int i = 1; i <= max1; i+= 2){
                terms_[i] += 2 * evaluation_[index] *
                        (evaluation_[index-i-1] + evaluation_[index +i + 2]);
                std::cout<<"A term["<<i<<"]="<<"("<<index-i-2<<"-"<<index<<") && ("<<index<<"-"<<index+i+2<<")"<<std::endl;

            }
            for(int i = max1 + 1; i <= max2; i += 2){
                terms_[i] += 2*evaluation_[index-i-2]*evaluation_[index];
                std::cout<<"B term["<<i<<"]="<<"("<<index-i-2<<"-"<<index<<")"<<std::endl;

            }
            /* Odd terms */
            for (int i = 2; i <= max1; i += 2) {
                terms_[i] += evaluation_[index] *
                            (evaluation_[index - i - 2] +
                             evaluation_[index + i + 2]);
                must_multiply = must_multiply && (terms_[i] != 0);
                std::cout<<"C term["<<i<<"]="<<"("<<index-i-2<<"-"<<index<<") && ("<<index<<"-"<<index+i+2<<")"<<std::endl;

            }
            for (int i = max1 + 2; i <= max2; i += 2) {
                terms_[i] += evaluation_[index-i-2]
                             * evaluation_[index];
                must_multiply = must_multiply && (terms_[i] != 0);
                std::cout<<"D term["<<i<<"]="<<"("<<index-i-2<<"-"<<index<<")"<<std::endl;

            }

            for (int i = max2 + 1; i <= nb_colors; i += 2) {
                must_multiply = must_multiply && (terms_[i] != 0);
            }
        }

#endif

        /* Product with the sign and adding */
        sign_ = -sign_;
        if(must_multiply){
            product_.init_unit(sign_);
            for(int i = 0; i < nb_colors; ++i){
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
        terms_[0] = 0;
        for(int i = 0; i < nb_colors - 2; ++i){
            terms_[0] += evaluation_[i] * evaluation_[i+2];
        }
        //if(nb_colors % 2 == 0){
        //    terms_[0] >>= 1;
        //    must_multiply = (terms_[0] != 0);
        //}else{
            must_multiply = true;
        //}

        /* Next terms, continue all over */
        for(int i = 1; i < nb_colors; ++i){
            terms_[i] = 0;
            for(int j = 0; j < nb_cubes - i - 2; ++j){
                terms_[i] += evaluation_[j] * evaluation_[j+i+2];
            }
            //if(i%2 == 0){
            //    terms_[i] >>= 1;
            //    must_multiply = must_multiply && (terms_[i] != 0);
            //}
        }

        /* Handle the sign */
        sign_ = 0;
        for(int i = 2; i < gray_shift-1; ++i){
            if(evaluation_[i] < 0){
                sign_++;
            }
        }
        sign_ = (sign_ % 2 == 0) ? 1 : -1;

        /* Init the sum */
        if(must_multiply){
            sumTask_.init_unit(sign_);
            for(int i = 0; i < nb_colors ; ++i){
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
        os << " sign="<<ts.sign_;
        return os;
    }

};

#endif //LANGFORD_GODFREY_TASK_HPP
