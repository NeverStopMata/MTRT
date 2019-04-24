#pragma once
#include "Hitable.h"
#include "metal.h"
#include "lambertian.h"
#include "dielectric.h"
class Hitable_list : public Hitable {
public:
	Hitable_list() {}
	Hitable_list(Hitable **l, int n) { list_ = l; list_size_ = n; }
	virtual bool Hit(const Ray& r, float tmin, float tmax, HitRecord& rec) const;
	virtual bool GetBoundingBox(float t0, float t1, Aabb& box) const;
	Hitable **list_;
	int list_size_;
};
bool Hitable_list::Hit(const Ray& r, float t_min, float t_max, HitRecord& rec)const {
	HitRecord temp_rec;
	bool hit_anything = false;
	double closest_so_far = t_max;
	for (int i = 0; i < list_size_; i++) {
		if (list_[i]->Hit(r, t_min, closest_so_far, temp_rec)) {
			hit_anything = true;
			closest_so_far = temp_rec.t;
			rec = temp_rec;
		}
	}
	return hit_anything;
}
bool Hitable_list::GetBoundingBox(float t0, float t1, Aabb& box) const {
	if (list_size_ < 1) return false;
	Aabb temp_box;
	bool first_true = list_[0]->GetBoundingBox(t0, t1, temp_box);
	if (!first_true)
		return false;
	else
		box = temp_box;
	for (int i = 1; i < list_size_; i++) {
		if (list_[i]->GetBoundingBox(t0, t1, temp_box)) {
			box = MergeTwoBoundingBoxes(box, temp_box);
		}
		else
			return false;
	}
	return true;
}