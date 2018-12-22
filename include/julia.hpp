#ifndef JULIA_HPP_
#define JULIA_HPP_

#include <common/common.hpp>
#include <complex.hpp>
#include <fractal.hpp>

BOOL is_in_julia(complex_number num, complex_number c, double* divergence_ratio);

#endif
