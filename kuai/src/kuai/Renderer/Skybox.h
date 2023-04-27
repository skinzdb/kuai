#pragma once

#include "Model.h"

namespace kuai {
	/** \class Skybox
	*	\brief A 3D background that is rendered behind everything in the scene. It gives the impression of objects or details at the horizon.
	*/
	class Skybox : public Model
	{
	public:
		Skybox(std::shared_ptr<Cubemap> cubemap);
		~Skybox();

	private:
    	static Rc<Mesh> mesh;
	};
}

