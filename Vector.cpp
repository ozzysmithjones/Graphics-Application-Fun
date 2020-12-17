#include "Vector.h"

Vector::Vector(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

Vector::Vector(XMFLOAT3 val)
{
	this->x = val.x;
	this->y = val.y;
	this->z = val.z;
}

Vector Vector::Cross(Vector a, Vector b)
{
	Vector x = Vector(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
	return x;
}

float Vector::Dot(Vector a, Vector b)
{
	return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

Vector Vector::Clamp(Vector a, float maxMagnitude)
{
	if (maxMagnitude == 0.0f)
		return Vector(0, 0, 0);

	float val = a.magnitude();
	if (val < maxMagnitude)
		return a;
	float portion = abs(maxMagnitude) / val;
	return Vector(a.x * portion, a.y * portion, a.z * portion);
	
}

Vector Vector::Normalised()
{
	float distance = magnitude();
	return Vector(x / distance,y / distance,z / distance);
}

 