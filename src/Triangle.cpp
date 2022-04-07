#include "Triangle.h"
#include <iostream>

Triangle::Triangle(unsigned int a, unsigned int b, unsigned int c)
{
	indices[0] = a;
	indices[1] = b;
	indices[2] = c;
}

Triangle::Triangle(unsigned int indices_init[3])
{
	memcpy_s(indices, sizeof(indices), indices_init, sizeof(indices));
}

Triangle::Triangle()
{
}
