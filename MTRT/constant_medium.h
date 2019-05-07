#pragma once
#include "hitable.h"
#include "texture.h"
#include "isotropic.h"
class ConstantMedium :public Hitable {
public:
	ConstantMedium(Hitable * boundary, float density, Texture* albedo) :boundary_(boundary), density_(density) {
		phase_func_ = new Isotropic(albedo);
	}
	virtual bool Hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const;
	virtual bool GetBoundingBox(float t0, float t1, Aabb& box) const {
		return boundary_->GetBoundingBox(t0, t1, box);
	}
	Hitable* boundary_;
	float density_;
	Material* phase_func_;
};

bool ConstantMedium::Hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const {
	HitRecord rec1, rec2;
	if (boundary_->Hit(r, -FLT_MAX, FLT_MAX, rec1)) {
		if (boundary_->Hit(r, rec1.t + FLOAT_EPSILON, FLT_MAX, rec2)) {
			if (rec1.t < t_min)
				rec1.t = t_min;
			if (rec2.t > t_max)
				rec2.t = t_max;
			if (rec1.t >= rec2.t)
				return false;
			if (rec1.t < 0)
				rec1.t = 0;
			float ray_direction_length = r.GetDirection().GetLength();
			float distance_inside_boundary = (rec2.t - rec1.t) * ray_direction_length;
			float hit_distance = -(1 / density_)*log(GetRandom01());
			if (hit_distance < distance_inside_boundary) {
				rec.t = rec1.t + hit_distance / ray_direction_length;
				rec.pos = r.GetPointAtParameter(rec.t);
				rec.normal = Vec3(1, 0, 0);
				rec.mat_ptr = phase_func_;
				return true;
			}
		}
	}
	return false;
}