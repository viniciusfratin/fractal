#ifndef MANDELBROT_HPP_
#define MANDELBROT_HPP_

#include <common/common.hpp>
#include <complex.hpp>
#include <fractal.hpp>

BOOL is_in_mandelbrot(complex_number num, double* divergence_ratio);

#endif
