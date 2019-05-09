#pragma once
#define TINYOBJLOADER_IMPLEMENTATION
#include <string>
#include <unordered_map>
#include "triangle.h"
#include "dielectric.h"
#include "tiny_obj_loader.h"
#include "bvh_node.h"
class TriangleMesh :public Hitable{
public:
	TriangleMesh() {}
	~TriangleMesh() {
		delete root_ptr_;
	}
	TriangleMesh(const std::string& obj_file_path,std::shared_ptr<Material> mat_ptr) {
		std::vector<Vertex> tmp_vertices;
		std::vector<uint32_t> tmp_indices;
		std::vector<std::shared_ptr<Hitable>> triangle_arry;
		LoadModel(obj_file_path, triangle_arry, mat_ptr);
		for (int i = 0; i < tmp_indices.size(); i += 3) {
			auto triangle_ptr = std::make_shared<Triangle>(tmp_vertices[tmp_indices[i]], tmp_vertices[tmp_indices[i + 1]], tmp_vertices[tmp_indices[i + 2]], mat_ptr);
			triangle_arry.push_back(triangle_ptr);
		}
		root_ptr_ = new BVHNode(triangle_arry.begin(), triangle_arry.end());
	}
	virtual bool Hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const {
		return root_ptr_->Hit(r, t_min, t_max, rec);
	}
	virtual bool GetBoundingBox(float t0, float t1, Aabb& box) const {
		return root_ptr_->GetBoundingBox(t0, t1, box);
	}
	void LoadModel(const std::string& obj_file_path,std::vector<std::shared_ptr<Hitable>>& hitable_array, std::shared_ptr<Material> mat_ptr);
	BVHNode * root_ptr_;
};


void TriangleMesh::LoadModel(const std::string& obj_file_path,std::vector<std::shared_ptr<Hitable>>& hitable_array, std::shared_ptr<Material> mat_ptr) {
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string warn, err;

	if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, obj_file_path.c_str())) {
		throw std::runtime_error(warn + err);
	}
	int idx_cnt = 0;

	for (const auto& shape : shapes) {
		std::cout << "Vertex Num: " << shape.mesh.indices.size() << std::endl;
		for (int i = 0; i < shape.mesh.indices.size();i+= 3) {
			Vertex vertex[3];
			bool has_normal = false;
			for (int j = 0; j < 3; j++) {
				vertex[j].pos_ = {
				attrib.vertices[3 * shape.mesh.indices[i + j].vertex_index + 0],
				attrib.vertices[3 * shape.mesh.indices[i + j].vertex_index + 1],
				attrib.vertices[3 * shape.mesh.indices[i + j].vertex_index + 2]
				};
				if (attrib.normals.size() != 0) {
					has_normal = true;
					vertex[j].normal_ = {
					attrib.normals[3 * shape.mesh.indices[i + j].normal_index + 0],
					attrib.normals[3 * shape.mesh.indices[i + j].normal_index + 1],
					attrib.normals[3 * shape.mesh.indices[i + j].normal_index + 2]
					};
				}

				vertex[j].u_ = attrib.texcoords[2 * shape.mesh.indices[i].texcoord_index + 0];
				vertex[j].v_ = 1 - attrib.texcoords[2 * shape.mesh.indices[i].texcoord_index + 1];//todo  whether need "1-" is not sure.
			}
			
			//vertex[0].pos_ *= 0.1;
			//vertex[1].pos_ *= 0.1;
			//vertex[2].pos_ *= 0.1;
			if (!has_normal) {
				Vec3 face_normal = Normalize(Cross(vertex[1].pos() - vertex[0].pos(), vertex[2].pos() - vertex[1].pos()));
				vertex[0].normal_ = face_normal;
				vertex[1].normal_ = face_normal;
				vertex[2].normal_ = face_normal;
			}
			hitable_array.push_back(std::make_shared<Triangle>(vertex[0], vertex[1], vertex[2], mat_ptr));
		}
	}
	std::cout << "Triangle Num: " << hitable_array.size() << std::endl;
}