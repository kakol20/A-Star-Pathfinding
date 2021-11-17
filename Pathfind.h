#ifndef PATHFIND_H
#define PATHFIND_H
#include "Node.h"
#include "Image.h"

class Pathfind {
public:
	Pathfind();

	static void A_Star(const char* pathLoc, const char* savePath, Heuristic heuristic, bool allowDiagonal, bool allowCross);

	~Pathfind();
};

#endif // !PATHFIND_H
