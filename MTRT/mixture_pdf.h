#pragma once
#include "pdf.h"
class MixturePDF :public PDF {
public:
	MixturePDF(PDF * p0, PDF* p1) { pdfs_[0] = p0; pdfs_[1] = p1; }
	virtual float GetValue(const Vec3& dir) const {
		return 0.5 * pdfs_[0]->GetValue(dir) + 0.5 * pdfs_[1]->GetValue(dir);//strange
	}
	virtual Vec3 Generate() const {
		if (GetRandom01() < 0.5)
			return pdfs_[0]->Generate();
		else
			return pdfs_[1]->Generate();
	}
	PDF* pdfs_[2];
};