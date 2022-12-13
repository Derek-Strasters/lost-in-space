//
// Created by derek on 2022-12-11.
//

#include "IntDivision.h"

using namespace IntDivision;


DitheredDivider::DitheredDivider(const unsigned int numerator, const unsigned int denominator) :
        _denominator(denominator),
        _quotient(numerator / denominator),
        _remainder(numerator % denominator),
        _ditherThreshold(denominator - _remainder),
        _index(0) {}

unsigned int DitheredDivider::quotientSize(const unsigned int index) const {
    const unsigned int plus = index + 1;
    const unsigned int dither = (plus * _remainder) % _denominator >= _ditherThreshold;
    return _quotient + dither;
}

unsigned int DitheredDivider::total(const unsigned int index) const {
    const unsigned int plus = index + 1;
    return plus * _quotient + (plus * _remainder) / _denominator;
}

void DitheredDivider::setNumeratorDenominator(unsigned int numerator, unsigned int denominator) {
    _denominator = denominator;
    _quotient = numerator / denominator;
    _remainder = numerator % denominator;
    _ditherThreshold = denominator - _remainder;
    _index = 0;
}

unsigned int DitheredDivider::nextQuotientSize() {
    return quotientSize(_index++ % _denominator);
}
