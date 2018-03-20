//
// Created by Julien Loiseau on 3/19/18.
//


#include "gtest/gtest.h"

#include "instance.hpp"

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
    TEST(result,lgf3){
        instance<3,long long> t;
        auto res = t.solve();

        std::ostringstream output;
        std::ostringstream result;
        result << 1 ;
        output << res;
        EXPECT_EQ(output.str(),result.str());
    }

    TEST(result,lgf4){
        instance<4,long long> t;
        auto res = t.solve();

        std::ostringstream output;
        std::ostringstream result;
        result << 1 ;
        output << res;
        EXPECT_EQ(output.str(),result.str());
    }

    TEST(result,lgf7){
        instance<7,long long> t;
        auto res = t.solve();

        std::ostringstream output;
        std::ostringstream result;
        result << 26 ;
        output << res;
        EXPECT_EQ(output.str(),result.str());
    }

    TEST(result,lgf8){
        instance<8,long long> t;
        auto res = t.solve();

        std::ostringstream output;
        std::ostringstream result;
        result << 150 ;
        output << res;
        EXPECT_EQ(output.str(),result.str());
    }

    TEST(result,lgf11){
        instance<11,long long> t;
        auto res = t.solve();

        std::ostringstream output;
        std::ostringstream result;
        result << 17792 ;
        output << res;
        EXPECT_EQ(output.str(),result.str());
    }

    TEST(result,lgf12){
        instance<12,long long> t;
        auto res = t.solve();

        std::ostringstream output;
        std::ostringstream result;
        result << 108144 ;
        output << res;
        EXPECT_EQ(output.str(),result.str());
    }
}