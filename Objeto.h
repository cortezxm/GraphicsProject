#pragma once
#include <string>
#include <vector>
#include "Cara.h"

class Objeto
{
public:
	std::string name;
	std::vector<Cara> caras;
	std::vector<Vertice> vertices;
};

