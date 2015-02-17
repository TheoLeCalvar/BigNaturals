#include "BigNatural.hpp"

#include <cstring>
#include <cstdlib>
#include <iostream>

BigNatural BigNatural::ZERO = BigNatural("0", 1);
BigNatural BigNatural::ONE  = BigNatural("1", 1);

BigNatural::BigNatural():size(0), digits(0) {
}

BigNatural::BigNatural(const char * digits, size_t size) {
        this->size = size;
        this->digits = new char[size];

        for (size_t i = 0; i < size; ++i)
                this->digits[i] = digits[size - 1 - i] - '0';

        resize();
}

BigNatural::BigNatural(const BigNatural& other) {
        size = other.size;
        digits = new char[size];

        memcpy(digits, other.digits, size);
}

BigNatural::BigNatural(const BigNatural& other, size_t offset) {
        size = other.size + offset;
        digits = new char[size];

        memcpy(digits + offset, other.digits, other.size);

        for (size_t i = 0; i < offset; ++i) 
                digits[i] = 0;
}

BigNatural::~BigNatural() {
        if (size > 0) delete[] digits;
}

BigNatural &    BigNatural::operator=(const BigNatural & rh) {
        if (size > 0) delete[] digits;

        size = rh.size;
        digits = new char[size];
        memcpy(digits, rh.digits, size);

        return *this;
}

BigNatural      BigNatural::operator+(const BigNatural & rh) const {
        char*   tmp = new char[MAX(size, rh.size) + 1];
        char    carry = 0;

        for (size_t i = 0; i < MAX(size, rh.size); ++i) {
                if (i >= size) {
                        tmp[i] = (rh.digits[i] + carry) % 10;
                        carry = ((rh.digits[i] + carry) > 9) ? 1 : 0;
                }
                else if (i >= rh.size) {
                        tmp[i] = (digits[i] + carry) % 10;
                        carry = ((digits[i] + carry) > 9) ? 1 : 0;
                }
                else {
                        tmp[i] = (digits[i] + rh.digits[i] + carry) % 10;
                        carry = ((digits[i] + rh.digits[i] + carry) > 9 ? 1 : 0);
                }
        }

        if (carry) tmp[MAX(size, rh.size)] = 1;

        BigNatural res;

        res.size = MAX(size, rh.size) + carry;
        res.digits = tmp;

        return res;
}


BigNatural      BigNatural::operator-(const BigNatural & rh) const {
        if (operator<(rh)) return BigNatural();

        char* tmp = new char[size];
        char carry = 0;


        for (size_t i = 0; i < size; ++i) {
                if (i >= rh.size) {
                        tmp[i] = (10 + digits[i] - carry) % 10;
                        carry = ((digits[i] - carry) < 0) ? 1 : 0;
                }
                else {
                        tmp[i] = (10 + digits[i] - rh.digits[i] - carry) % 10;
                        carry = ((digits[i] - rh.digits[i] - carry) < 0) ? 1 : 0;
                }
        }

        BigNatural res;

        res.size = size;
        res.digits = tmp;

        res.resize();

        return res;
}



BigNatural      BigNatural::operator*(const BigNatural & rh) const {
        if (!isValid() || !rh.isValid()) return BigNatural();
        if (operator==(ZERO) || rh == ZERO) return ZERO;
        if (operator==(ONE)) return rh;
        if (rh == ONE) return *this;       

        BigNatural res;

        for (size_t i = 0; i < size; ++i) {
                BigNatural tmp = ZERO;

                for (char j = 0; j < digits[i]; ++j) 
                        tmp += rh;
                
                tmp.shift(i);
                res += tmp;

        }

        return res;
}


BigNatural      BigNatural::operator/(const BigNatural & rh) const {
        if (!isValid() || !rh.isValid()) return BigNatural();
        if (operator==(ZERO)) return ZERO;
        if (rh == ZERO) return BigNatural();

        BigNatural res = ZERO;
        BigNatural tmp = *this;


        while (tmp >= rh) {
                tmp -= rh;
                res += ONE;
        }
        return res;
        
}


BigNatural      BigNatural::operator%(const BigNatural & rh) const {
        if (!isValid() || !rh.isValid()) return BigNatural();
        if (operator==(ZERO)) return ZERO;
        if (rh == ZERO) return BigNatural();

        BigNatural tmp = *this;

        while (tmp > rh) {
                tmp -= rh;
        }

        return tmp;
}



BigNatural&     BigNatural::operator+=(const BigNatural & rh) {
        BigNatural res = operator+(rh);

        *this = res;

        return *this;
}


BigNatural&     BigNatural::operator-=(const BigNatural & rh) {
        BigNatural res = operator-(rh);

        *this = res;

        return *this;
}


BigNatural&     BigNatural::operator*=(const BigNatural & rh) {
        BigNatural res = operator*(rh);

        *this = res;

        return *this;

}


BigNatural&     BigNatural::operator/=(const BigNatural & rh) {
        BigNatural res = operator/(rh);

        *this = res;

        return *this;
}




bool            BigNatural::operator==(const BigNatural & rh) const {
        if (size != rh.size) return false;

        return !memcmp(digits, rh.digits, size);
}


bool            BigNatural::operator!=(const BigNatural & rh) const {
        return !operator==(rh);
}


bool            BigNatural::operator<(const BigNatural & rh) const {
        return !operator>=(rh);
}


