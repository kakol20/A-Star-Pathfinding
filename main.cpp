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
	files.push_back("blank_10x10");
	files.push_back("simple");
	files.push_back("obstacles");
	files.push_back("maze");

	for (auto it = files.begin(); it != files.end(); it++) {
		String pathLoc = "images/";
		String savePathEuclidean = "images/";
		String savePathManhattan = "images/";

		pathLoc = pathLoc + *it;
		savePathManhattan = pathLoc + "_solve_manhattan.png";
		savePathEuclidean = pathLoc + "_solve_euclidean.png";
		pathLoc = pathLoc + ".png";

		Pathfind::A_Star_ClosedSet(pathLoc.GetChar(), savePathManhattan.GetChar(), Heuristic::MANHATTAN, true, false);
		Pathfind::A_Star_ClosedSet(pathLoc.GetChar(), savePathEuclidean.GetChar(), Heuristic::EUCLIDEAN, true, false);

		std::cout << "----------\n";
		//Pathfind::A_Start_OpenSet(pathLoc.GetChar(), savePath.GetChar(), Heuristic::EUCLIDEAN, true, false);
	}

	system("pause");

	return 0;
}