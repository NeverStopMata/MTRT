#pragma once
#include <vector>
#include <tuple>
#include "hitable.h"
#include "aabb.h"

class Vertex {
public:
	Vertex() {
		pos_ = Vec3(0, 0, 0);
		normal_ = Vec3(0, 0, 0);
		u_ = 0;
		v_ = 0;
	}
	Vertex(Vec3 pos, Vec3 normal, float u, float v) :pos_(pos), u_(u), v_(v), normal_(normal) {}
	Vec3 pos() const { return pos_; }
	Vec3 normal() const { return normal_; }
	float u() const { return u_; }
	float v() const { return v_; }
	friend inline Vertex operator*(const Vertex& vertex, float weight);
	inline Vertex& operator+=(const Vertex& vertex) {
		this->normal_ += vertex.normal();
		this->pos_ += vertex.pos();
		this->u_ += vertex.u();
		this->v_ += vertex.v();
		return *this;
	}

	bool operator==(const Vertex& other) const {
		return (other.pos_ == this->pos_
			&& other.u_ == this->u_
			&& other.v_ == this->v_
			&& other.normal_ == this->normal_);
	}



	Vec3 pos_;
	float u_;
	float v_;
	Vec3 normal_;
};

namespace std {
	template<> struct hash<Vertex> {
		size_t operator()(Vertex const& vertex) const {
			return (((hash<Vec3>()(vertex.pos_) ^
				(hash<float>()(vertex.u_) << 1)) >> 1) ^
				(hash<float>()(vertex.v_) << 1) >> 1) ^
				(hash<Vec3>()(vertex.normal_));
		}
	};
}
inline Vertex operator*(const Vertex& vertex, float weight) {
	Vertex ret;
	ret.normal_ = vertex.normal_ * weight;
	ret.pos_ = vertex.pos_ * weight;
	ret.u_ = vertex.u_ * weight;
	ret.v_ = vertex.v_ * weight;
	return ret;
}

struct  Plane {
	Vec3 normal;
	float k;
};


Vertex InterpolateVertices(std::vector<std::tuple<Vertex, float>>& vertices_weights) {
	Vertex ret_vertex;
	std::tuple<int, float> a;
	for (auto itor = vertices_weights.begin(); itor != vertices_weights.end(); itor++) {
		ret_vertex += std::get<0>(*itor) * std::get<1>(*itor);
	}
	return ret_vertex;
}

enum DenominatorType
{
	xy,
	yz,
	zx,
};
class Triangle :public Hitable {
public:
	Triangle() {}
	~Triangle(){
	}
	Triangle(const Vertex& v0, const Vertex& v1, const Vertex& v2, std::shared_ptr<Material> mat_ptr) :v0_(v0), v1_(v1), v2_(v2), mat_ptr_(mat_ptr) {
		try {
			vec_p0p1_ = v1_.pos() - v0_.pos();
			vec_p0p2_ = v2_.pos() - v0_.pos();
			denominator_type_ = DenominatorType::xy;
			while (abs((vec_p0p1_[denominator_type_] * vec_p0p2_[(denominator_type_ + 1) % 3] - vec_p0p1_[(denominator_type_ + 1) % 3] * vec_p0p2_[denominator_type_])) < FLOAT_EPSILON) {
				denominator_type_ = DenominatorType((denominator_type_ + 1) % 3);
			}
			//std::cout << denominator_type_ << std::endl;
			denominator_ = 1.0 / (vec_p0p1_[denominator_type_] * vec_p0p2_[(denominator_type_ + 1) % 3] - vec_p0p1_[(denominator_type_ + 1) % 3] * vec_p0p2_[denominator_type_]);
			auto tmp_min = Vec3(ffmin(ffmin(v0_.pos().x(), v1_.pos().x()), v2_.pos().x()),
								ffmin(ffmin(v0_.pos().y(), v1_.pos().y()), v2_.pos().y()),
								ffmin(ffmin(v0_.pos().z(), v1_.pos().z()), v2_.pos().z()));
			tmp_min -= Vec3(FLOAT_EPSILON, FLOAT_EPSILON, FLOAT_EPSILON);
			auto tmp_max = Vec3(ffmax(ffmax(v0_.pos().x(), v1_.pos().x()), v2_.pos().x()),
								ffmax(ffmax(v0_.pos().y(), v1_.pos().y()), v2_.pos().y()),
								ffmax(ffmax(v0_.pos().z(), v1_.pos().z()), v2_.pos().z()));
			tmp_max += Vec3(FLOAT_EPSILON, FLOAT_EPSILON, FLOAT_EPSILON);
			bbx_ = Aabb(tmp_min, tmp_max);
			plane_.normal = Normalize(Cross(-vec_p0p2_, vec_p0p1_));
			plane_.k = Dot(v0_.pos(), plane_.normal);
		}
		catch (std::out_of_range& vec_idx_e) {
			std::cerr << vec_idx_e.what() << std::endl;
		}

	}
	virtual bool Hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const;
	virtual bool GetBoundingBox(float t0, float t1, Aabb& box) const;
	Aabb bbx_;
	Vertex v0_;
	Vertex v1_;
	Vertex v2_;
	Plane plane_;
	Vec3 vec_p0p1_;
	Vec3 vec_p0p2_;
	float denominator_;
	DenominatorType denominator_type_;
	std::shared_ptr<Material> mat_ptr_;
};

bool Triangle::Hit(const Ray& r, float t_min, float t_max, HitRecord& rec)const {
	Vec3 p_vec = Cross(r.GetDirection(), vec_p0p2_);
	float det = Dot(vec_p0p1_, p_vec);
	if (fabsf(det) < FLOAT_EPSILON)
		return false;
	float inv_det = 1.0f / det;
	Vec3 t_vec = r.GetOrigin() - v0_.pos();
	float u = Dot(t_vec, p_vec) * inv_det;
	if (u < 0 || u > 1) return false;

	Vec3 q_vec = Cross(t_vec, vec_p0p1_); 
	float v = Dot(r.GetDirection(), q_vec) * inv_det;
	if (v < 0 || v + u > 1) return false;

	Vec3 n = Normalize(v0_.pos() * (1 - u - v)+ v1_.pos() * u+ v2_.pos()*v);
	Vec3 vec_origin_v0 = v0_.pos() - r.GetOrigin();
	float d = Dot(n, vec_origin_v0);
	float t = d / Dot(n, r.GetDirection());
	if (t > t_max || t < t_min)
		return false;

	std::vector<std::tuple<Vertex, float>> vertex_weight_arry(3);
	vertex_weight_arry[0] = std::make_tuple(v0_, 1 - u - v);
	vertex_weight_arry[1] = std::make_tuple(v1_, u);
	vertex_weight_arry[2] = std::make_tuple(v2_, v);
	auto interp_ret = InterpolateVertices(vertex_weight_arry);
	rec.pos = r.GetPointAtParameter(t);
	rec.normal = Normalize(interp_ret.normal());
	rec.t = t;
	rec.u = interp_ret.u();
	rec.v = interp_ret.v();
	rec.mat_ptr = mat_ptr_;
	return true;
}

bool Triangle::GetBoundingBox(float t0, float t1, Aabb& box) const {
	box = bbx_;
	return true;
}
