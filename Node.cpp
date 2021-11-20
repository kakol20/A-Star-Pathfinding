#include "Node.h"

#include <stdlib.h>
#include <math.h>
#include <algorithm>

Node::Node() {
	m_fCost = 0;
	m_gCost = 0;
	m_hCost = 0;

	m_x = 0;
	m_y = 0;

	m_obstacle = false;
}

Node::Node(const int x, const int y) {
	m_fCost = 0;
	m_gCost = 0;
	m_hCost = 0;

	m_x = x;
	m_y = y;

	m_obstacle = false;
}

void Node::SetCoordinates(const int x, const int y) {
	m_x = x;
	m_y = y;
}

int Node::GetX() {
	return m_x;
}

int Node::GetY() {
	return m_y;
}

void Node::SetCosts(const float hCost, const float gCost) {
	m_hCost = hCost;
	m_gCost = gCost;

	m_fCost = hCost + gCost;
}

/// <summary>
/// Total of hCost and gCost
/// </summary>
void Node::SetFCost() {
	m_fCost = m_hCost + m_gCost;
}

/// <summary>
/// Distance from starting node (cumulative distance)
/// </summary>
/// <param name="gCost"></param>
void Node::SetGCost(const float gCost) {
	m_gCost = gCost;
}

/// <summary>
/// Distance to end node (actual)
/// </summary>
/// <param name="end"></param>
void Node::SetHCost(const Node* end, Heuristic heuristic) {
	m_hCost = Node::Distance(this, end, heuristic);
}

float Node::GetFCost() {
	return m_fCost;
}

float Node::GetGCost() {
	return m_gCost;
}

float Node::GetHCost() {
	return m_hCost;
}

void Node::SetObstacle(const bool flag) {
	m_obstacle = flag;
}

bool Node::GetObstacle() {
	return m_obstacle;
}

void Node::SetParent(Node* parent) {
	m_parent = parent;
}

Node* Node::GetParent() {
	return m_parent;
}

bool Node::operator>(const Node* other) {
	return m_fCost > other->m_fCost;
}

float Node::Distance(const Node* start, const Node* end, Heuristic heuristic) {
	float x = (float)abs(end->m_x - start->m_x);
	float y = (float)abs(end->m_y - start->m_y);

	float mx = std::max(x, y);
	float mn = std::min(x, y);

	float sqrt2 = 1.41f;

	float F = sqrt2 - 1.0f;

	switch (heuristic) {
	case Heuristic::MANHATTAN:
		return x + y;

	case Heuristic::EUCLIDEAN:
		return (sqrt2 * mn) + 1.0f * (mx - mn);

	case Heuristic::CHEBYSHEV:
		return std::max(x, y);

	case Heuristic::OCTILE:
		return F * mn + mx;

	}
	return 0.0f;
}

Node::~Node() {
}
