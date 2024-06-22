/**
 * @file permutationVector.h
 * 
 * @details
 * The permutationVector class is a specialized class to iterate through all possibilities.
 * it makes the above process concise and easy to use, as can be seen in the code example below
 * changing line 106 (number[i] =1) to (number[i] = 0) would make this class more useful for other non-specialized applications
 * specially to those related to the SUBSET SUM problem
 * 
 * @author German Becerra
 * 
 * Usage:
 * @code
 * #include "permutationVector.h"
 * 
 * int main() {
 *     unsigned int base = 9;  // Replace with corresponding base
 *     unsigned int digits = 5; // Replace with corresponding # of digits
 * 
 *     for(permutationVector pv(base, digits); !pv.allPermutationsIterated; pv++){
 *         pv.print();
 *     }
 *     return 0;
 * }
 *
 * @endcode
 */

#pragma once
//This class in itself prevents us from having to store in memory all possibilities, thus optimizing space but tackles a problem similar to the clasic SUBSET SUM problem
//As such, using this class incurrs to having some sort of exponential (NP) complexity. in this case O(digits^base)

#include <iostream>
#include <vector>
#include <stdint.h>

/// @brief Specialized class to get all possibilities of a nonogram row, simply doing ObjectName++ will get the next permutation. 
class permutationVector {
public: 
    /// @brief Constructor for permutationVector, 
    /// @param base the space available in the cell, MUST BE >0. ( [cols or rows] - nonoInput[x].sum())  
    /// @param digits the amount of white space runs inbetween black space runs, (nonoInput[x].size()-1)
    permutationVector(uintmax_t base, uintmax_t digits) : base(base), digits(digits), number(digits, 1), currentSum(digits), allPermutationsIterated(false)  {}

    /// @brief Overload prefix ++ operator to do nextPermutation
    permutationVector& operator++() {
        nextPermutation();
        return *this;
    }

    /// @brief Overload prostfix ++ operator to do nextPermutation
    permutationVector operator++(int) {
        permutationVector temp = *this;
        nextPermutation();
        return temp;
    }

    /// @brief prints held number vector. Debugging purposes.
    void print() const {
        for (auto it = number.rbegin(); it != number.rend(); ++it) {
            std::cout << *it << " ";
        }
        std::cout << std::endl;
    }

    /// @brief currentSum accessor
    /// @return summation of number vector
    uintmax_t getSum() const { return currentSum;}

    /// @brief number vector accesor to avoid copying it
    const std::vector<uintmax_t>& getNumber() const {
        return number;
    }

    bool allPermutationsIterated; //boolean flag to stop ++ operations

private:

    uintmax_t base;
    uintmax_t digits;
    uintmax_t currentSum;
    
    std::vector<uintmax_t> number;

    /// @brief sum all elements in the number vector and assign answer to currentSum
    void sum() {
        uintmax_t sum{};
        for(uintmax_t i =0; i < this->digits; i++){
            sum += this->number[i];
        }
        this->currentSum = sum;
        return;
    }

    /// @brief go to next Permutation
    ///acts as if 1 was 0 (as minimum space inbetween two black runs must be 1), and then just treat it like a counting problem.
    void nextPermutation() {
        
        do{
            int i = 0;
            while (i < digits) {
                number[i]++;
                if (number[i] < base) {
                    break;
                }
                number[i] = 1;
                i++;
            }
            if(i >= digits){ //helps to determine when to stop doing permutationVectorObject++.
                allPermutationsIterated = true;
            }
            this->sum();
        }while(this->currentSum > base);
    }

};
