#pragma once
#include <cmath>
#include "Vertice.h" 

class Vector
{
public:
	float x, y, z;

	Vector(float x, float y, float z) {
		this->x = x;
		this->y = y;
		this->z = z;
	};

	static Vector subtract(Vertice a, Vertice b)
	{
		return Vector(
			a.x - b.x,
			a.y - b.y,
			a.z - b.z);
	}

	static Vector add(Vector a, Vector b)
	{
		return Vector(
			a.x + b.x,
			a.y + b.y,
			a.z + b.z);
	}

	static Vector crossProduct(Vector a, Vector b) {
		return Vector(
			a.y * b.z - a.z * b.y,
			a.z * b.x - a.x * b.z,
			a.x * b.y - a.y * b.x);
	}

	static Vector direction(Vertice start, Vertice end)
	{
		return Vector::subtract(end, start);
	}

	
	void normalize()
	{
		float mag = sqrt(x * x + y * y + z * z);
		x /= mag;
		y /= mag;
		z /= mag;
	}

};

