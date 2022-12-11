//
// Created by derek on 2022-12-11.
//

#include "IntDivision.h"

using namespace IntDivision;


DitheredDivider::DitheredDivider(const unsigned int numerator, const unsigned int denominator) :
        denominator(denominator),
        quotient(numerator / denominator),
        remainder(numerator % denominator),
        ditherThreshold(denominator - remainder) {}

unsigned int DitheredDivider::quotientSize(const unsigned int index) const {
    const unsigned int plus = index + 1;
    const unsigned int dither = (plus * remainder) % denominator >= ditherThreshold;
    return quotient + dither;
}

unsigned int DitheredDivider::total(const unsigned int index) const {
    const unsigned int plus = index + 1;
    return plus * quotient + (plus * remainder) / denominator;
}
