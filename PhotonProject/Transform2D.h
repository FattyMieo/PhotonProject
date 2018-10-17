#pragma once

#include "Vector2.h"
#include "Matrix.h"

class Transform2D
{
public:
	Vector2 position;
	float rotation;
	Vector2 scale;

	Transform2D();
	~Transform2D();
};
