/*
Author: Jake Mathai
Purpose: Bigint header file
*/

#ifndef BIGINT_H
#define BIGINT_H

#include <iostream>
#include <list>
#include <chrono> 
#include <string>
#include <string.h>
#include <random>
#include <numeric>
#include <iterator>
#include <unordered_set>

class Bigint {
    public:
        Bigint();
        Bigint(std::string num);
        Bigint(const char* num);
        Bigint(unsigned long long size, bool randomize);
        friend std::ostream& operator << (std::ostream& out, const Bigint& b);  // Outstream overload
        void operator ++(); // Pre-increment operator
        bool operator < (const Bigint& b) const;  // Optimized comparison operators
        bool operator > (const Bigint& b) const;
        bool operator == (const Bigint& b) const { return digits == b.digits; }
        void operator = (const Bigint& b) { digits = b.digits; }    // Assignment operator
        void print();
        std::string as_str() const;
        const char* as_cstr() const;
        friend void operator + (Bigint& sum, Bigint& b);    // Addition
        friend void operator - (Bigint& result, Bigint& b);   // Subtraction
        friend Bigint operator * (const Bigint& b, short k); // Scalar multiplication
        friend Bigint operator * (const Bigint& b1, const Bigint& b2); // Bigint multiplication
        friend void rule_multiply(Bigint& product, const Bigint& rule, short k); // Optimized inplace scalar multiplication
        friend bool div(Bigint& dividend, Bigint& divisor); // Div operator
        std::list<short> digits;  // Digits stored in reverse order in list
};

// Hash function - use stringified digits
struct BigintHash {  
    std::size_t operator()(const Bigint& b) const {
        return std::hash<std::string>()(b.as_str());
    }
};

extern "C" {
    Bigint* generate(unsigned size, bool randomize) {
        return new Bigint(size, randomize);
    }
    Bigint* from_str(const char* num) {
        return new Bigint(num);
    }
    unsigned long long size(Bigint* num) {
        return num -> digits.size();
    }
    const char* as_cstr(Bigint* num) {
        return num -> as_cstr();
    }
    bool divides(Bigint* dividend, Bigint* divisor) { 
        return div(*dividend, *divisor); 
    }
}

#endif /* bigint_h */
