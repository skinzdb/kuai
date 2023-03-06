#include "kpch.h"
#include "ModelLoader.h"

#include <tiny_obj_loader.h>

namespace kuai {
    std::shared_ptr<Mesh> ModelLoader::loadObj(const std::string& filename)
    {
        tinyobj::ObjReaderConfig reader_config;
        reader_config.mtl_search_path = "./"; // Path to material files

        tinyobj::ObjReader reader;

        if (!reader.ParseFromFile(filename, reader_config)) {
            if (!reader.Error().empty()) {
                KU_CORE_ERROR("Error loading OBJ file: {0}", reader.Error());
            }
            exit(1);
        }

        if (!reader.Warning().empty()) {
            KU_CORE_WARN("Warnings loading OBJ file: {0}", reader.Warning());
        }

        auto& attrib = reader.GetAttrib();
        auto& shapes = reader.GetShapes();
        auto& materials = reader.GetMaterials();

        std::vector<uint32_t> indices;

        for (size_t s = 0; s < shapes.size(); s++) // Build indices vector
        {
            for (size_t i = 0; i < shapes[s].mesh.indices.size(); i++)
            {
                indices.push_back(shapes[s].mesh.indices[i].vertex_index);
            }
        }

        KU_CORE_INFO("Loaded OBJ file: {0}", filename);

        // REMOVE (one day) ---------------------
        std::vector texCoords = attrib.normals;
        if (texCoords.empty())
            texCoords = { 0, 0 };
        // --------------------------------------

        return std::make_shared<Mesh>(attrib.vertices, attrib.normals, texCoords, indices);
    }
}
