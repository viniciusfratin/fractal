#include <stdio.h>
#include <stdlib.h>

#include <common/common.hpp>
#include <complex.hpp>
#include <draw.hpp>
#include <mandelbrot.hpp>
#include <julia.hpp>
#include <main.h>

extern BOOL redraw;

double clicked_x = 0.0;
double clicked_y = 0.0;

#define FRACTAL_MANDELBROT 1
#define FRACTAL_JULIA 2

#define MIN_PARAM_NUMBER 2
#define MANDELBROT_PARAM_NUMBER 2
#define JULIA_PARAM_NUMBER 4
#define MAX_PARAM_NUMBER 8

int main(int argc, char* argv[])
{
	double x_complex_range_low = -3.0;
	double x_complex_range_high = 1.0;
	double y_complex_range_low = -2.0;
	double y_complex_range_high = 2.0;

	int fractal_type;
	complex_number fractal_seed;
	complex_number_attrib(&fractal_seed, 0.0, 0.0);

	if(argc < MIN_PARAM_NUMBER || argc > MAX_PARAM_NUMBER || (argc != MANDELBROT_PARAM_NUMBER && argc != JULIA_PARAM_NUMBER && argc != MAX_PARAM_NUMBER))
	{
		char usage_str[512];
		sprintf(usage_str, 
			"%s\n%s\n",
			"Usage: ./fractal <TYPE> [<FRACTAL_SEED_REAL> <FRACTAL_SEED_IMAG> <LOW_X> <HIGH_X> <LOW_Y> <HIGH_Y>]",
			"\t<TYPE> = 1 (Mandelbrot), 2 (Julia)"
			);

		abort(usage_str);
	}

	fractal_type = atoi(argv[1]);
	if(fractal_type != FRACTAL_MANDELBROT && fractal_type != FRACTAL_JULIA)
	{
		abort("Invalid fractal type.\n");
	}

	if(argc == JULIA_PARAM_NUMBER)
	{
		fractal_seed.real = atof(argv[2]);
		fractal_seed.imag = atof(argv[3]);
	}

	if(argc == MAX_PARAM_NUMBER)
	{
		fractal_seed.real = atof(argv[2]);
		fractal_seed.imag = atof(argv[3]);

		x_complex_range_low = atof(argv[4]);
		x_complex_range_high = atof(argv[5]);
		y_complex_range_low = atof(argv[6]);
		y_complex_range_high = atof(argv[7]);
	}
	
	init(X_RES * Y_RES);

	complex_number* points = (complex_number*)malloc(X_RES * Y_RES * sizeof(complex_number));
	rgb_color* colors = (rgb_color*)malloc(X_RES * Y_RES * sizeof(rgb_color));

	double current_x = x_complex_range_low;
	double current_y = y_complex_range_low;
	double x_step = (double)(x_complex_range_high - x_complex_range_low) / (double)X_RES;
	double y_step = (double)(y_complex_range_high - y_complex_range_low) / (double)Y_RES;

	do
	{
		if(redraw == TRUE)
		{
			double complex_x_center = x_complex_range_low + clicked_x * x_step;
			double complex_y_center = y_complex_range_low + (Y_RES - clicked_y) * y_step;

			double complex_x_margin = ((x_complex_range_high - x_complex_range_low))/MAGNIFICATION;
			double complex_y_margin = ((double)Y_RES/(double)X_RES) * complex_x_margin;

			x_complex_range_low = complex_x_center - complex_x_margin;
			x_complex_range_high = complex_x_center + complex_x_margin;
			y_complex_range_low = complex_y_center - complex_y_margin;
			y_complex_range_high = complex_y_center + complex_y_margin;

			current_x = x_complex_range_low;
			current_y = y_complex_range_low;
 			x_step = (double)(x_complex_range_high - x_complex_range_low) / (double)X_RES;
			y_step = (double)(y_complex_range_high - y_complex_range_low) / (double)Y_RES;
		}
			
		printf("%le %le %le %le\n", x_complex_range_low, x_complex_range_high, y_complex_range_low, y_complex_range_high);
		int i, j;
		for(i = 0; i < X_RES; i++)
		{
			for(j = 0; j < Y_RES; j++)
			{
				complex_number z;
				complex_number_attrib(&z, current_x + i*x_step, current_y + j*y_step);
				double divergence_ratio, convergence_ratio;
				double color_r, color_g, color_b;
	
				BOOL is_in_set;
				switch(fractal_type)
				{
					case FRACTAL_MANDELBROT:
						is_in_set = is_in_mandelbrot(z, &divergence_ratio);
						break;

					case FRACTAL_JULIA:
						is_in_set = is_in_julia(z, fractal_seed, &divergence_ratio);
						break;
				}

				convergence_ratio = 1 - divergence_ratio;
				if(is_in_set == FALSE)
				{
					complex_number_attrib(&z, x_complex_range_low, y_complex_range_low);
				}
				
				z.real = (2/(x_complex_range_high-x_complex_range_low)) * z.real - (2*x_complex_range_low/(x_complex_range_high - x_complex_range_low) + 1);
				z.imag = (2/(y_complex_range_high-y_complex_range_low)) * z.imag - (2*y_complex_range_low/(y_complex_range_high - y_complex_range_low) + 1);
	
				points[i*Y_RES + j] = z;
				rgb_color_attrib(&colors[i*Y_RES + j], convergence_ratio, convergence_ratio, convergence_ratio);
			}
		}
		
		draw(points, colors, X_RES * Y_RES);
	} while(redraw == TRUE);

	finalize();
}
