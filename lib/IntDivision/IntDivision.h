//
// Created by derek on 2022-12-11.
//

#ifndef LOST_IN_SPACE_INTDIVISION_H
#define LOST_IN_SPACE_INTDIVISION_H


namespace IntDivision {

    /**
     * Divide an integer into integer parts that will sum to the original.
     *
     * Say the integer 1,000 is divided by 60 - the integer solution is 16.
     * However 16 * 60 = 960 which shows that if the quotients are iteratively
     * added (16 + 16 + ... ) = 960 the sum will fall short.
     *
     * DitheredDivider provides a quotientSize that can be used when iterating
     * using the quotientSize method to provide integer quotients given their
     * index that are periodically larger as needed such that when summed
     * together they will produce the original numerator:
     * (16 + 17 + 17 + 16 + 17 + 17 + 16 + ...) = 1,000
     */
    class DitheredDivider {
    protected:
        const unsigned int denominator;
        const unsigned int quotient;
        const unsigned int remainder;
        const unsigned int ditherThreshold;

    public:
        explicit DitheredDivider(unsigned int numerator, unsigned int denominator);

        unsigned int quotientSize(unsigned int index) const;

        unsigned int total(unsigned int index) const;
    };
}

#endif //LOST_IN_SPACE_INTDIVISION_H
