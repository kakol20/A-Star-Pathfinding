#include "Weights.h"
#include "Vector2D.h"

Weights::Weights() {
}

float Weights::StraightTowards(Node* current, Node* neighbour, Node* end) {
	// Current to End vector
	Vector2D curr2End(end->GetX() - current->GetX(), end->GetY() - current->GetY());
	curr2End.Normalize();

	Vector2D curr2Nei(neighbour->GetX() - current->GetX(), neighbour->GetY() - current->GetY());
	curr2Nei.Normalize();

	return (1.0f - Vector2D::DotProduct(curr2End, curr2Nei)) * 0.01f;
}

Weights::~Weights() {
}
