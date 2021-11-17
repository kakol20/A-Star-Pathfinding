#include "Pathfind.h"

#include <vector>
#include <stdlib.h>
#include <algorithm>

Pathfind::Pathfind() {
}

void Pathfind::A_Star(const char* pathLoc, const char* savePath, Heuristic heuristic, bool allowDiagonal, bool allowCross) {
	Image path(pathLoc);

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

	std::vector<Node*> openSet;
	std::vector<Node*> closedSet;

	// set h cost of every node
	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			grid[x][y]->SetHCost(end, heuristic);
		}
	}

	start->SetGCost(0);
	start->SetFCost();

	//int maxFCost = abs(end->GetX() - start->GetX()) + abs(end->GetY() - start->GetY());

	openSet.push_back(start);

	Node* current = nullptr;
	while (!openSet.empty()) {
		// set current node from open set with lowest f cost
		int index = 0;
		for (int i = 0; i < (int)openSet.size(); i++) {
			if (openSet[i]->GetFCost() < openSet[index]->GetFCost()) index = i;
		}
		current = openSet[index];
		if (current == end) {
			break;
		}

		openSet.erase(openSet.begin() + index);
		closedSet.push_back(current);

		// find neighbours
		std::vector<Node*> neighbours;

		int x = current->GetX();
		int y = current->GetY();

		bool up = y != 0, down = y < height - 1, left = x != 0, right = x < width - 1;
		bool upObstacle = true, downObstacle = true, leftObstacle = true, rightObstacle = true;

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
					if (allowCross || (!upObstacle && !rightObstacle)) {
						neighbours.push_back(grid[x + 1][y - 1]);
					}
				}

				if (down) { // bottom
					if (allowCross || (!downObstacle && !rightObstacle)) {
						neighbours.push_back(grid[x + 1][y + 1]);
					}
				}
			}

			// left hand side
			if (left) {
				if (up) { // top
					if (allowCross || (!upObstacle && !leftObstacle)) {
						neighbours.push_back(grid[x - 1][y - 1]);
					}
				}

				if (down) { // bottom
					if (allowCross || (!downObstacle && !leftObstacle)) {
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
					bool inOpenSet = std::find(openSet.begin(), openSet.end(), neighbours[i]) != openSet.end();

					float tempG = (float)current->GetGCost() + Node::Distance(current, neighbours[i], heuristic);

					if (inOpenSet) {
						if (tempG < neighbours[i]->GetGCost()) {
							neighbours[i]->SetGCost(tempG);
							neighbours[i]->SetParent(current);
							neighbours[i]->SetFCost();
						}
					}
					else {
						neighbours[i]->SetGCost(tempG);
						neighbours[i]->SetParent(current);
						neighbours[i]->SetFCost();

						openSet.push_back(neighbours[i]);
					}
				}
			}
		}
	}

	// draw open set
	if (!openSet.empty()) {
		for (size_t i = 0; i < openSet.size(); i++) {
			int x = openSet[i]->GetX();
			int y = openSet[i]->GetY();
			int index = path.GetIndex(x, y);

			path.SetData(index + 0, (uint8_t)127);
			path.SetData(index + 1, (uint8_t)255);
			path.SetData(index + 2, (uint8_t)127);
		}
	}

	for (size_t i = 0; i < closedSet.size(); i++) {
		int x = closedSet[i]->GetX();
		int y = closedSet[i]->GetY();
		int index = path.GetIndex(x, y);

		path.SetData(index + 0, (uint8_t)230);
		path.SetData(index + 1, (uint8_t)255);
		path.SetData(index + 2, (uint8_t)230);
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

	// destroy pointers
	openSet.clear();
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

Pathfind::~Pathfind() {
}
