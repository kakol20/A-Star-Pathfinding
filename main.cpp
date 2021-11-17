#include <vector>
#include <stdlib.h>
#include <algorithm>

#include "Image.h"
#include "Node.h"
#include "String.h"
#include "Pathfind.h"

int main() {
	// open image
	std::vector<String> files;

	files.push_back("blank");
	files.push_back("simple");
	files.push_back("obstacles");
	files.push_back("maze");

	for (auto it = files.begin(); it != files.end(); it++) {
		String pathLoc = "images/";
		String savePath = "images/";

		pathLoc = pathLoc + *it;
		savePath = pathLoc + "_solve_withClosedSet_noDiagonal.png";
		pathLoc = pathLoc + ".png";

		Pathfind::A_Star_ClosedSet(pathLoc.GetChar(), savePath.GetChar(), Heuristic::EUCLIDEAN, false, false);
		//Pathfind::A_Start_OpenSet(pathLoc.GetChar(), savePath.GetChar(), Heuristic::EUCLIDEAN, true, false);
	}

	system("pause");

	return 0;
}