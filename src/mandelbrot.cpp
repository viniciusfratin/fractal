#include <stdio.h>
#include <math.h>

#include <common/common.hpp>
#include <complex.hpp>
#include <mandelbrot.hpp>

BOOL is_in_mandelbrot(complex_number num, double* divergence_ratio)
{
	BOOL is_in_set;
	BOOL escaped = FALSE;
	int i = 0;
	int i_conv = 0;
	
	complex_number z, temp;
	double z_radius_squared;
	complex_number_attrib(&z, 0.0, 0.0);

	while(i < MAX_ITERATIONS && escaped == FALSE)
	{
		z = complex_number_add(complex_number_square(z), num);
		z_radius_squared = z.real*z.real + z.imag*z.imag;
		if(z_radius_squared > ESCAPE_RADIUS_SQUARED)
		{
			i_conv = i;
			escaped = TRUE;
		}
			
		i++;
	}

	if(divergence_ratio != NULL)
	{
		*divergence_ratio = 1 - ((double)i_conv - log(log(z_radius_squared)/log(ESCAPE_RADIUS_SQUARED)))/MAX_ITERATIONS;
	}

	return TRUE;
}
