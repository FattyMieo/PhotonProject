#include "Vector2.h"

Vector2::Vector2()
{
	x = 0;
	y = 0;
}

Vector2::Vector2(float f)
{
	SetEqual(f);
}

Vector2::Vector2(float x, float y)
{
	this->x = x;
	this->y = y;
}

Vector2::~Vector2()
{

}

void Vector2::SetEqual(float f)
{
	this->x = f;
	this->y = f;
}

float Vector2::GetSquaredMagnitude()
{
	return (this->x * this->x) + (this->y * this->y);
}

Vector2 Vector2::operator+(const Vector2& vec)
{
	return Vector2(this->x + vec.x, this->y + vec.y);
}

Vector2& Vector2::operator+=(const Vector2& vec)
{
	this->x += vec.x;
	this->y += vec.y;
	return *this;
}

Vector2& Vector2::operator*=(const float f)
{
	this->x *= f;
	this->y *= f;
	return *this;
}

Vector2& Vector2::operator*=(const Vector2& vec)
{
	this->x *= vec.x;
	this->y *= vec.y;
	return *this;
}

Vector2 Vector2::operator*(const float f)
{
	return Vector2(this->x * f, this->y * f);
}

Vector2 Vector2::operator*(const Vector2& vec)
{
	return Vector2(this->x * vec.x, this->y * vec.y);
}

Vector2 Vector2::Random(Vector2 min, Vector2 max)
{
	Vector2 ret;
	ret.x = MathExtension::GetRandomFloat(min.x, max.x);
	ret.y = MathExtension::GetRandomFloat(min.y, max.y);

	return ret;
}

Vector2 Vector2::Lerp(Vector2 a, Vector2 b, float t)
{
	Vector2 ret;
	ret.x = MathExtension::Lerp(a.x, b.x, t);
	ret.y = MathExtension::Lerp(a.y, b.y, t);

	return ret;
}