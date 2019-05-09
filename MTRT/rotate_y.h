#pragma once
#include "hitable.h"
class RotateY :public Hitable {
public:
	RotateY(Hitable* hitable_ptr, float angle);
	~RotateY() {
		delete hitable_ptr_;
	}
	virtual bool Hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const;
	virtual bool GetBoundingBox(float t0, float t1, Aabb& box) const {
		box = bbox_;
		return has_box_;
	}
	Hitable * hitable_ptr_;
	float sin_theta_;
	float cos_theta_;
	bool has_box_;
	Aabb bbox_;
};
RotateY::RotateY(Hitable* hitable_ptr, float angle): hitable_ptr_(hitable_ptr){
	float radians = (M_PI / 180.) * angle;
	sin_theta_ = sin(radians);
	cos_theta_ = cos(radians);
	has_box_ = hitable_ptr_->GetBoundingBox(0, 1, bbox_);
	Vec3 min(FLT_MAX, FLT_MAX, FLT_MAX);
	Vec3 max(-FLT_MAX, -FLT_MAX, -FLT_MAX);
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			for (int k = 0; k < 2; k++) {
				float x = i * bbox_.GetMaxCorner().x() + (1 - i)* bbox_.GetMinCorner().x();
				float y = j * bbox_.GetMaxCorner().y() + (1 - j)* bbox_.GetMinCorner().y();
				float z = k * bbox_.GetMaxCorner().z() + (1 - k)* bbox_.GetMinCorner().z();
				float new_x =  cos_theta_ * x + sin_theta_ * z;
				float new_z = -sin_theta_ * x + cos_theta_ * z;
				Vec3 tester(new_x, y, new_z);
				for (int c = 0; c < 3; c++) {
					if (tester[c] > max[c])
						max[c] = tester[c];
					if (tester[c] < min[c])
						min[c] = tester[c];
				}
			}
		}
	}
	bbox_ = Aabb(min, max);
}
bool RotateY::Hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const {
	Vec3 origin = r.GetOrigin();
	Vec3 direction = r.GetDirection();
	origin[0] = cos_theta_ * r.GetOrigin()[0] - sin_theta_ * r.GetOrigin()[2];
	origin[2] = sin_theta_ * r.GetOrigin()[0] + cos_theta_ * r.GetOrigin()[2]; 
	direction[0] = cos_theta_ * r.GetDirection()[0] - sin_theta_ * r.GetDirection()[2];
	direction[2] = sin_theta_ * r.GetDirection()[0] + cos_theta_ * r.GetDirection()[2];
	Ray rotated_r(origin, direction, r.GetTime());
	if (hitable_ptr_->Hit(rotated_r, t_min, t_max, rec)) {
		Vec3 p = rec.pos;
		Vec3 normal = rec.normal;
		p[0] =  cos_theta_ * rec.pos[0] + sin_theta_ * rec.pos[2];
		p[2] = -sin_theta_ * rec.pos[0] + cos_theta_ * rec.pos[2];
		normal[0] = cos_theta_ * rec.normal[0] + sin_theta_ * rec.normal[2];
		normal[2] = -sin_theta_ * rec.normal[0] + cos_theta_ * rec.normal[2];
		rec.pos = p;
		rec.normal = normal;
		return true;
	}
	else {
		return false;
	}
}