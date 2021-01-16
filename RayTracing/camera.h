#pragma once

#include "ray.h"

class Camera {
public:
	Camera() = default;

	Ray getRay(float u, float v)
	{
		return Ray(origin, lowerLeftCorner + u * horizontal + v * vertical);
	}

private:
	Math::Vec3 lowerLeftCorner{ -2.0f, -1.0f, -1.0f };
	Math::Vec3 horizontal{ 4.0f, 0.0f, 0.0f };
	Math::Vec3 vertical{ 0.0f, 2.0f, 0.0f };
	Math::Vec3 origin{ 0.0f, 0.0f, 0.0f };
};