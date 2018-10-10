#pragma once

#include <cstdlib>

namespace MathExtension
{
	static int GetRandomInt(int min, int max)
	{
		if (min == max)
			return min;
		else if (max > min)
		{
			int temp = min;
			min = max;
			max = temp;
		}

		int diff = max - min;
		int r = rand() % diff;

		return min + r;
	}

	static float GetRandomFloat(float min, float max, int dr = 100)
	{
		if (min == max)
			return min;
		else if (max > min)
		{
			float temp = min;
			min = max;
			max = temp;
		}

		float diff = max - min;
		float random01 = ((float)rand()) / (float)RAND_MAX; //Random float from 0.0f to 1.0f
		float r = random01 * diff;

		return min + r;
	}

	static int Lerp(int a, int b, float t)
	{
		if (a == b)
			return a;

		return (int)(((1.0f - t) * a) + (t * b));
	}

	static float Lerp(float a, float b, float t)
	{
		if (a == b)
			return a;

		return ((1.0f - t) * a) + (t * b);
	}
}