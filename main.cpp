#include <vector>
#include <stdlib.h>
#include <algorithm>

#include "Image.h"
#include "Node.h"
#include "String.h"
#include "Pathfind.h"

int main() {
	// open image
	String fileName = "obstacles";
	String pathLoc = "images/";
	pathLoc = pathLoc + fileName;

	//String savePath = "images/obstacles_solve_manhattan.png";
	String savePath = "images/";
	savePath = pathLoc + "_solve_manhattan.png";

	pathLoc += ".png";

	Pathfind::A_Star(pathLoc.GetChar(), savePath.GetChar(), Heuristic::MANHATTAN, true, false);

	system("pause");

	return 0;
}