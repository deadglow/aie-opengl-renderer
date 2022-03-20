#pragma once

struct Triangle
{
	unsigned int indices[3];

	Triangle(unsigned int a, unsigned int b, unsigned int c);
	Triangle(unsigned int indices_init[3]);
	Triangle();
};