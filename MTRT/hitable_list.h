#pragma once
#include "Hitable.h"
#include "metal.h"
#include "lambertian.h"
#include "dielectric.h"
#include <vector>
class Hitable_list : public Hitable {
public:
	Hitable_list() {}
	~Hitable_list() {
		list_.clear();
		list_.shrink_to_fit();
	}
	Hitable_list(const std::vector<std::shared_ptr<Hitable>>& list):list_(list) {}
	virtual bool Hit(const Ray& r, float tmin, float tmax, HitRecord& rec) const;
	virtual bool GetBoundingBox(float t0, float t1, Aabb& box) const;
	virtual float GetPDFValue(const Vec3& origin, const Vec3& dir) const;
	virtual Vec3 GetRandomDirToSelf(const Vec3& origin) const;
	std::vector<std::shared_ptr<Hitable>> list_;
};
bool Hitable_list::Hit(const Ray& r, float t_min, float t_max, HitRecord& rec)const {
	HitRecord temp_rec;
	bool hit_anything = false;
	float closest_so_far = t_max;
	for (int i = 0; i < list_.size(); i++) {
		if (list_[i]->Hit(r, t_min, closest_so_far, temp_rec)) {
			hit_anything = true;
			closest_so_far = temp_rec.t;
			rec = temp_rec;
		}
	}
	return hit_anything;
}
bool Hitable_list::GetBoundingBox(float t0, float t1, Aabb& box) const {
	if (list_.size() < 1) return false;
	Aabb temp_box;
	bool first_true = list_[0]->GetBoundingBox(t0, t1, temp_box);
	if (!first_true)
		return false;
	else
		box = temp_box;
	for (int i = 1; i < list_.size(); i++) {
		if (list_[i]->GetBoundingBox(t0, t1, temp_box)) {
			box = MergeTwoBoundingBoxes(box, temp_box);
		}
		else
			return false;
	}
	return true;
}

float Hitable_list::GetPDFValue(const Vec3& origin, const Vec3& dir) const {
	float weight = 1. / list_.size();
	float sum = 0;
	for (auto itor = list_.begin(); itor != list_.end(); itor++) {
		sum += weight * (*itor)->GetPDFValue(origin, dir);
	}
	return sum;
}
Vec3 Hitable_list::GetRandomDirToSelf(const Vec3& origin) const {
	int idx = std::min(int(GetRandom01()  * list_.size()), int(list_.size() - 1));
	return list_[idx]->GetRandomDirToSelf(origin);
}