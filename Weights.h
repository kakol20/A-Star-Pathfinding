#pragma once
#include "Node.h"

class Weights {
public:
	Weights();

	static float AvoidTurning(Node* current, Node* neighbour);
	static float StraightTowards(Node* current, Node* neighbour, Node* end);

	~Weights();
};
