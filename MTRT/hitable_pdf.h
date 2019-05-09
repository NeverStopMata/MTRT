#pragma once
#include "pdf.h"
#include "hitable.h"
class HitablePDF : public PDF {
public:
	HitablePDF(std::shared_ptr<Hitable> hitable_ptr, const Vec3& origin):hitable_ptr_(hitable_ptr), origin_(origin) {}
	virtual float GetValue(const Vec3& dir) const {
		return hitable_ptr_->GetPDFValue(origin_, dir);
	}
	virtual Vec3 Generate() const {
		return hitable_ptr_->GetRandomDirToSelf(origin_);
	}
	Vec3 origin_;
	std::shared_ptr<Hitable> hitable_ptr_;
};