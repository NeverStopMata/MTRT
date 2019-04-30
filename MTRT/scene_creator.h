#pragma once
#include <vector>
#include "hitable.h"
#include "metal.h"
#include "lambertian.h"
#include "dielectric.h"
#include "sphere.h"
#include "xy_rect.h"
#include "triangle_mesh.h"
#include "const_texture.h"
#include "checker_texture.h"
#include "image_texture.h"
#include "noise_texture.h"
#include "diffuse_light.h"
class SceneCreator {
public:
	static std::vector<Hitable*> CreateRandomScene(int num_element) {
		std::vector<Hitable*> ret;
		Texture* checker = new CheckerTexture(new ConstTexture(Vec3(0.2, 0.3, 0.1)), new ConstTexture(Vec3(0.9, 0.9, 0.9)));
		ret.push_back(new Sphere(Vec3(0, -1000, 0), 1000, new Lambertian(checker)));
		auto temp_sqrt_size = int(sqrt(float(num_element)));
		for (int i = -temp_sqrt_size / 2; i < temp_sqrt_size / 2; i++) {
			for (int j = -temp_sqrt_size / 2; j < temp_sqrt_size / 2; j++) {
				float choose_mat = GetRandom01();
				Vec3 center(1.2f*i + 0.7f * (GetRandom01()), 0.2f, 1.2f*j + 0.7f * (GetRandom01()));
				if ((center - Vec3(4, 0.2f, 0)).GetLength() > 0.9f) {
					if (choose_mat < 0.7) {
						ret.push_back(new Sphere(center, 0.2,
							new Lambertian(new ConstTexture(Vec3(GetRandom01()*GetRandom01(), GetRandom01()*GetRandom01(), GetRandom01()*GetRandom01())))));
					}
					else if (choose_mat < 0.8) {
						ret.push_back(new Sphere(center, 0.2,
							new Metal(Vec3(0.5*(1+GetRandom01()), 0.5*(1 + GetRandom01()), 0.5*(1 + GetRandom01())),0.3*GetRandom01())));
					}
					else {
						ret.push_back(new Sphere(center, 0.2, new Dielectric(1.5)));
					}
				}
			}
		}
		ret.push_back(new Sphere(Vec3(0, 1, 0), 1, new Dielectric(1.5)));
		ret.push_back(new Sphere(Vec3(-4, 1, 0), 1, new Lambertian(new ConstTexture(Vec3(0.4,0.2,0.1)))));
		ret.push_back(new Sphere(Vec3(4, 1, 0), 1, new Metal(Vec3(0.7,0.6,0.5), 0.0)));
		return ret;
	}
	

	static std::vector<Hitable*> CreateLightScene() {
		Texture* noise_tex = new NoiseTexture(4);
		std::vector<Hitable*> ret;
		ret.push_back(new Sphere(Vec3(0, -1000, 0), 1000, new Lambertian(noise_tex)));
		ret.push_back(new Sphere(Vec3(0,     2, 0), 2   , new Lambertian(noise_tex)));
		ret.push_back(new XYRect(3, 5, 1, 3, -2, new DiffuseLight(new ConstTexture(Vec3(1, 1, 1)), 4)));
		return ret;
	}


	static std::vector<Hitable*> CreateMeshScene() {
		std::vector<Hitable*> ret;
		Texture* checker = new CheckerTexture(new ConstTexture(Vec3(0.2, 0.3, 0.1)), new ConstTexture(Vec3(0.9, 0.9, 0.9)));
		Texture* const_tex = new ConstTexture(Vec3(1, 1, 1));
		ret.push_back(new TriangleMesh(MODEL_PATH, new Dielectric(1.5)));
		//ret.push_back(new TriangleMesh(MODEL_PATH, new Metal(Vec3(0.72,0.72,0.72),0.0)));
		return ret;
	}



};