#pragma once
#include "vec3.h"
class Ray
{
public:
	Ray() {}
	Ray(const Vec3& a, const Vec3& b, float ti = 0.0) :origin_(a), direction_(b), time_(ti){}
	Vec3 GetOrigin()    const { return origin_; }
	Vec3 GetDirection() const { return direction_; }
	float GetTime() const { return time_; }
	Vec3 GetPointAtParameter(float t) const { return origin_ + t * direction_; }
	
	Vec3  origin_;
	Vec3  direction_;
	float time_;
};