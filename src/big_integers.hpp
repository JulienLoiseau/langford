//
// Created by All_j on 17/03/2018.
//

#ifndef LANGFORD_GODFREY_BIG_INTEGERS_HPP
#define LANGFORD_GODFREY_BIG_INTEGERS_HPP

#include <cstdlib>
#include <iterator>
#include <cmath>
#include <iostream>
#include <vector>
#include <string>


#ifdef CUDA
#define HOST_DEVICE __host__ __device__
#else
#define HOST_DEVICE
#endif

/**
 * @brief Representation of a big integer
 * @tparam T
 * @tparam D
 */
template <
        typename T,
        size_t D
>
class big_integer{
    using type_t = T;

    const static size_t dim = D;
    const static size_t word_size = sizeof(type_t)*8;
    const static size_t size_base = word_size / 2;
    const static long long base = (1LL << size_base);

private:
    type_t array_[dim];

    //size_t used_words_;
    size_t room_per_word_;
    size_t number_words_;
public:

    HOST_DEVICE explicit big_integer():
            number_words_(1),
            room_per_word_(1)
    {
        std::fill(std::begin(array_),std::end(array_),0);
    }

    ~big_integer() = default;

    /**
     * @brief Reset a big integer to zero
     * Also reset counters
     */
    HOST_DEVICE
    void reset()
    {
        number_words_ = 1;
        room_per_word_ = 1;
        std::fill(std::begin(array_),std::end(array_),0);
    }

    /**
     * @brief Reset a long integer to a unit value
     * @param val The value of first word
     */
    HOST_DEVICE
    void init_unit(const type_t& val)
    {
        reset();
        array_[0] = val;
        room_per_word_  += 1;
    }

    /**
     * @brief Check if a big integer is zero
     * @return True if zero
     */
    HOST_DEVICE
    bool is_zero()
    {
        return number_words_ == 1 && array_[0] == 0;
    }


    /**
     * @ Sum up two integers in the current one
     * @param bi The other operand of addition
     */
    HOST_DEVICE
    void add_big_integer(
            const big_integer& bi
    )
    {
        if(number_words_ < bi.number_words_)
        {
            number_words_ = bi.number_words_;
        }
        // Adds up
        for(int i = 0; i < number_words_ ; ++i){
            array_[i] += bi.array_[i];
        }

        while(number_words_ > 1 && array_[number_words_-1] == 0) {
            number_words_--;
        }
        room_per_word_++;

        if(room_per_word_ == word_size){
            propagate_carries();
        }
    }

    /**
     * @brief Multiply big integer by small value
     * Store the result in current big integer
     * @tparam TL Type of the value for multiplication
     * @param val The second operand for multiplication. Keep copy of value
     * for abs.
     */
    HOST_DEVICE
    template <
            typename TL
    >
    void mul_big_integer(TL val){
        if(val==0){
            reset();
        }else{
            for(int i = 0; i < number_words_ ; ++i){
                array_[i] *= val;
            }

            if(fabs(val) < 32){
                room_per_word_ += 5;
            }else if(fabs(val) < 64){
                room_per_word_ += 6;
            }else{
                room_per_word_ += 7;
            }

            if(room_per_word_ >= word_size - 7){
                propagate_carries();
            }
        }
    }

    /**
     * @brief Propagate the carries in the current big integer
     */
    HOST_DEVICE
    void propagate_carries(){
        for(int i = 0; i < dim-1 ; ++i){
            array_[i+1] += (array_[i] / base);
            array_[i] %= base;
        }
        if(number_words_ < dim -1 && array_[number_words_] != 0){
            number_words_++;
        }else{
            while(number_words_>1 && array_[number_words_-1] == 0){
                number_words_--;
            }
        }
        room_per_word_ = size_base;
    }

    /**
     * @brief Propagate the carries and take care of the sign
     */
    HOST_DEVICE
    void propagate_carries_sign(){
        propagate_carries();
        if(array_[number_words_ - 1] > 0){
            for(int i = number_words_ - 2; i >= 0; --i){
                if(array_[i] < 0){
                    array_[i] += base;
                    array_[i+1]--;
                }
            }
        }else{
            for(int i = number_words_ - 2; i >= 0; --i){
                if(array_[i] > 0){
                    array_[i] -= base;
                    array_[i+1]++;
                }
            }
        }

        while(number_words_> 1 && array_[number_words_ - 1] == 0){
            number_words_--;
        }
    }

