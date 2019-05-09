#pragma once
#include "hitable.h"
#include "hitable_list.h"
#include "xy_rect.h"
#include "yz_rect.h"
#include "xz_rect.h"
#include "flip_normals.h"
class Box :public Hitable {
public: 
	Box(const Vec3& p0, const Vec3& p1, std::shared_ptr<Material> mat_ptr_);
	~Box() {
		delete list_ptr_;
	}
	virtual bool Hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const;
	virtual bool GetBoundingBox(float t0, float t1, Aabb& box) const {
		box = Aabb(p_min_, p_max_);
		return true;
	}
	Vec3 p_min_, p_max_;
	Hitable* list_ptr_;
};
Box::Box(const Vec3& p0, const Vec3& p1, std::shared_ptr<Material> mat_ptr) {
	p_min_ = p0;
	p_max_ = p1;
	std::vector<std::shared_ptr<Hitable>> hitable_array;
	hitable_array.push_back(std::make_shared<XYRect>(p0.x(), p1.x(), p0.y(), p1.y(), p1.z(), mat_ptr));
	hitable_array.push_back(std::make_shared<FlipNormals>(new XYRect(p0.x(), p1.x(), p0.y(), p1.y(), p0.z(), mat_ptr)));
	hitable_array.push_back(std::make_shared<XZRect>(p0.x(), p1.x(), p0.z(), p1.z(), p1.y(), mat_ptr));
	hitable_array.push_back(std::make_shared<FlipNormals>(new XZRect(p0.x(), p1.x(), p0.z(), p1.z(), p0.y(), mat_ptr)));
	hitable_array.push_back(std::make_shared<YZRect>(p0.y(), p1.y(), p0.z(), p1.z(), p1.x(), mat_ptr));
	hitable_array.push_back(std::make_shared<FlipNormals>(new YZRect(p0.y(), p1.y(), p0.z(), p1.z(), p0.x(), mat_ptr)));
	list_ptr_ = new Hitable_list(hitable_array);
	hitable_array.clear();
	hitable_array.shrink_to_fit();
}

bool Box::Hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const {
	return list_ptr_->Hit(r, t_min, t_max, rec);
}