#include "Pathfind.h"
#include "NodeHeap.h"

#include <vector>
#include <stdlib.h>
#include <algorithm>

Pathfind::Pathfind() {
}

/// <summary>
/// Version with closed set array
/// </summary>
/// <param name="filePath"></param>
/// <param name="savePath"></param>
/// <param name="heuristic"></param>
/// <param name="allowDiagonal"></param>
/// <param name="allowCross"></param>
void Pathfind::A_Star_ClosedSet(const char * filePath, const char* savePath, Heuristic heuristic, bool allowDiagonal, bool allowCross) {
	std::cout << '\n';

	Image path(filePath);

	// create grid
	int width = path.GetWidth();
	int height = path.GetHeight();

	bool foundStart = false, foundEnd = false;

	Node* start = nullptr;
	Node* end = nullptr;

	std::vector<std::vector<Node*>> grid;
	for (int x = 0; x < width; x++) {
		std::vector<Node*> yGrid;
		for (int y = 0; y < height; y++) {
			Node* tempNode = new Node(x, y);

			int index = path.GetIndex(x, y);
			uint8_t r = path.GetData(index + 0);
			uint8_t g = path.GetData(index + 1);
			uint8_t b = path.GetData(index + 2);

			if ((r == 0) && (g == 0) && (b == 0)) {
				tempNode->SetObstacle(true);
			}
			else {
				if (!foundStart && (r == 255) && (g == 0) && (b == 0)) {
					start = tempNode;
					foundStart = true;
				}

				if (!foundEnd && (r == 0) && (g == 0) && (b == 255)) {
					end = tempNode;
					foundEnd = true;
				}
			}

			yGrid.push_back(tempNode);
		}

		grid.push_back(yGrid);
	}

	//std::vector<Node*> openSet;
	std::vector<Node*> closedSet;

	NodeHeap* openSet = new NodeHeap();

	//std::priority_queue<Node*, std::vector<Node*>, decltype(&Node::Ascending)> openSet(Node::Ascending);

	// set h cost of every node
	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			grid[x][y]->SetHCost(end, heuristic);
		}
	}

	start->SetGCost(0);
	start->SetFCost();

	openSet->Push(start);

	Node* current = nullptr;
	while (!openSet->Empty()) {
		current = openSet->Top();
		
		if (current == end) break;

		openSet->Pop();

		closedSet.push_back(current);

		// find neighbours
		std::vector<Node*> neighbours;

		int x = current->GetX();
		int y = current->GetY();

		bool up = y != 0, down = y < height - 1, left = x != 0, right = x < width - 1;
		bool upObstacle = false, downObstacle = false, leftObstacle = false, rightObstacle = false;

		// up
		if (up) {
			neighbours.push_back(grid[x][y - 1]);
			upObstacle = grid[x][y - 1]->GetObstacle();
		}
		// down
		if (down) {
			neighbours.push_back(grid[x][y + 1]);
			downObstacle = grid[x][y + 1]->GetObstacle();
		}
		// left {
		if (left) {
			neighbours.push_back(grid[x - 1][y]);
			leftObstacle = grid[x - 1][y]->GetObstacle();
		}
		// right
		if (right) {
			neighbours.push_back(grid[x + 1][y]);
			rightObstacle = grid[x + 1][y]->GetObstacle();
		}

		if (allowDiagonal) {
			// right hand side
			if (right) {
				if (up) { // top
					if (allowCross || !(upObstacle && rightObstacle)) {
						neighbours.push_back(grid[x + 1][y - 1]);
					}
				}

				if (down) { // bottom
					if (allowCross || !(downObstacle && rightObstacle)) {
						neighbours.push_back(grid[x + 1][y + 1]);
					}
				}
			}

			// left hand side
			if (left) {
				if (up) { // top
					if (allowCross || !(upObstacle && leftObstacle)) {
						neighbours.push_back(grid[x - 1][y - 1]);
					}
				}

				if (down) { // bottom
					if (allowCross || !(downObstacle && leftObstacle)) {
						neighbours.push_back(grid[x - 1][y + 1]);
					}
				}
			}
		}

		// query neighbours
		int tentative_gCost = 0;
		for (int i = 0; i < (int)neighbours.size(); i++) {
			if (!neighbours[i]->GetObstacle()) {
				bool inClosedSet = std::find(closedSet.begin(), closedSet.end(), neighbours[i]) != closedSet.end();

				if (!inClosedSet) {
					bool inOpenSet = openSet->Contains(neighbours[i]);

					float tempG = (float)current->GetGCost() + Node::Distance(current, neighbours[i], heuristic);

					if (tempG < neighbours[i]->GetGCost() || !inOpenSet) {
						neighbours[i]->SetGCost(tempG);
						neighbours[i]->SetFCost();

						neighbours[i]->SetParent(current);

						if (!inOpenSet) {
							openSet->Push(neighbours[i]);
						}
					}
				}
			}
		}
	}

	// draw open set
	if (!openSet->Empty()) {
		for (size_t i = 0; i < openSet->Size(); i++) {
			/*int x = openSet[i]->GetX();
			int y = openSet[i]->GetY();*/
			int x = openSet->GetIndex(i)->GetX();
			int y = openSet->GetIndex(i)->GetY();
			int index = path.GetIndex(x, y);

			path.SetData(index + 0, (uint8_t)127);
			path.SetData(index + 1, (uint8_t)255);
			path.SetData(index + 2, (uint8_t)127);
		}
	}

	// draw closed set
	for (size_t i = 0; i < closedSet.size(); i++) {
		int x = closedSet[i]->GetX();
		int y = closedSet[i]->GetY();
		int index = path.GetIndex(x, y);

		path.SetData(index + 0, (uint8_t)191);
		path.SetData(index + 1, (uint8_t)255);
		path.SetData(index + 2, (uint8_t)191);
	}

	// get path and draw
	Node* pathNode = end;
	std::vector<Node*> pathArray;
	while (pathNode != start) {
		pathArray.push_back(pathNode);

		pathNode = pathNode->GetParent();
	}
	pathArray.push_back(start);

	for (int i = 0; i < (int)pathArray.size(); i++) {
		int x = pathArray[i]->GetX();
		int y = pathArray[i]->GetY();
		int index = path.GetIndex(x, y);

		float gradient = (float)i / (float)pathArray.size();
		gradient *= 255.0f;

		path.SetData(index + 0, gradient);
		path.SetData(index + 1, 0.0f);
		path.SetData(index + 2, 255.0f - gradient);
	}

	path.Write(savePath);
	std::cout << "Path Length: " << end->GetGCost() << '\n';

	// destroy pointers
	openSet->Clear();
	delete openSet;
	openSet = nullptr;

	closedSet.clear();
	pathArray.clear();
	for (int x = 0; x < width; x++) {
		for (auto p : grid[x]) {
			delete p;
			p = nullptr;
		}
		grid[x].clear();
	}
	grid.clear();
}

