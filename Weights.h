#pragma once
#include "Node.h"

class Weights {
public:
	Weights();

	static float StraightTowards(Node* current, Node* neighbour, Node* end);

	~Weights();
};
