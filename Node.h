#ifndef NODE_H
#define NODE_H

enum class Heuristic {
	MANHATTAN, EUCLIDEAN
};

class Node {
public:
	Node();
	Node(const int x, const int y);

	void SetCoordinates(const int x, const int y);
	int GetX();
	int GetY();

	void SetCosts(const float hCost, const float gCost);
	void SetFCost();
	void SetGCost(const float gCost);
	void SetHCost(const Node* end, Heuristic heuristic);
	float GetFCost();
	float GetGCost();
	float GetHCost();

	void SetObstacle(const bool flag);
	bool GetObstacle();

	static float Distance(const Node* start, const Node* end, Heuristic heuristic);

	void SetParent(Node* parent);
	Node* GetParent();

	~Node();

private:
	float m_fCost = 0;
	float m_gCost = 0;
	float m_hCost = 0;

	int m_x = 0;
	int m_y = 0;

	bool m_obstacle = false;

	Node* m_parent = nullptr;
};

#endif // !NODE_H
