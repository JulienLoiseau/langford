//
// Created by Julien Loiseau on 3/18/18.
//


#include "gtest/gtest.h"

#include "big_integers.hpp"

namespace {
    TEST(big_integer,info){
        big_integer<int32_t,6> a;
        big_integer<int64_t,6> b;
        big_integer<int16_t,6> c;

        a.info();
        b.info();
        c.info();
    }

    TEST(big_integer, general) {
        big_integer<int64_t,6> a;
        big_integer<int64_t,6> b;

        b.init_unit(4);
        a.reset();
        a.init_unit(2);

        a.mul_big_integer(5);
        a.division(1);
        a.propagate_carries();
        a.propagate_carries_sign();
        a.add_big_integer(b);

        std::ostringstream output;
        output << a;

        EXPECT_EQ(output.str(),"9");
    }


    TEST(big_integer, multiplication) {
        big_integer<int64_t,6> a;
        a.init_unit(1);

        const int pow_calc = 38;

        for(int i = 0; i < pow_calc ; ++i){
                a.mul_big_integer(2);
        }

        std::ostringstream output;
        std::ostringstream result;
        result << (1LL<<pow_calc) ;
        output << a;
        EXPECT_EQ(output.str(),result.str());
    }

    TEST(big_integer, addition){
        big_integer<int16_t,6> a;
        big_integer<int16_t,6> b;
        a.init_unit(0);
        b.init_unit(1);

        const int nadd = 1.0e4;

        for(int j = 0; j < nadd; ++j){
            for(int i = 0; i < nadd ; ++i) {
                a.add_big_integer(b);
            }
        }

        std::ostringstream output;
        std::ostringstream result;
        result << (nadd * nadd);
        output << a;
        EXPECT_EQ(output.str(), result.str());
    }
}