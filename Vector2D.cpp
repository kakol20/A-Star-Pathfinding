#include "Vector2D.h"
#include <cmath>

Vector2D::Vector2D() {
	m_x = 0;
	m_y = 0;
}

Vector2D::Vector2D(float x, float y) {
	m_x = x;
	m_y = y;
}

Vector2D::Vector2D(int x, int y) {
	m_x = (float)x;
	m_y = (float)y;
}

void Vector2D::Normalize() {
	float d = sqrt((m_x * m_x) + (m_y * m_y));

	*this /= d;
}

Vector2D::~Vector2D() {
}

Vector2D Vector2D::operator/=(const Vector2D& other) {
	m_x /= other.m_x;
	m_y /= other.m_y;

	return *this;
}

Vector2D Vector2D::operator/=(const float& div) {
	m_x /= div;
	m_y /= div;
	return *this;
}

float Vector2D::DotProduct(const Vector2D& first, const Vector2D& second) {
	return (first.m_x * second.m_x) + (first.m_y * second.m_y);
}
