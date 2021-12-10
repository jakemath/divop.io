/*
Author: Jake Mathai
Purpose: Bigint implementation file
*/
#include "bigint.h"

Bigint::Bigint() {}

Bigint::Bigint(std::string num) {
    for (unsigned long long i = 0; i < num.length(); ++i)
        digits.push_back(num[num.length() - 1 - i] - '0');
}

Bigint::Bigint(const char* num) {
    std::string num_as_string(num);
    for (unsigned long long i = 0; i < num_as_string.length(); ++i)
        digits.push_back(num_as_string[num_as_string.length() - 1 - i] - '0');
}

Bigint::Bigint(unsigned long long size, bool randomize, bool is_divisor /*=false*/) {
    if (randomize) { // Randomized digits
        std::default_random_engine engine;
        engine.seed(std::chrono::system_clock::now().time_since_epoch().count());
        std::uniform_int_distribution<int> uniform(0, 9);
        for (unsigned long long i = 1; i <= size; ++i)
            digits.push_back(uniform(engine));
        while (digits.back() == 0)
            digits.back() = uniform(engine);
        if (is_divisor) {
            short valid_ones_digits[4] = {1, 3, 7, 9};
            short exception_digit = 5;
            if (digits.size() == 1) {
                valid_ones_digits[0] = 5;
                exception_digit = 1;
            }
            short ones_digit = digits.front();
            while (ones_digit % 2 == 0 || ones_digit == exception_digit) {
                digits.front() = valid_ones_digits[uniform(engine) % 4];
                ones_digit = digits.front();
            }
        }
    }
    else    // 0-filled
        digits = std::list<short>(size, 0);
}

// Addition by reference. Result stored in sum argument
void operator + (Bigint& sum, Bigint& b) {
    if (b.digits.back() == 0)  // In this program we mark a number as 0 by making the most significant digit 0. If 0 --> no execution
        return;
    if (sum.digits.back() >= 0 && b.digits.back() < 0) {   // A + (-B) = A - B
        b.digits.back() *= -1;
        sum - b;
        return;
    }
    short carry = 0, digit_sum;  // Carry stores potential overflow at each digit, sum holds result of sum at each digit
    std::list<short>::iterator i, j;
    for (i = sum.digits.begin(), j = b.digits.begin(); i != sum.digits.end() && j != b.digits.end(); ++i, ++j) {
        digit_sum = *i + *j + carry;
        if (digit_sum > 9) {  // If sum > 9 --> carry = 1, store sum - 10
            *i = digit_sum - 10;
            carry = 1;
        }
        else {   // If sum < 10 --> store sum
            *i = digit_sum;
            carry = 0;
        }
    }
    if (i != sum.digits.end() && j == b.digits.end()) {   // If sum has more digits than b, compute                                               // the rest of digits while a carry of 1 exists
        for (; i != sum.digits.end() && carry == 1; ++i)  {// No need to continue once a carry of 0 occurs
            digit_sum = *i + carry;
            if (digit_sum > 9) {
                *i = digit_sum - 10;
                carry = 1;
            }
            else {
                *i = digit_sum;
                carry = 0;
            }
        }
    }
    if (carry != 0) // If carry exists after computing each digit --> add a new digit of 1
        sum.digits.push_back(carry);
}

// Subtraction by reference. Result stored in difference argument
void operator - (Bigint& difference, Bigint& quantity) {
    if (quantity.digits.back() == 0) // No execution if subtracting 0
        return;
    if (difference < quantity) {// Negative results defaulted to negative
        difference.digits.back() *= -1;
        return;
    }
    if (difference == quantity) { // If same number --> 0
        difference.digits.back() = 0;
        return;
    }
    short diff; // diff holds result of subtraction at each digit
    std::list<short>::iterator i, j;
    for (i = difference.digits.begin(), j = quantity.digits.begin(); i != difference.digits.end() && j != quantity.digits.end(); ++i, ++j) {
        diff = *i - *j;
        if (diff < 0) {  // If negative result --> find the first non-zero digit after the current digit
            std::list<short>::iterator k = i;   // Iterate from current digit
            if (*i != 0)    // If current digit non-zero --> ignore
                ++k;
            for (; *k == 0 && k != difference.digits.end(); ++k)    // Switch all 0 digits to 9 until a non-zero digit is found
                *k = 9;
            if (k != difference.digits.end()) {
                --*k;   // Decrement first non-zero digit to borrow from it
                *i = diff + 10; // Add borrow to the current digit
            }
        }
        else    // If positive result --> store diff
            *i = diff;
    }
    if (difference.digits.back() == 0) { // If subtracting same sized numbers --> most significant digit may result in 0 and will need to be removed
        while (difference.digits.back() == 0 && !difference.digits.empty())    // While the most significant digit is 0, pop_back()
            difference.digits.pop_back();
        if (difference.digits.empty())  // If the whole number was cleared, return 0
            difference.digits.push_back(0);
    }
}

