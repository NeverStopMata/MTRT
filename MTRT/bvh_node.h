#pragma once
#include "hitable.h"
#include <algorithm>
#include <vector>
class BVHNode : public Hitable {
public:
	BVHNode() {}
	BVHNode(std::vector<std::shared_ptr<Hitable>>::iterator hitable_ary_begin, std::vector<std::shared_ptr<Hitable>>::iterator hitable_ary_end, float time0, float time1);
	virtual bool Hit(const Ray& r, float tmin, float tmax, HitRecord& rec) const;
	virtual bool GetBoundingBox(float t0, float t1, Aabb& box) const;
	std::shared_ptr<Hitable> left_;
	std::shared_ptr<Hitable> right_;
	Aabb box_;
private:
	Vec3 GetRootMeanSquareInNodes(std::vector<std::shared_ptr<Hitable>>::iterator hitable_ary_begin, std::vector<std::shared_ptr<Hitable>>::iterator hitable_ary_end, float time0, float time1);

};
BVHNode::BVHNode(std::vector<std::shared_ptr<Hitable>>::iterator hitable_ary_begin, std::vector<std::shared_ptr<Hitable>>::iterator hitable_ary_end, float time0  = 0, float time1 = 0)
{
	Vec3 root_mean_squares = GetRootMeanSquareInNodes(hitable_ary_begin, hitable_ary_end, time0, time1);
	float max_root_mean_square = std::fmaxf(std::fmaxf(root_mean_squares.x(), root_mean_squares.y()), root_mean_squares.z());
	int n = (int)std::distance(hitable_ary_begin, hitable_ary_end);
	if (max_root_mean_square == root_mean_squares.x())
		std::sort(hitable_ary_begin, hitable_ary_end, CompareBoxX);
	else if (max_root_mean_square == root_mean_squares.y())
		std::sort(hitable_ary_begin, hitable_ary_end, CompareBoxY);
	else
		std::sort(hitable_ary_begin, hitable_ary_end, CompareBoxZ);
	if (n == 1)
		left_ = right_ = *hitable_ary_begin;
	else if (n == 2) {
		left_ = *hitable_ary_begin;
		right_ = *(hitable_ary_begin + 1);
	}
	else {
		left_ = std::make_shared<BVHNode>(hitable_ary_begin, hitable_ary_begin + n / 2, time0, time1);//todo
		right_ = std::make_shared<BVHNode>(hitable_ary_begin + n / 2, hitable_ary_end, time0, time1);
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
		if (hit_left && hit_right) {
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


Vec3 BVHNode::GetRootMeanSquareInNodes(std::vector<std::shared_ptr<Hitable>>::iterator hitable_ary_begin, std::vector<std::shared_ptr<Hitable>>::iterator hitable_ary_end, float time0, float time1) {
	int n = (int)std::distance(hitable_ary_begin, hitable_ary_end);
	std::vector<Vec3> center_array(n);
	int cnt = 0;
	Vec3 average(0, 0, 0);
	Vec3 sum(0, 0, 0);
	Vec3 mean_square(0, 0, 0);
	for (auto itor = hitable_ary_begin; itor != hitable_ary_end; itor++) {
		Aabb temp_box;
		(*hitable_ary_begin)->GetBoundingBox(time0, time1, temp_box);
		auto temp_centor = (temp_box.max_ + temp_box.min_) * 0.5;
		center_array[cnt++] = temp_centor;
		sum += temp_centor;
	}
	average = sum / Vec3(n, n, n);

	for (int i = 0; i < n; i++) {
		auto temp_diff = average = center_array[i];
		mean_square += temp_diff * temp_diff;
	}
	return Vec3(sqrt(mean_square.x()), sqrt(mean_square.y()), sqrt(mean_square.z()));
}