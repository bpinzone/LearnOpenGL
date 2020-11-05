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

class Material;

class Model {
public:

    using Meshes_t = std::vector<std::shared_ptr<Mesh>>;

    Model(const Meshes_t& meshes_in);

    // Use this shader, but extract textures and create material from model_path
    Model(std::shared_ptr<Shader> shader, const std::string& model_path);

    // Use this material. Only use mesh data from model_path
    Model(std::shared_ptr<Material> mat, const std::string& model_path);

    // TODO: rename to get_first_material.
    std::shared_ptr<Material> get_material() const {
        if(meshes.empty()){
            return nullptr;
        }
        return meshes.front()->get_material();
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

    // TODO: should be able to draw using a different material. Like the geometry pass.
    // Calls draw on every mesh in this model.
    void draw(std::shared_ptr<Material> mat_override = nullptr);
    void draw_instanced(int num_instances, std::shared_ptr<Material> mat_override = nullptr);

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