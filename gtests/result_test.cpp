//
// Created by Julien Loiseau on 3/19/18.
//


#include "gtest/gtest.h"

#include "big_integers.hpp"

/**
 * 3    1
 * 4    1
 * 5 6
 * 7    26
 * 8    150
 * 9 10
 * 11   11792
 * 12   108144
 */

namespace {
    TEST(result,lgf8){
        big_integer<int32_t,6> a;
        big_integer<int64_t,6> b;
        big_integer<int16_t,6> c;

        a.info();
        b.info();
        c.info();
    }
}