#pragma once

#include "MathExtension.h"

class Vector2
{
public:
	float x;
	float y;

	Vector2();
	Vector2(float f);
	Vector2(float x, float y);
	~Vector2();

	void SetEqual(float f);
	float GetSquaredMagnitude();
	Vector2 operator+(const Vector2& vec);
	Vector2& operator+=(const Vector2& vec);
	Vector2& operator*=(const float f);
	Vector2& operator*=(const Vector2& vec);
	Vector2 operator*(const float f);
	Vector2 operator*(const Vector2& vec);

	static Vector2 Random(Vector2 min, Vector2 max);
	static Vector2 Lerp(Vector2 a, Vector2 b, float t);
};