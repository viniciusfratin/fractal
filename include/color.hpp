#ifndef COLOR_HPP_
#define COLOR_HPP_

struct rgb_color_struct
{
	double r;
	double g;
	double b;
};

typedef struct rgb_color_struct rgb_color;

int rgb_color_attrib(rgb_color* color, double r, double g, double b);

#endif
