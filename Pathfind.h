#pragma once
#include "Node.h"
#include "Image.h"

class Pathfind {
public:
	Pathfind();

	static void AStar(const char * filePath, const char* savePath, Heuristic heuristic, bool allowDiagonal, bool allowCross);

	~Pathfind();
};
