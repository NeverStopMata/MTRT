#pragma once
#include "vec3.h"
class ONB
{
public:
	ONB() {}
	inline Vec3 operator[](int i) const { return axis_[i]; }
	Vec3 u() const { return axis_[0]; }
	Vec3 v() const { return axis_[1]; }
	Vec3 w() const { return axis_[2]; }
	Vec3 TransformToWorld(float a, float b, float c)const { return a * u() + b * v() + c * w(); }
	Vec3 TransformToWorld(const Vec3& a)const { return a.x() * u() + a.y() * v() + a.z() * w(); }
	void BuildFormW(const Vec3& n) {
		axis_[2] = Normalize(n);
		Vec3 a;
		if (fabs(w().x()) > 0.9)
			a = Vec3(0, 1, 0);
		else
			a = Vec3(1, 0, 0);
		axis_[1] = Normalize(Cross(w(), a));
		axis_[0] = Cross(v(), w());
	}
	Vec3 axis_[3];
};