#include "smpch.h"
#include "ModelLoader.h"

#include <tiny_obj_loader.h>

namespace Smong {
    std::shared_ptr<Mesh> ModelLoader::LoadObj(std::string filename)
    {
        tinyobj::ObjReaderConfig reader_config;
        reader_config.mtl_search_path = "./"; // Path to material files

        tinyobj::ObjReader reader;

        if (!reader.ParseFromFile(filename, reader_config)) {
            if (!reader.Error().empty()) {
                SM_ERROR("Error loading OBJ file: {0}", reader.Error());
            }
            exit(1);
        }

        if (!reader.Warning().empty()) {
            SM_WARN("Warnings loading OBJ file: {0}", reader.Warning());
        }

        auto& attrib = reader.GetAttrib();
        auto& shapes = reader.GetShapes();
        auto& materials = reader.GetMaterials();

        std::vector<uint32_t> indices;

        for (size_t s = 0; s < shapes.size(); s++) 
        {
            for (size_t i = 0; i < shapes[s].mesh.indices.size(); i++)
            {
                indices.push_back(shapes[s].mesh.indices[i].vertex_index);
            }
        }

        return std::make_shared<Mesh>(attrib.vertices, attrib.normals, attrib.texcoords, indices);
    }
}
