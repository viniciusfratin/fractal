#include <stdio.h>
#include <stdlib.h>

#include <common/common.hpp>

void abort(const char* msg)
{
	fprintf(stderr, "%s\n", msg);
	exit(RET_ERROR);
}
