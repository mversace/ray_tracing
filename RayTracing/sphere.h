#pragma once

#include "rayMath.h"
#include "ray.h"

struct HitRec {
	float t;
	Math::Vec3 p;
	Math::Vec3 normal;
};

class Sphere {
public:
	Sphere(const Math::Vec3& center, float radius)
	{
		_center = center;
		_radius = radius;
	}

	// 判断射线是否与球相交
	// 射线上的点 f(t) = rayOrigin + t * rayDirection;
	// 判定一个点是否在球体表面 dot(p-c, p-c) = r * r;
	// dot(f(t)-c, f(t)-c) = r * r;
	// t*t*dot(rayDirection, rayDirection) + 2*t*dot(rayDirection, rayOrigin-center)
	// + dot(rayOrigin-center, rayOrigin-center) - r*r = 0;
	// ax^2 + 2bx + c = 0
	// 有没有根 b^2 - 4ac >= 0
	// a = dot(rayDirection, rayDirection)
	// b = dot(rayDirection, rayOrigin - center)
	// c = dot(rayOrigin-center, rayOrigin - center) - r * r
	bool hit(const Ray& r, float fMin, float fMax, HitRec& rec) const
	{
		Math::Vec3 oc = r.origin() - _center;
		Math::Vec3 direction = r.direction();

		float a = Math::dot(direction, direction);
		float b = 2.0f * Math::dot(direction, oc);
		float c = Math::dot(oc, oc) - _radius * _radius;
		float dis = b * b - 4.0f * a * c;
		if (dis > 0) {
			float tmp = (-b - sqrt(dis)) / (2.0f * a);
			if (fMin < tmp && tmp < fMax) {
				rec.t = tmp;
				rec.p = r.pointAtParameter(tmp);
				rec.normal = rec.p - _center;
				rec.normal.normalize();
				return true;
			}

			tmp = (-b + sqrt(dis)) / (2.0f * a);
			if (fMin < tmp && tmp < fMax) {
				rec.t = tmp;
				rec.p = r.pointAtParameter(tmp);
				rec.normal = rec.p - _center;
				rec.normal.normalize();
				return true;
			}
		}

		return false;
	}

private:
	Math::Vec3 _center;
	float _radius;
};