#pragma once
#include "hitable.h"
class BVHNode : public Hitable {
public:
	BVHNode() {}
	BVHNode(Hitable**l, int n, float time0, float time1);
	virtual bool Hit(const Ray& r, float tmin, float tmax, HitRecord& rec) const;
	virtual bool GetBoundingBox(float t0, float t1, Aabb& box) const;
	Hitable* left_;
	Hitable* right_;
	Aabb box_;

};
BVHNode::BVHNode(Hitable** l, int n, float time0, float time1)
{
	int axis = int(3 * ((double)rand() / RAND_MAX));
	if (axis == 0)
		qsort(l, n, sizeof(Hitable *), box_x_compare);
	else if(axis == 1)
		qsort(l, n, sizeof(Hitable *), box_y_compare);
	else
		qsort(l, n, sizeof(Hitable *), box_y_compare);
	if (n == 1)
		left_ = right_ = l[0];
	else if (n == 2) {
		left_ = l[0];
		right_ = l[1];
	}
	else {
		left_ = new BVHNode(l, n / 2, time0, time1);
		right_ = new BVHNode(l + n / 2,n - n/2, time0, time1);
	}
	Aabb box_left, box_right;
	if (!left_->GetBoundingBox(time0, time1, box_left) || !right_->GetBoundingBox(time0, time1, box_right))
		std::cerr << "no bounding box in bvh_node constructor" << std::endl;
	box_ = MergeTwoBoundingBoxes(box_left, box_right);
}
bool BVHNode::GetBoundingBox(float t0, float t1, Aabb& box) const {
	box = box_;
	return true;
}

bool BVHNode::Hit(const Ray& r, float tmin, float tmax, HitRecord& rec) const {
	if (box_.Hit(r, tmin, tmax)) {
		HitRecord left_rec, right_rec;
		bool hit_left = left_->Hit(r, tmin, tmax, left_rec);
		bool hit_right = right_->Hit(r, tmin, tmax, right_rec);
		if (hit_left && hit_left) {
			rec = left_rec.t < right_rec.t ? left_rec : right_rec;
			return true;
		}
		else if (hit_left) {
			rec = left_rec;
			return true;
		}
		else if (hit_right) {
			rec = right_rec;
			return true;
		}
		else
			return false;
	}
	else
		return false;
}

