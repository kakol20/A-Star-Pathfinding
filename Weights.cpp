#include "Weights.h"
#include "Vector2D.h"

Weights::Weights() {
}

float Weights::AvoidTurning(Node* current, Node* neighbour) {
	if (current->GetParent() != nullptr) {
		Vector2D forward(current->GetX() - current->GetParent()->GetX(), current->GetY() - current->GetParent()->GetY());
		forward.Normalize();

		Vector2D neighbourDir(neighbour->GetX() - current->GetX(), neighbour->GetY() - current->GetY());
		neighbourDir.Normalize();

		return (1.0f - Vector2D::DotProduct(forward, neighbourDir)) * 0.1f;
	}
	return 0.0f;
}

float Weights::StraightTowards(Node* current, Node* neighbour, Node* end) {
	// Current to End vector
	Vector2D curr2End(end->GetX() - current->GetX(), end->GetY() - current->GetY());
	curr2End.Normalize();

	Vector2D curr2Nei(neighbour->GetX() - current->GetX(), neighbour->GetY() - current->GetY());
	curr2Nei.Normalize();

	return (1.0f - Vector2D::DotProduct(curr2End, curr2Nei)) * 0.1f;
}

Weights::~Weights() {
}
