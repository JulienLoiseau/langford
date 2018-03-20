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

    /**
     * Create a gray sequence init to 0 by default
     */
    gray():current_(0LL){}

    ~gray() = default;

    /**
     * @brief Reset the current gray sequence
     */
    void
    reset()
    {
        current_ = 0LL;
    }

    /**
     * @brief Get the next bit changed in the sequence
     * Base on the search of XOR between current and next value
     * then __builtin_clzll to get the first 1 bit
     * @return  The next bit change position
     */
    int
    nextbit2flip()
    {
        return nbits-1-__builtin_clzll( BRGC(current_) ^ BRGC(current_++ +1) );
    }

    /**
     * @brief Get the current bit position value
     * @todo Store the value in the class
     * @return The current bit changed
     */
    const int
    get_current() const
    {
        return nbits-1-__builtin_clzll( BRGC(current_) ^ BRGC(current_ +1) );
    }

private:
    type_t current_;

    /**
     * Transfer from integer to gray code
     * @param n The integer value
     * @return The gray code value
     */
    const type_t
    BRGC(
            const type_t& n) const
    {
        return n ^ n>>1;
    }

};

#endif //LANGFORD_GODFREY_GRAY_H
