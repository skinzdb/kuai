#pragma once

#include "Mesh.h"

namespace kuai {
	class Skybox : public Mesh
	{
	public:
		Skybox(std::shared_ptr<Cubemap> cubemap);
		~Skybox();

		virtual void render() override;
		
	private:

	};
}

