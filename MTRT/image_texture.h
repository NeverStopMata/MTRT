#pragma once
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "texture.h"
class ImageTexture : public Texture {
public:
	ImageTexture() {}
	ImageTexture(unsigned char *pixels, int nx, int ny) :data_(pixels),nx_(nx),ny_(ny){}
	virtual Vec3 Sample(float u, float v, const Vec3& p) const;
	unsigned char *data_;
	int nx_, ny_;
};

Vec3 ImageTexture::Sample(float u, float v, const Vec3& p) const {
	int i = u * nx_;
	int j = (1 - v) * ny_ - 0.001;
	if (i < 0) i = 0;
	if (j < 0) j = 0;
	if (i > nx_ - 1) i = nx_ - 1;
	if (j > ny_ - 1) j = ny_ - 1;
	float r = int(data_[3 * i + 3 * nx_ * j]) / 255.0f;
	float g = int(data_[3 * i + 3 * nx_ * j + 1]) / 255.0f;
	float b = int(data_[3 * i + 3 * nx_ * j + 2]) / 255.0f;
	return Vec3(r, g, b);
}