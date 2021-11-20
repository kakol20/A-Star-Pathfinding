#include <vector>
#include <stdlib.h>
#include <algorithm>
#include <map>
#include <unordered_map>

#include "Image.h"
#include "Node.h"
#include "String.h"
#include "Pathfind.h"

int main() {
	// open image
	std::vector<String> files;

	//files.push_back("blank");
	//files.push_back("blank_10x10");
	//files.push_back("simple");
	files.push_back("obstacles");
	//files.push_back("maze");

	std::map<Heuristic, String> heuristics;
	heuristics[Heuristic::CHEBYSHEV] = "chebyshev";
	heuristics[Heuristic::EUCLIDEAN] = "euclidean";
	heuristics[Heuristic::MANHATTAN] = "manhattan";
	heuristics[Heuristic::OCTILE] = "octile";

	for (auto it = files.begin(); it != files.end(); it++) {
		String pathLoc = "images/";
		String tempLoc = pathLoc + *it;

		pathLoc = tempLoc;
		
		pathLoc = pathLoc + ".png";

		for (auto jt = heuristics.begin(); jt != heuristics.end(); jt++) {
			String savePath = "images/weights/";
			savePath += *it;
			savePath += "_straightTowards_";

			savePath += (*jt).second;
			savePath += ".png";

			Pathfind::AStar(pathLoc.GetChar(), savePath.GetChar(), (*jt).first, true, false);
		}


		std::cout << "\n----------\n";
		//Pathfind::A_Start_OpenSet(pathLoc.GetChar(), savePath.GetChar(), Heuristic::EUCLIDEAN, true, false);
	}

	system("pause");

	return 0;
}