#include <stdio.h>
#include <math.h>

#include <common/common.hpp>
#include <complex.hpp>
#include <julia.hpp>

BOOL is_in_julia(complex_number num, complex_number c, double* divergence_ratio)
{
	BOOL is_in_set;
	BOOL escaped = FALSE;
	int i = 0;
	int i_conv = 0;
	
	complex_number z;
	double z_radius_squared;
	z = num;

	while(i < MAX_ITERATIONS && escaped == FALSE)
	{
		z = complex_number_add(complex_number_square(z), c);
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
		*divergence_ratio = 1 - ((double)i_conv - log(log(z_radius_squared)/log(ESCAPE_RADIUS_SQUARED)))/i_conv;
	}

	return TRUE;
}
