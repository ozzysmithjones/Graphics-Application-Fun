#pragma once
#include <cmath>
#include <Windows.h>
#include <d3d11_1.h>
#include <directxmath.h>

using namespace DirectX;

struct Vector
{
public:
		float x, y, z;
		Vector(float x, float y, float z);
		Vector(XMFLOAT3 val);

		static Vector Cross(Vector a, Vector b);
		static float Dot(Vector a, Vector b);
		static Vector Clamp(Vector a, float maxMagnitude);

		inline float magnitude() { return sqrtf(x * x + y * y + z * z); }

		Vector Normalised();

		operator XMFLOAT3() { return XMFLOAT3(x, y, z); };

		Vector operator*(float value) { return Vector(x * value, y * value, z * value); };
		Vector operator/(float value) { return Vector(x / value, y / value, z / value); };

		Vector operator+(Vector b) { return Vector(x + b.x, y + b.y, z + b.z); };
		Vector operator-(Vector b) { return Vector(x - b.x, y - b.y, z - b.z); };

		Vector operator+(XMFLOAT3 b) { return Vector(x + b.x, y + b.y, z + b.z); };
		Vector operator-(XMFLOAT3 b) { return Vector(x - b.x, y - b.y, z - b.z); };
};

