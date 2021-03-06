#pragma once

#include <algorithm>
#include <vector>

#include "Node.h"

class NodeHeap {
public:
	NodeHeap();

	void Reserve(const size_t capacity);

	void Push(Node* item);
	void Pop();

	void Clear();
	
	int Contains(Node* item);
	bool Empty();
	size_t Size();

	Node* operator[](const size_t i);
	Node* GetIndex(const size_t i);

	void SortDown(const size_t i);
	void SortUp(const size_t i);

	Node* Top();

	~NodeHeap();

private:
	void SortDown();
	void SortUp();
	

	std::vector<Node*> m_heap;
};
