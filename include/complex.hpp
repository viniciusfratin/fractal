#ifndef COMPLEX_HPP_
#define COMPLEX_HPP_

#include <common/common.hpp>

struct complex_number_struct
{
	double real;
	double imag;
};

typedef struct complex_number_struct complex_number;


int complex_number_attrib(complex_number* num, double real, double imag);
complex_number complex_number_add(complex_number a, complex_number b);
complex_number complex_number_square(complex_number a);

#endif
