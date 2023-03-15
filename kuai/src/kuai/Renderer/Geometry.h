#pragma once

#include "Model.h"

namespace kuai {
	class Geometry
	{
	public:
		static std::shared_ptr<Model> plane() { return std::make_shared<Model>("plane.obj"); }

        static std::shared_ptr<Model> cube() { return std::make_shared<Model>("cube.obj"); }

        static std::shared_ptr<Model> sphere() { return std::make_shared<Model>("sphere.obj"); }
	};
}