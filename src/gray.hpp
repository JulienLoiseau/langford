//
// Created by Julien Loiseau on 3/19/18.
//

#ifndef LANGFORD_GODFREY_GRAY_H
#define LANGFORD_GODFREY_GRAY_H

#include <cstdlib>
#include <iterator>
#include <cmath>
#include <iostream>
#include <vector>
#include <string>

class gray{

    using type_t = long long;

    const size_t nbits = sizeof(type_t)*8;

public:

    gray():current_(0LL){}

    ~gray() = default;

    void
    reset()
    {
        current_ = 0LL;
    }

    int
    nextbit2flip()
    {
        return nbits-1-__builtin_clzll( BRGC(current_) ^ BRGC(current_++ +1) );
    }

    const int
    get_current() const
    {
        return nbits-1-__builtin_clzll( BRGC(current_) ^ BRGC(current_ +1) );
    }

private:
    type_t current_;

    const type_t
    BRGC(
            const type_t& n) const
    {
        return n ^ n>>1;
    }

};

#endif //LANGFORD_GODFREY_GRAY_H
