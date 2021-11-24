#pragma once
#include "Node.h"
#include "Image.h"

enum class Weight {
	NONE, STRAIGHT, TURNING
};

class Pathfind {
public:
	Pathfind();

	static void AStar(const char * filePath, const char* savePath, Heuristic heuristic, Weight weight, bool allowDiagonal, bool allowCross);

	~Pathfind();
};
