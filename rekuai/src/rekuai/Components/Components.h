#pragma once

#include "glm/glm.hpp"

#include "rekuai/Renderer/Mesh.h"
#include "rekuai/Renderer/Material.h"

namespace kuai {

	//struct Relationship
	//{
	//	EntityId first = null_entity;
	//	EntityId prev = null_entity;
	//	EntityId next = null_entity;
	//	EntityId parent = null_entity;
	//};

	struct Transform
	{
		glm::vec3 pos;
		glm::vec3 rot;
		glm::vec3 scale;

		bool operator==(const Transform& other) const
		{
			return pos == other.pos && rot == other.rot && scale == other.scale;
		}
	};

	struct MeshRenderer
	{
		MeshRenderer(const Mesh& mesh, const Material& material) : mesh(mesh), material(material) {}
		
		Mesh mesh;
		Material material;
	};


}
