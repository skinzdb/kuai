#pragma once

#include "glm/glm.hpp"

namespace kuai {

	struct Relationship
	{
		EntityId first = null_entity;
		EntityId prev = null_entity;
		EntityId next = null_entity;
		EntityId parent = null_entity;
	};

	struct Transform
	{
		glm::vec3 pos;
		glm::vec3 rot;
		glm::vec3 scale;
	};


}