    /**
     * @brief Divide the big integer by 2^number_bits and store result
     * current object
     * @param number_bits Power of the division by 2
     * @return true if multiple of 2; false otherwise
    */
    HOST_DEVICE
    bool division(int number_bits){
        long long divisor;
        size_t index = number_bits / size_base;

        number_bits %= size_base;

        if(index >= number_words_){
            return(is_zero());
        }

        for(int i = 0; i < index; ++i){
            if(array_[i] != 0){
                return false;
            }
        }

        /* 1 */
        number_words_ -= index;
        for(int i = 0; i < number_words_; ++i){
            array_[i] = array_[i+index];
        }
        for(int i = number_words_; i < number_words_ + index; ++i){
            array_[i] = 0;
        }

        /* 2 */
        divisor = 1LL << number_bits;
        if(array_[0] % divisor != 0){
            return false;
        }

        /* 3 */
        /* 3a */
        for(int i = 0; i < number_words_-1; ++i){
            array_[i] /= divisor;
            array_[i] += ((array_[i+1]%divisor) << (size_base - number_bits));
        }
        /* 3b */
        array_[number_words_ - 1] /= divisor;
        if(array_[number_words_-1] == 0){
            number_words_--;
        }
        return true;
    }

    /**
     * @brief Output the big integer in base 10
     * Convertion is done from BASE to base 1024, base 1000 and then base 10
     * @param os The output stream
     * @param bi The big integer to display
     * @return The output stream
     */
    friend  std::ostream& operator<<(
            std::ostream& os,
            const big_integer& bi
    )
    {
        int nb10 = static_cast<int>(
                ((bi.number_words_ + 1 ) * size_base ) / 10);
        long long* seq10 = new long long[nb10]();
        long long x;
        size_t tmp = 0;

        /* 1: base 2^size_base ==> seq10 base 1024 */
        for(int i = 0; i < bi.number_words_; ++i){
            tmp = (i*size_base)/10;
            x = (bi.array_[i] << (i*size_base)%10);
            while(x!=0){
                seq10[tmp++] += x % 1024;
                x /= 1024;
            }
        }
        nb10 = tmp;

        /* 2: from base 1024 to base 1000 */
        /* 1024 a + b = 1000 a + (24 a + b) */
        for(int i = nb10 - 1; i > 0; --i){
            for(tmp = i; tmp < nb10; ++tmp){
                seq10[tmp-1] += (24 * seq10[tmp]);
            }
        }

        /* 3: to base 1000 */
        /* 3a: every position except last one */
        for(int i = 0; i < nb10 - 1; ++i){
            seq10[i+1] += (seq10[i]/1000);
            seq10[i] %= 1000;
        }

        /* 3b: last position normalization */
        while(seq10[nb10 - 1] >= 1000){
           seq10[nb10] += (seq10[nb10-1]/1000);
            seq10[nb10-1] %= 1000;
            nb10++;
        }

        /* 4: display */
        if(seq10[nb10-1] >= 0){
            os << seq10[nb10-1];
            for(int i = nb10 - 2; i >= 0; --i){
                if(seq10[i] < 10){
                    os << "00";
                }else if(seq10[i] < 100){
                    os << "0";
                }
                os << seq10[i];
            }
        }else{
            os << seq10[nb10-1];
            for(int i = nb10 - 2; i >= 0; --i){
                if(seq10[i] > -10){
                    os << "00";
                }else if(seq10[i] > -100){
                    os << "0";
                }
                os << -seq10[i];
            }
        }
        delete[] seq10;
        return os;
    }

    /**
     * @brief Display interesting data for the big integer
     */
    void info(){
        std::cout << "big_integer: [ ";
        for(auto val: array_){
            std::cout << val << " ";
        }
        std::cout << "]";
        std::cout << " nW=" << number_words_;
        std::cout << " mW=" << dim;
        std::cout << " rW=" << room_per_word_;
        std::cout << " wSize=" << word_size;
        std::cout << " base=" << base;
        std::cout << " sBase="<<size_base;
        std::cout << std::endl;
    }
};

#endif //LANGFORD_GODFREY_BIG_INTEGERS_HPP