bool            BigNatural::operator>(const BigNatural & rh) const {
        return !operator<=(rh);
}


bool            BigNatural::operator<=(const BigNatural & rh) const {
        if (size < rh.size) return true;
        if (size > rh.size) return false;
        if (size == 0)      return true;

        size_t i = size - 1;
        while ((i > 0) && (digits[i] == rh.digits[i])) --i;


        return (digits[i] <= rh.digits[i]);
}


bool            BigNatural::operator>=(const BigNatural & rh) const {
        if (size > rh.size) return true;
        if (size < rh.size) return false;
        if (size == 0)      return true;

        size_t i = size - 1;
        while ((i > 0) && (digits[i] == rh.digits[i])) --i;

        return (digits[i] >= rh.digits[i]);
}

bool    BigNatural::isValid() const {
        return size != 0;
}

BigNatural BigNatural::factorial(const BigNatural & rh) {
        BigNatural res = ONE;
        BigNatural i = ONE;

        while (i <= rh) {
                res *= i;
                i += ONE;
        }

        return res;
}

void    BigNatural::print() const {
        if (size == 0)
                std::cout << "NaN";

        for (size_t i = size; i > 0; --i) 
                std::cout << (char)(digits[i - 1] + '0');

        std::cout << std::endl;
}

void    BigNatural::shift(size_t offset) {
        char* newDigits = new char[size + offset];

        memmove(newDigits + offset, digits, size);

        size += offset;

        for (size_t i = 0; i < offset; ++i) 
                newDigits[i] = 0;  

        delete[] digits;

        digits = newDigits;     
}

void    BigNatural::resize() {
        if (!isValid()) return;

        char * newDigit;

        while ((size > 1) && !digits[size - 1]) --size;

        newDigit = new char[size];

        memcpy(newDigit, digits, size);

        delete[] digits;

        digits = newDigit;
}

#ifdef TEST

#include "test.h"
#include "log.h"

int main(int argc, char** argv) {

        BigNatural *a = new BigNatural("123", 3);
        BigNatural *b = new BigNatural(*a);

        TEST_ASSERT("Init BigNatural from char*", 
                ((a->size = 3) && (a->digits[0] == 3) && (a->digits[2] == 1)));
        TEST_ASSERT("Init BigNatural with copy", (*a == *b));
        BigNatural *c = new BigNatural(*a, 2);
        TEST_ASSERT("Init with shift", 
                ((c->size == 5) && (c->digits[0] == 0) && (c->digits[2] == 3)));

        *c = *a;
        TEST_ASSERT("Test operator=", (*a == *c));
        TEST_ASSERT("Test operator==", (*a == *a));


        BigNatural *d = new BigNatural("00001234", 8);
        d->resize();
        TEST_ASSERT("Test resize, 00001234 -> 1234", 
                ((d->size == 4) 
                        && (d->digits[0] == 4) 
                        && (d->digits[3] == 1)
                ));

        delete d;
        d = new BigNatural("0000000", 7);
        d->resize();
        TEST_ASSERT("Test resize, 0000 -> 0", 
                ((d->size == 1) && (d->digits[0] == 0)));

        TEST_ASSERT("Test operator+ without carry", 
                ((*a + *b) == BigNatural("246", 3)));

        TEST_ASSERT("Test operator-", (*a - *b) == BigNatural("0", 1));

        delete b;
        b = new BigNatural("12345", 5);
        TEST_ASSERT("Test operator-, a - b, a < b, (a-b) should not be valid", 
                !(*a - *b).isValid());


        TEST_ASSERT("Test operator*, a, b != 0", 
                (*a * *b == BigNatural("1518435", 7)));

        TEST_ASSERT("Test operator*, a * 0",
                (*a * BigNatural::ZERO == BigNatural::ZERO))

        TEST_ASSERT("Test operator/", 
                ((BigNatural("1518435", 7) / *b) == *a));


        TEST_ASSERT("Test operator/",
                ((BigNatural("14589652", 8) / *a) == BigNatural("118615", 6)));


        TEST_ASSERT("Test operator%", 
                ((BigNatural("14589652", 8) % *a) == BigNatural("7", 1)));


        delete b;
        b = new BigNatural("1543", 4);
        TEST_ASSERT("Test operator!=", (*a != *b));
        
        delete b;
        b = new BigNatural("1234", 4);
        TEST_ASSERT("Test operator>=, b.size > a.size", (*b >= *a));
        TEST_ASSERT("Test operator<=, b.size > a.size", (*a <= *b));

        delete b;
        b = new BigNatural("124", 3);
        TEST_ASSERT("Test operator>=, b.size == a.size", (*b >= *a));
        TEST_ASSERT("Test operator<=, b.size == a.size",  (*a <= *b));

        delete b;
        b = new BigNatural(*a);
        TEST_ASSERT("Test operator>=, b == a", (*b >= *a));
        TEST_ASSERT("Test operator<=, b == a", (*a <= *b));

        delete b;
        b = new BigNatural();
        TEST_ASSERT("Test isValid, a is valid", a->isValid());
        TEST_ASSERT("Test isValid, b isn't valid", !b->isValid());

        TEST_ASSERT("Test factorial(100)", 
                (BigNatural::factorial(BigNatural("100", 3)) == 
                BigNatural("93326215443944152681699238856266700490715968264381621468592963895217599993229915608941463976156518286253697920827223758251185210916864000000000000000000000000", 158)));


        return 0;
}

#endif

