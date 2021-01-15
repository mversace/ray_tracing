#pragma once

#include <vector>
#include "sphere.h"
#include "ray.h"

class World {
public:
	World() = default;

	void addObject(const Sphere& obj)
	{
		_vec.push_back(obj);
	}

	bool hit(const Ray& r, float fMin, float fMax, HitRec& rec) const
	{
		HitRec hitRecTemp;
		bool isHit = false;
		float fMaxTmp = fMax;
		
		for (const auto& obj : _vec) {
			if (obj.hit(r, fMin, fMaxTmp, hitRecTemp)) {
				isHit = true;
				fMaxTmp = hitRecTemp.t;
				rec = hitRecTemp;
			}
		}

		return isHit;
	}

private:
	std::vector<Sphere> _vec;
};