// Pre-increment
void Bigint::operator ++() {
    if (digits.front() != 9)  {// If ones digit != 9 --> simple increment
        ++digits.front();
        return;
    }
    std::list<short>::iterator i = digits.begin();  // If ones digit == 9 --> find first non-nine digit to add the carry to
    for (; *i == 9 && i != digits.end(); ++i) // Switch all 9's to 0 until first non-9 digit found
        *i = 0;
    if (i != digits.end())    // If not at the end of number --> increment digit
        ++*i;
    else    // If at end of number --> append a new significant digit
        digits.push_back(1);
}

// Multiplication by short
Bigint operator * (const Bigint& b, short k) {
    if (k == 0 || b.digits.back() == 0)   // If multiplying by 0 --> return 0
        return Bigint("0");
    Bigint product; // Store result in product
    if (abs(k) < 10) { // If k < 10 --> no recursion
        short prod, carry = 0; // Prod holds result of digit multiplication, carry holds potential overflow from prod
        for (std::list<short>::const_iterator i = b.digits.begin(); i != b.digits.end(); ++i) {
            prod = (abs(*i * k)) + carry;  // Use abs and worry about negativity after
            if (prod > 9) {  // If prod > 10 --> store ones digit of prod and let carry = prod / 10. So if prod = 81 --> result = 1, carry = 8
                product.digits.push_back(prod % 10);
                carry = prod / 10;
            }
            else {   // If prod < 10 --> store result
                product.digits.push_back(prod);
                carry = 0;
            }
        }
        if (carry != 0) // push_back any leftover carry after iterating
            product.digits.push_back(carry);
    }
    else if (abs(k) == 10) {  // Optimization for multiple of 10
        product = b;
        product.digits.push_front(0);
    }
    else {  // k > 10 and not multiple of 10 --> recurse and add (b * ones digit of k) + (b * k / 10)
        Bigint this_product = b * (k%10), remaining_product = b * (k/10);
        remaining_product.digits.push_front(0);
        remaining_product + this_product;
        if (b.digits.back() < 0 != k < 0)    // Account for differing signs
            remaining_product.digits.back() *= -1;
        return remaining_product;
    }
    if (b.digits.back() < 0 != k < 0)    // Account for differing signs
        product.digits.back() *= -1;
    return product;
}

// Bigint multiplication, similar principle as above. Operands assumed to be positive
Bigint operator * (const Bigint& b1, const Bigint& b2) {
    if (b1.digits.back() == 0 || b2.digits.back() == 0)
        return Bigint("0");
    Bigint product;
    if (b2.digits.size() >= 2) {
        if (b2.digits.size() == 2 && b2.digits.front() == 0 && (b2.digits.back() == 1 || b2.digits.back() == -1)) {
            product = b1;
            product.digits.push_front(0);
        }
        else {
            product = b2;
            product.digits.pop_front();
            Bigint this_product = b1 * b2.digits.front(), remaining_product = b1 * product;
            remaining_product.digits.push_front(0);
            remaining_product + this_product;
            return remaining_product;
        }
    }
    else {
        short prod, carry = 0;
        for (std::list<short>::const_iterator i = b1.digits.begin(); i != b1.digits.end(); ++i) {
            prod = (abs(*i) * b2.digits.front()) + carry;
            if (prod > 9) {
                product.digits.push_back(prod % 10);
                carry = prod / 10;
            }
            else {
                product.digits.push_back(prod);
                carry = 0;
            }
        }
        if (carry != 0)
            product.digits.push_back(carry);
    }
    if (b1.digits.back() < 0 != b2.digits.back() < 0)
        product.digits.back() *= -1;
    return product;
}

