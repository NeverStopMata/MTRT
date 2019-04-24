#pragma once
#include "ray.h"
#include "aabb.h"
class Material;
struct  HitRecord
{
	float t;
	Vec3 pos;
	Vec3 normal;
	Material* mat_ptr;
};

class Hitable {
public:
	virtual bool Hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const = 0;
	virtual bool GetBoundingBox(float t0, float t1, Aabb& box) const = 0;
};
