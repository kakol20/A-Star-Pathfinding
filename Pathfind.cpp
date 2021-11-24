#include "Pathfind.h"
#include "NodeHeap.h"
#include "Weights.h"

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
void Pathfind::AStar(const char * filePath, const char* savePath, Heuristic heuristic, Weight weight, bool allowDiagonal, bool allowCross) {
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
	openSet->Reserve(width * height);

	// set h cost of every node
	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			grid[x][y]->SetHCost(end, heuristic);
		}
	}

	if (!foundStart) {
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				if (!grid[x][y]->GetObstacle()) start = grid[x][y];
			}
		}
	}

	if (!foundEnd) {
		for (int y = height - 1; y >= 0; y--) {
			for (int x = width; x >= 0; x--) {
				if (!grid[x][y]->GetObstacle()) end = grid[x][y];
			}
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
		for (size_t i = 0; i < neighbours.size(); i++) {
			if (!neighbours[i]->GetObstacle()) {
				bool inClosedSet = std::find(closedSet.begin(), closedSet.end(), neighbours[i]) != closedSet.end();

				if (!inClosedSet) {
					int inOpenSet = openSet->Contains(neighbours[i]);

					//float tempG = (float)current->GetGCost() + Node::Distance(current, neighbours[i], heuristic) + ;
					//float tempG = current->GetGCost() + Node::Distance(current, neighbours[i], heuristic) + ;
					float tempG = (float)current->GetGCost() + Node::Distance(current, neighbours[i], heuristic);

					switch (weight) {
					case Weight::STRAIGHT:
						tempG += Weights::StraightTowards(current, neighbours[i], end);
						break;

					case Weight::TURNING:
						tempG += Weights::AvoidTurning(current, neighbours[i]);
						break;

					default:
						break;
					}

					if (tempG < neighbours[i]->GetGCost() || inOpenSet < 0) {
						neighbours[i]->SetGCost(tempG);
						neighbours[i]->SetFCost();

						neighbours[i]->SetParent(current);

						if (inOpenSet < 0) {
							openSet->Push(neighbours[i]);
						}
						else {
							openSet->SortUp(inOpenSet);
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
	float length = 0;
	std::vector<Node*> pathArray;
	while (pathNode != start) {
		pathArray.push_back(pathNode);

		length += Node::Distance(pathNode, pathNode->GetParent(), Heuristic::EUCLIDEAN);

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
	std::cout << "Path Length: " << length << '\n';

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

Pathfind::~Pathfind() {
}
