#include <stdio.h>
#include <stdlib.h>

#include <common/common.hpp>
#include <complex.hpp>
#include <draw.hpp>
#include <mandelbrot.hpp>

extern BOOL redraw;

double X_COMPLEX_RANGE_LOW = -3.0;
double X_COMPLEX_RANGE_HIGH = 1.0;
double Y_COMPLEX_RANGE_LOW = -2.0;
double Y_COMPLEX_RANGE_HIGH = 2.0;

double clicked_x = 0.0;
double clicked_y = 0.0;

#define MAGNIFICATION 4

int main(int argc, char* argv[])
{
	init(X_RES * Y_RES);

	complex_number* points = (complex_number*)malloc(X_RES * Y_RES * sizeof(complex_number));
	double* colors = (double*)malloc(3 * X_RES * Y_RES * sizeof(double));

	if(argc == 5)
	{
		X_COMPLEX_RANGE_LOW = atof(argv[1]);
		X_COMPLEX_RANGE_HIGH = atof(argv[2]);
		Y_COMPLEX_RANGE_LOW = atof(argv[3]);
		Y_COMPLEX_RANGE_HIGH = atof(argv[4]);
	}
	
	double current_x = X_COMPLEX_RANGE_LOW;
	double current_y = Y_COMPLEX_RANGE_LOW;
	double x_step = (double)(X_COMPLEX_RANGE_HIGH - X_COMPLEX_RANGE_LOW) / (double)X_RES;
	double y_step = (double)(Y_COMPLEX_RANGE_HIGH - Y_COMPLEX_RANGE_LOW) / (double)Y_RES;

	do
	{
		if(redraw == TRUE)
		{
			double complex_x_center = X_COMPLEX_RANGE_LOW + clicked_x * x_step;
			double complex_y_center = Y_COMPLEX_RANGE_LOW + (Y_RES - clicked_y) * y_step;

			double complex_x_margin = ((X_COMPLEX_RANGE_HIGH - X_COMPLEX_RANGE_LOW))/MAGNIFICATION;
			double complex_y_margin = ((double)Y_RES/(double)X_RES) * complex_x_margin;

			X_COMPLEX_RANGE_LOW = complex_x_center - complex_x_margin;
			X_COMPLEX_RANGE_HIGH = complex_x_center + complex_x_margin;
			Y_COMPLEX_RANGE_LOW = complex_y_center - complex_y_margin;
			Y_COMPLEX_RANGE_HIGH = complex_y_center + complex_y_margin;

			current_x = X_COMPLEX_RANGE_LOW;
			current_y = Y_COMPLEX_RANGE_LOW;
 			x_step = (double)(X_COMPLEX_RANGE_HIGH - X_COMPLEX_RANGE_LOW) / (double)X_RES;
			y_step = (double)(Y_COMPLEX_RANGE_HIGH - Y_COMPLEX_RANGE_LOW) / (double)Y_RES;
		}
			
		printf("%le %le %le %le\n", X_COMPLEX_RANGE_LOW, X_COMPLEX_RANGE_HIGH, Y_COMPLEX_RANGE_LOW, Y_COMPLEX_RANGE_HIGH);
		int i, j;
		for(i = 0; i < X_RES; i++)
		{
			for(j = 0; j < Y_RES; j++)
			{
				complex_number z;
				complex_number_attrib(&z, current_x + i*x_step, current_y + j*y_step);
				double divergence_ratio, convergence_ratio;
				double color_r, color_g, color_b;
	
				BOOL is_in_set = is_in_mandelbrot(z, &divergence_ratio);
				convergence_ratio = 1 - divergence_ratio;
				if(is_in_set == FALSE)
				{
					complex_number_attrib(&z, X_COMPLEX_RANGE_LOW, Y_COMPLEX_RANGE_LOW);
				}
				
				z.real = (2/(X_COMPLEX_RANGE_HIGH-X_COMPLEX_RANGE_LOW)) * z.real - (2*X_COMPLEX_RANGE_LOW/(X_COMPLEX_RANGE_HIGH - X_COMPLEX_RANGE_LOW) + 1);
				z.imag = (2/(Y_COMPLEX_RANGE_HIGH-Y_COMPLEX_RANGE_LOW)) * z.imag - (2*Y_COMPLEX_RANGE_LOW/(Y_COMPLEX_RANGE_HIGH - Y_COMPLEX_RANGE_LOW) + 1);
	
				points[i*Y_RES + j] = z;
				colors[3*(i*Y_RES + j)] = convergence_ratio;
				colors[3*(i*Y_RES + j) + 1] = convergence_ratio;
				colors[3*(i*Y_RES + j) + 2] = convergence_ratio;
			}
		}
		
		draw(points, colors, X_RES * Y_RES);
	} while(redraw == TRUE);

	finalize();
}
