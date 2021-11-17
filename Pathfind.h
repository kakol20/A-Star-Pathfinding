#ifndef PATHFIND_H
#define PATHFIND_H
#include "Node.h"
#include "Image.h"

class Pathfind {
public:
	Pathfind();

	static void A_Star_ClosedSet(const char * filePath, const char* savePath, Heuristic heuristic, bool allowDiagonal, bool allowCross);
	static bool A_Start_OpenSet(const char* filePath, const char* savePath, Heuristic heuristic, bool allowDiagonal, bool allowCross);

	~Pathfind();
};

#endif // !PATHFIND_H
