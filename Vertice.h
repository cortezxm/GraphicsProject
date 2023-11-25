#pragma once
#include <vector>

class Vertice
{
public:
	int id;
	float x;
	float y;
	float z;
	float w = 1;
	float r, g, b;

	Vertice() {

	};

	Vertice(float x, float y, float z) {
		this->x = x;
		this->y = y;
		this->z = z;
	};

	Vertice(int id, float x, float y, float z) {
		this->id = id;
		this->x = x;
		this->y = y;
		this->z = z;
	};
};

