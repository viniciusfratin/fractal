#include <stdlib.h>

#include <common/common.hpp>
#include <complex.hpp>

int complex_number_attrib(complex_number* num, double real, double imag)
{
	if(num == NULL)
	{
		abort("Null pointer in complex number attribution.\n");
	}

	num->real = real;
	num->imag = imag;

	return RET_SUCCESS;
}

complex_number complex_number_add(complex_number a, complex_number b)
{
	complex_number sum;
	sum.real = a.real + b.real;
	sum.imag = a.imag + b.imag;

	return sum;
}

complex_number complex_number_square(complex_number a)
{
	complex_number square;
	square.real = a.real * a.real - (a.imag * a.imag);
	square.imag = 2 * a.real * a.imag;

	return square;
}
