#ifndef MODEL_H
#define MODEL_H

#include "shader.h"
#include "mesh.h"
#include "texture.h"
#include "material.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>  // gen smooth normals

#include <string>
#include <vector>
#include <memory>

class Model {
public:

    using Meshes_t = std::vector<std::shared_ptr<Mesh>>;

    Model(const Meshes_t& meshes_in);

    Model(std::shared_ptr<Shader> shader, const std::string& model_path);

    std::shared_ptr<Material> get_material() const {
        if(meshes.empty()){
            return nullptr;
        }
        return meshes.front()->material;
    }

    void set_materials(std::shared_ptr<Material> material){
        for(auto& mesh : meshes){
            mesh->set_material(material);
        }
    }

    void reverse_all_mesh_winding_orders(){
        for(auto& mesh : meshes){
            mesh->reverse_winding_order();
        }
    }

    // Calls draw on every mesh in this model.
    void draw();
    void draw_instanced(int num_instances);

    // Requires: This model only has a single mesh. Binds that mesh's vao.
    // Throws: If this model doesn't have exactly one mesh.
    void bind_only_mesh_vao() const;

    // Throws: If this model doesn't have exactly one mesh.
    int get_only_mesh_next_avail_vert_attrib_idx() const;

private:

    void load_meshes(aiNode* node, const aiScene* scene, std::shared_ptr<Shader> shader);

    Meshes_t meshes;
    std::string directory;
};

#endif