#pragma once

#include "Mesh.h"

namespace kuai {
	/** \class Skybox
	*	\brief A 3D background that is rendered behind everything in the scene. It gives the impression of objects or details at the horizon.
	*/
	class Skybox : public Mesh
	{
	public:
		Skybox(std::shared_ptr<Cubemap> cubemap);
		~Skybox();

		// @private
		virtual void render() override;
	
	};
}

