#include "kpch.h"

#include "Geometry.h"

namespace kuai {
    static std::vector<Vertex> planeVertices =
    {
        { -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f },
        {  1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f },
        { -1.0f, 0.0f,-1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f },
        {  1.0f, 0.0f,-1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f }
    };

    static std::vector<u32> planeIndices = { 0, 1, 2, 2, 0, 1 };

    Rc<Model> Geometry::plane = nullptr; // MakeRc<Model>(MakeRc<Mesh>(planeVertices, planeIndices));

    Rc<Model> Geometry::cube = nullptr;

    Rc<Model> Geometry::sphere = nullptr;
}