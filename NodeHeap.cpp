#include "NodeHeap.h"

NodeHeap::NodeHeap() {
}

/// <summary>
/// Add to heap
/// </summary>
/// <param name="item"></param>
void NodeHeap::Push(Node* item) {
	m_heap.push_back(item);

	SortUp();
}

/// <summary>
/// Removes top
/// </summary>
void NodeHeap::Pop() {
	if (m_heap.size() > 1) {
		Node* temp = m_heap[m_heap.size() - 1];

		m_heap[m_heap.size() - 1] = m_heap[0];
		m_heap[0] = temp;
		//std::iter_swap(m_heap.begin(), m_heap.end() - 1);
	}

	m_heap.pop_back();

	SortDown();
}

void NodeHeap::Clear() {
	m_heap.clear();
}

bool NodeHeap::Contains(Node* item) {
	return std::find(m_heap.begin(), m_heap.end(), item) != m_heap.end();
}

bool NodeHeap::Empty() {
	return m_heap.empty();
}

size_t NodeHeap::Size() {
	return m_heap.size();
}

Node* NodeHeap::operator[](const size_t i) {
	return m_heap[i];
}

Node* NodeHeap::GetIndex(const size_t i) {
	return m_heap[i];
}

/// <summary>
/// Get top of heap
/// </summary>
/// <returns></returns>
Node* NodeHeap::Top() {
	return m_heap.front();
}

NodeHeap::~NodeHeap() {
}

/// <summary>
/// Sorts from the top
/// </summary>
void NodeHeap::SortDown() {
	size_t parent = 0;
	
	while (true) {
		size_t childL = (parent * 2) + 1;
		size_t childR = (parent * 2) + 2;

		size_t swapIndex = 0;

		if (childL < m_heap.size()) {
			swapIndex = childL;

			if (childR < m_heap.size()) {
				if (m_heap[childL]->GetFCost() > m_heap[childR]->GetFCost()) {
					swapIndex = childR;
				}
			}

			if (m_heap[parent]->GetFCost() > m_heap[swapIndex]->GetFCost()) {
				Node* temp = m_heap[swapIndex];

				m_heap[swapIndex] = m_heap[parent];
				m_heap[parent] = temp;
			}

			parent = swapIndex;
		}
		else {
			break;
		}
	}
}

/// <summary>
/// Sorts from the bottom
/// </summary>
void NodeHeap::SortUp() {
	int child = m_heap.size() - 1;

	while (true) {
		int parent = (child - 1) / 2;

		if (m_heap[parent]->GetFCost() > m_heap[child]->GetFCost()) {
			Node* temp = m_heap[child];

			m_heap[child] = m_heap[parent];
			m_heap[parent] = temp;
		}
		else {
			break;
		}

		child = parent;
	}
}