// Optimized inplace scalar multiplication function. Result stored in product argument
void multiply_by_reference(Bigint& product, const Bigint& rule, short k) {
    if (k == 0) {  // Multiplication by 0
        product.digits.back() = 0;
        return;
    }
    short prod, carry = 0;
    std::list<short>::const_iterator i;
    std::list<short>::iterator j;
    for (i = rule.digits.begin(), j = product.digits.begin(); i != rule.digits.end() && j != product.digits.end(); ++i, ++j) {
        prod = (*i * k) + carry;
        if (prod > 9) {
            *j = prod % 10;
            carry = prod / 10;
        }
        else {
            *j = prod;
            carry = 0;
        }
    }
    if (i == rule.digits.end() && j != product.digits.end()) {
        if (carry != 0) {
            *j = carry;
            carry = 0;
        }
        else
            --j;
        std::list<short>::iterator k = std::next(product.digits.end(), -1);
        while (j != k) {
            product.digits.pop_back();
            k = std::next(product.digits.end(), -1);
        }
    }
    else if (i != rule.digits.end() && j == product.digits.end()) {
        for (; i != rule.digits.end(); ++i) {
            prod = (abs(*i) * k) + carry;
            if (prod > 9) {
                product.digits.push_back(prod % 10);
                carry = prod / 10;
            }
            else {
                product.digits.push_back(prod);
                carry = 0;
            }
        }
    }
    if (carry != 0)
        product.digits.push_back(carry);
    if (rule.digits.back() < 0)
        product.digits.back() *= -1;
}

// < optimized by assuming comparison to a positive number
bool Bigint::operator < (const Bigint& b) const {
    return digits.size() != b.digits.size() ? digits.size() < b.digits.size()
            : lexicographical_compare(digits.rbegin(), digits.rend(), b.digits.rbegin(), b.digits.rend());
}

// > optimized by assuming comparison to a positive number
bool Bigint::operator > (const Bigint& b) const {
    return digits.back() <= 0 ? false
            : (digits.size() != b.digits.size() ? digits.size() > b.digits.size()
                : !lexicographical_compare(digits.rbegin(), digits.rend(), b.digits.rbegin(), b.digits.rend()));
}

// Div operator. Iteratively reduce A to a small enough number to compute
// divisibility. divisor divides dividend_{i} <--> divisor divides dividend_{i+1}
bool div(Bigint& dividend, Bigint& divisor) {
    if (divisor.digits.size() == 1 && divisor.digits.back() != 7 && divisor.digits.back() != 9) {   // Special cases: 2, 3, and 5
        if (divisor.digits.front() == 2)  // Div by 2
            return dividend.digits.front() % 2 == 0;
        if (divisor.digits.front() == 3)  // Div by 3
            return std::accumulate(dividend.digits.begin(), dividend.digits.end(), 0) % 3 == 0;
        return dividend.digits.front() == 0 || dividend.digits.front() == 5;   // div by 5
    }
    if (dividend.digits.size() == divisor.digits.size()) {
        if (dividend == divisor)
            return true;
        if (dividend > divisor) {
            for (short i = 2; i <= 9; ++i) {
                if (dividend == divisor * i)
                    return true;
            }
        }
        return false;
    }
    if (dividend.digits.size() < divisor.digits.size())
        return false;
    Bigint rule, threshold = divisor;
    threshold.digits.push_front(0);
    std::unordered_set<Bigint, BigintHash> multiples;
    multiples.insert(divisor);
    multiples.insert(threshold);
    for (short factor = 2; factor <= 9; ++factor)
        multiples.insert(divisor*factor);
    rule = (divisor.digits.front() == 1 || divisor.digits.front() == 9) ? divisor : divisor*3;
    if (rule.digits.front() == 1) {
        rule.digits.pop_front();
        rule.digits.back() *= -1;
    }
    else {
        if (rule.digits.size() > 1)
            rule.digits.pop_front();
        ++rule;
    }
    void (*arithmetic_operation)(Bigint&, Bigint&);
    if (rule.digits.back() < 0) {
        rule.digits.back() *= -1;
        arithmetic_operation = operator -;
    }
    else 
        arithmetic_operation = operator +;
    Bigint product(rule.digits.size(), 0);
    short ones;
    while (dividend > threshold) { // Iterate until within bounds of the set
        ones = dividend.digits.front();
        dividend.digits.pop_front();
        multiply_by_reference(product, rule, ones); // product = rule * ones
        arithmetic_operation(dividend, product);   // dividend = dividend/10 + product
    }
    short last_digit = dividend.digits.back();
    if (last_digit < 0)  // If negative --> is not divisible
        return false;
    if (last_digit == 0)  // If 0 --> is divisible
        return true;
    return multiples.find(dividend) != multiples.end(); // If one of the multiples --> is divisible
}

std::string Bigint::as_str() const {
    std::string digit_string = "";
    for (std::list<short>::const_reverse_iterator i = digits.rbegin(); i != digits.rend(); ++i)
        digit_string += std::to_string(*i);
    return digit_string;
}

const char* Bigint::as_cstr() const { 
    return strdup(as_str().data());
}

std::ostream& operator <<(std::ostream& out, const Bigint& b) {  // Outstream overload
    for (std::list<short>::const_reverse_iterator i = b.digits.rbegin(); i != b.digits.rend(); ++i)
        out << *i;
    return out;
}
