#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>  // for memcpy
#include <math.h>    // for fma

/* re-interpret a 64-bit unsigned integer in two halves as a double */
double hilo_uint32_as_double (uint32_t hi, uint32_t lo)
{
    double r;
    uint64_t t = ((uint64_t)hi << 32) + (uint64_t)lo;
    memcpy (&r, &t, sizeof r);
    return r;
}

double uint64_to_double (uint64_t a)
{
    const double two_to_32 = 0x1.0p32;
    const double magic = hilo_uint32_as_double (0x43300000, 0); // 0x1.0p52

    /* split 64-bit number into two 32-bit halves */
    uint32_t hi = (uint32_t)(a >> 32);         // stw
    uint32_t lo = (uint32_t)(a & 0xffffffff);  // stw
    /* convert each 32-bit half into a double */
    double lof = hilo_uint32_as_double (0x43300000, lo) - magic; // stw, lfd, fsub
    double hif = hilo_uint32_as_double (0x43300000, hi) - magic; // stw, lfd, fsub
    /* combine halves with a single rounding */
    return fma (hif, two_to_32, lof); // fmadd
}

int main (void)
{
    uint64_t a = 1234567890123456ULL;
    printf ("%llu -> %23.16e\n", a, uint64_to_double (a));
    return EXIT_SUCCESS;
}

/*
There is no modulus instruction. If you want to get the remainder, take the quotient, multiple it by the divisor, and subtract it from the dividend.

dividend / divisor = quotient
numerator / denominator = quotient

A % B = A-((A/B)*B)
7576-((7576//365)*365)
*/
