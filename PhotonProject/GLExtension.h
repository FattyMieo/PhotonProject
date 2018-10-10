#pragma once

#include "GLFW/glfw3.h"
#include "Vector2.h"

namespace GLExtension
{
	static GLfloat* GetUV(Vector2 min, Vector2 max)
	{
		return new GLfloat[12]
		{
			min.x, min.y,
			min.x, max.y,
			max.x, min.y,

			min.x, max.y,
			max.x, min.y,
			max.x, max.y
		};
	}
}