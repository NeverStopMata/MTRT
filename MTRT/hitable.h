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
	float u;
	float v;
};

class Hitable {
public:
	virtual bool Hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const = 0;
	virtual bool GetBoundingBox(float t0, float t1, Aabb& box) const = 0;

	static bool CompareBoxX(Hitable* a, Hitable* b);
	static bool CompareBoxY(Hitable* a, Hitable* b);
	static bool CompareBoxZ(Hitable* a, Hitable* b);
};

bool Hitable::CompareBoxX(Hitable* a, Hitable* b)
{
	Aabb box_left, box_right;
	if (!a->GetBoundingBox(0, 0, box_left) || !b->GetBoundingBox(0, 0, box_right))
		std::cerr << "no bounding box in bvh_node constructor" << std::endl;
	if (box_left.GetMinCorner().x() - box_right.GetMinCorner().x() < 0.0)
		return true;
	else
		return false;
}


bool Hitable::CompareBoxY(Hitable* a, Hitable* b) {
	Aabb box_left, box_right;
	if (!a->GetBoundingBox(0, 0, box_left) || !b->GetBoundingBox(0, 0, box_right))
		std::cerr << "no bounding box in bvh_node constructor" << std::endl;
	if (box_left.GetMinCorner().y() - box_right.GetMinCorner().y() < 0.0)
		return true;
	else
		return false;
}

bool Hitable::CompareBoxZ(Hitable* a, Hitable* b) {
	Aabb box_left, box_right;
	if (!a->GetBoundingBox(0, 0, box_left) || !b->GetBoundingBox(0, 0, box_right))
		std::cerr << "no bounding box in bvh_node constructor" << std::endl;
	if (box_left.GetMinCorner().z() - box_right.GetMinCorner().z() < 0.0)
		return true;
	else
		return false;
}
