#pragma once

#include "rayMath.h"

class Ray {
public:
	Ray() = default;
	Ray(const Math::Vec3& a, const Math::Vec3& b)
	{
		A = a;
		B = b;
		B.normalize();
	}

	auto origin() const { return A; }
	auto direction() const { return B; }
	auto pointAtParameter(float t) const { return A + t * B; }

private:
	Math::Vec3 A;
	Math::Vec3 B;
};