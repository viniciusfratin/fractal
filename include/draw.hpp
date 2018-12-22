#ifndef DRAW_HPP_
#define DRAW_HPP_

#include <GL/glew.h>

#include <complex.hpp>
#include <color.hpp>

#define X_RES 1024
#define Y_RES 768

int init(long size);
int draw(complex_number* points, rgb_color* colors, long size);
int finalize();
int convert_points_to_ogl_array(complex_number* points, GLdouble* ogl_array, long size);
int convert_colors_to_ogl_array(rgb_color* colors, GLdouble* ogl_array, long size);

#endif
