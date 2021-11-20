#pragma once
class Vector2D {
public:
	Vector2D();
	Vector2D(float x, float y);
	Vector2D(int x, int y);

	void Normalize();

	~Vector2D();

public:
	Vector2D operator/=(const Vector2D& other);
	Vector2D operator/=(const float& div);

public:

	static float DotProduct(const Vector2D& first, const Vector2D& second);

private:
	float m_x;
	float m_y;
};

