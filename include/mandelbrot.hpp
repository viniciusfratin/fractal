#ifndef MANDELBROT_H_
#define MANDELBROT_H_

#include <common/common.hpp>
#include <complex.hpp>

#define MAX_ITERATIONS 100
#define ESCAPE_RADIUS 50000.0
#define ESCAPE_RADIUS_SQUARED ESCAPE_RADIUS*ESCAPE_RADIUS

BOOL is_in_mandelbrot(complex_number num, double* divergence_ratio);

#endif
