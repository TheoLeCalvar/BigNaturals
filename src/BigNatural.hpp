#ifndef _BIGNATURAL_H_
#define _BIGNATURAL_H_ 

#include <cstring>


#define MAX(a, b) \
                ((a) > (b) ? (a) : (b))


class BigNatural
{
protected:
        size_t  size;
        char *  digits;
public:

        static BigNatural ZERO;
        static BigNatural ONE;

        BigNatural();
        BigNatural(const char * digits, size_t size);
        BigNatural(const BigNatural & other);
        BigNatural(const BigNatural & other, size_t offset);

        ~BigNatural();

        BigNatural&     operator=(const BigNatural & rh);

        BigNatural      operator+(const BigNatural & rh) const;
        BigNatural      operator-(const BigNatural & rh) const;

        BigNatural      operator*(const BigNatural & rh) const;
        BigNatural      operator/(const BigNatural & rh) const;
        BigNatural      operator%(const BigNatural & rh) const;

        BigNatural&     operator+=(const BigNatural & rh);
        BigNatural&     operator-=(const BigNatural & rh);
        BigNatural&     operator*=(const BigNatural & rh);
        BigNatural&     operator/=(const BigNatural & rh);

        bool            operator==(const BigNatural & rh) const;
        bool            operator!=(const BigNatural & rh) const;
        bool            operator< (const BigNatural & rh) const;
        bool            operator> (const BigNatural & rh) const;
        bool            operator<=(const BigNatural & rh) const;
        bool            operator>=(const BigNatural & rh) const;

        bool            isValid() const;


        static BigNatural      factorial(const BigNatural & rh);

        void            shift(size_t offset);
        void            print() const;

private:
        void            resize();

#ifdef TEST
        friend int main(int argc, char** argv);
#endif
};

#endif