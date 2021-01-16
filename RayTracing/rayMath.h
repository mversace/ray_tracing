#pragma once
#include <math.h>

namespace Math {
	class Vec3 {
	public:
		Vec3() = default;
		Vec3(float e1, float e2, float e3)
		{
			e[0] = e1;
			e[1] = e2;
			e[2] = e3;
		}

		inline float x() const { return e[0]; }
		inline float y() const { return e[1]; }
		inline float z() const { return e[2]; }

		inline float length() const { return static_cast<float>(sqrt(e[0] * e[0] + e[1] * e[1] + e[2] * e[2])); }
		void normalize()
		{
			auto len = length();
			e[0] /= len;
			e[1] /= len;
			e[2] /= len;
		}

		float e[3];
	};

	inline Vec3 operator*(float t, const Vec3& v) { return Vec3(t * v.e[0], t * v.e[1], t * v.e[2]); }
	inline Vec3 operator+(const Vec3& v1, const Vec3& v2) { return Vec3(v1.e[0] + v2.e[0], v1.e[1] + v2.e[1], v1.e[2] + v2.e[2]); }
	inline Vec3 operator+=(Vec3& v1, const Vec3& v2) { v1 = v1 + v2;  return v1; }
	inline Vec3 operator-(const Vec3& v1, const Vec3& v2) { return Vec3(v1.e[0] - v2.e[0], v1.e[1] - v2.e[1], v1.e[2] - v2.e[2]); }
	inline Vec3 operator/=(Vec3& v1, float t) { v1.e[0] /= t; v1.e[1] /= t; v1.e[2] /= t; return v1; }

	inline float dot(const Vec3& v1, const Vec3& v2)
	{
		return v1.e[0] * v2.e[0] + v1.e[1] * v2.e[1] + v1.e[2] * v2.e[2];
	}
}