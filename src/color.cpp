#include <stdlib.h>

#include <common/common.hpp>
#include <color.hpp>

int rgb_color_attrib(rgb_color* color, double r, double g, double b)
{
	if(color == NULL)
	{
		abort("NULL color.\n");
	}

	color->r = r;
	color->g = g;
	color->b = b;

	return RET_SUCCESS;
}
