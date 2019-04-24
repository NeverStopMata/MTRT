#pragma 
#include "vec3.h"
#include "ray.h"
#include <algorithm>
inline float ffmin(float a, float b) { return a < b ? a : b; }
inline float ffmax(float a, float b) { return a > b ? a : b; }
class Aabb {
public:
	Aabb() {}
	Aabb(const Vec3& min, const Vec3& max) :max_(max), min_(min) {}
	Vec3 GetMinCorner() const { return min_; }
	Vec3 GetMaxCorner() const { return max_; }
	bool Hit(const Ray& r, float tmin, float tmax)const {
		for (int i = 0; i < 3; i++) {
			float inv_direc_comp = 1.0f / r.GetDirection()[i];
			float t0 = (min_[i] - r.GetOrigin()[i]) * inv_direc_comp;
			float t1 = (max_[i] - r.GetOrigin()[i]) * inv_direc_comp;
			if (inv_direc_comp < 0.0f)
				std::swap(t0, t1);
			tmin = ffmax(t0, tmin);
			tmax = ffmin(t1, tmax);
			if (tmax <= tmin)
				return false;
		}
		return true;
	}
	Vec3 min_, max_;
};

Aabb MergeTwoBoundingBoxes(Aabb& box0, Aabb& box1) {
	Vec3 new_min(ffmin(box0.GetMinCorner().x(), box1.GetMinCorner().x()),
			   ffmin(box0.GetMinCorner().y(), box1.GetMinCorner().y()),
			   ffmin(box0.GetMinCorner().z(), box1.GetMinCorner().z()));

	Vec3 new_max(ffmax(box0.GetMinCorner().x(), box1.GetMinCorner().x()),
				 ffmax(box0.GetMinCorner().y(), box1.GetMinCorner().y()),
				 ffmax(box0.GetMinCorner().z(), box1.GetMinCorner().z()));
	return Aabb(new_min, new_max);
}

int box_x_compare(const void* a, const void* b) {
	Aabb box_left, box_right;
	Hitable* hitable_a = *((Hitable**)a);
	Hitable* hitable_b = *((Hitable**)b);
	if(!hitable_a->GetBoundingBox(0,0,box_left)|| !hitable_b->GetBoundingBox(0, 0, box_right))
		std::cerr << "no bounding box in bvh_node constructor" << std::endl;
	if (box_left.GetMinCorner().x() - box_right.GetMinCorner().x() < 0.0)
		return -1;
	else
		return 1;
}

int box_y_compare(const void* a, const void* b) {
	Aabb box_left, box_right;
	Hitable *hitable_a = *(Hitable**)a;
	Hitable *hitable_b = *(Hitable**)b;
	if (!hitable_a->GetBoundingBox(0, 0, box_left) || !hitable_b->GetBoundingBox(0, 0, box_right))
		std::cerr << "no bounding box in bvh_node constructor" << std::endl;
	if (box_left.GetMinCorner().y() - box_right.GetMinCorner().y() < 0.0)
		return -1;
	else
		return 1;
}

int box_z_compare(const void* a, const void* b) {
	Aabb box_left, box_right;
	Hitable *hitable_a = *(Hitable**)a;
	Hitable *hitable_b = *(Hitable**)b;
	if (!hitable_a->GetBoundingBox(0, 0, box_left) || !hitable_b->GetBoundingBox(0, 0, box_right))
		std::cerr << "no bounding box in bvh_node constructor" << std::endl;
	if (box_left.GetMinCorner().z() - box_right.GetMinCorner().z() < 0.0)
		return -1;
	else
		return 1;
}