/// <summary>
/// Version without using a closed set array
/// </summary>
/// <param name="filePath"></param>
/// <param name="savePath"></param>
/// <param name="heuristic"></param>
/// <param name="allowDiagonal"></param>
/// <param name="allowCross"></param>
/// <returns></returns>
bool Pathfind::A_Start_OpenSet(const char* filePath, const char* savePath, Heuristic heuristic, bool allowDiagonal, bool allowCross) {

	// ----- GENERATE GRID -----

	Image gridImg(filePath);

	bool foundStart = false, foundEnd = false;
	int height = gridImg.GetHeight();
	int width = gridImg.GetWidth();
	Node* end = nullptr;
	Node* start = nullptr;
	std::vector<std::vector<Node*>> grid;

	for (int x = 0; x < width; x++) {
		std::vector<Node*> yGrid;
		for (int y = 0; y < height; y++) {
			Node* tempNode = new Node(x, y);

			int index = gridImg.GetIndex(x, y);
			uint8_t r = gridImg.GetData(index + 0);
			uint8_t g = gridImg.GetData(index + 1);
			uint8_t b = gridImg.GetData(index + 2);

			tempNode->SetGCost((float)INT_MAX);
			tempNode->SetFCost();

			if ((r == 0) && (g == 0) && (b == 0)) {
				tempNode->SetObstacle(true);
			}
			else {
				if (!foundStart && (r == 255) && (g == 0) && (b == 0)) {
					start = tempNode;
					foundStart = true;
				}

				if (!foundEnd && (r == 0) && (g == 0) && (b == 255)) {
					end = tempNode;
					foundEnd = true;
				}
			}

			yGrid.push_back(tempNode);
		}

		grid.push_back(yGrid);
	}

	// calculate h cost of ever node
	for (auto x : grid) {
		for (auto y : x) {
			y->SetHCost(end, heuristic);
		}
	}

	// ----- INIT START NODE -----
	start->SetGCost(0);
	start->SetHCost(end, heuristic);
	start->SetFCost();

	// ----- CREATE OPEN SET -----
	std::vector<Node*> openSet;
	openSet.push_back(start);

	bool foundRoute = false;

	while (!openSet.empty()) {

		// ----- CURRENT FROM OPEN SET WITH LOWEST F COST -----

		size_t index = 0;
		for (size_t i = 0; i < openSet.size(); i++) {
			if (openSet[i]->GetFCost() < openSet[index]->GetFCost()) index = i;
		}
		Node* current = openSet[index];

		// ----- CHECK IF END REACHED -----

		if (current == end) {
			foundRoute = true;
			break;
		}

		openSet.erase(openSet.begin() + index);

		// ----- GENERATE ARRAY OF NEIGHBOURS -----

		std::vector<Node*> neighbours;
		int x = current->GetX();
		int y = current->GetY();

		bool top = y != 0, bottom = y < height - 1, left = x != 0, right = x < width - 1; // check if valid neighbour
		bool topObstacle = false, bottomObstacle = false, leftObstacle = false, rightObstacle = false; // check if obstacle

		if (top) {
			topObstacle = grid[x][y - 1]->GetObstacle();

			if (!topObstacle) neighbours.push_back(grid[x][y - 1]);
		}
		if (bottom) {
			bottomObstacle = grid[x][y + 1]->GetObstacle();

			if (!bottomObstacle) neighbours.push_back(grid[x][y + 1]);
		}
		if (left) {
			leftObstacle = grid[x - 1][y]->GetObstacle();

			if (!leftObstacle) neighbours.push_back(grid[x - 1][y]);
		}
		if (right) {
			rightObstacle = grid[x + 1][y]->GetObstacle();

			if (!rightObstacle) neighbours.push_back(grid[x + 1][y]);
		}	

		if (allowDiagonal) {
			if (right) {
				if (top && (allowCross || !topObstacle || !rightObstacle)) {
					if (!grid[x + 1][y - 1]->GetObstacle()) neighbours.push_back(grid[x + 1][y - 1]);
				}

				if (bottom && (allowCross || !bottomObstacle || !rightObstacle)) {
					if (!grid[x + 1][y + 1]->GetObstacle()) neighbours.push_back(grid[x + 1][y + 1]);
				}
			}
			if (left) {
				if (top && (allowCross || !topObstacle || !leftObstacle)) {
					if (!grid[x - 1][y - 1]->GetObstacle()) neighbours.push_back(grid[x - 1][y - 1]);
				}

				if (bottom && (allowCross || !bottomObstacle || !leftObstacle)) {
					if (!grid[x - 1][y + 1]->GetObstacle()) neighbours.push_back(grid[x - 1][y + 1]);
				}
			}
		}

		// ----- CHECK NEIGBOURS -----
		if (!neighbours.empty()) {
			for (size_t i = 0; i < neighbours.size(); i++) {
				float tentativeGCost = current->GetGCost() + Node::Distance(current, neighbours[i], heuristic);

				if (tentativeGCost < neighbours[i]->GetGCost()) {
					neighbours[i]->SetParent(current);
					neighbours[i]->SetGCost(tentativeGCost);
					neighbours[i]->SetFCost();

					bool inOpenSet = std::find(openSet.begin(), openSet.end(), neighbours[i]) != openSet.end();

					if (!inOpenSet) openSet.push_back(neighbours[i]);
				}
			}
		}
	}

	// ----- SAVE RESULT AS PNG IMAGE -----

	if (foundRoute) {
		// draw path
		Node* pathNode = end;
		std::vector<Node*> pathArray;
		while (pathNode != start) {
			pathArray.push_back(pathNode);

			pathNode = pathNode->GetParent();
		}
		pathArray.push_back(pathNode);

		for (size_t i = 0; i < pathArray.size(); i++) {
			int x = pathArray[i]->GetX();
			int y = pathArray[i]->GetY();
			int index = gridImg.GetIndex(x, y);

			float gradient = (float)i / (float)(pathArray.size() - 1);
			gradient *= 255.0f;

			gridImg.SetData(index + 0, gradient);
			gridImg.SetData(index + 1, 0.0f);
			gridImg.SetData(index + 2, 255.0f - gradient);
		}

		gridImg.Write(savePath);
	}

	// ----- DESTROY POINTERS -----

	openSet.clear();
	for (auto x : grid) {
		for (auto y : x) {
			delete y;
			y = nullptr;
		}
		x.clear();
	}
	grid.clear();

	return foundRoute;
}

Pathfind::~Pathfind() {
}
