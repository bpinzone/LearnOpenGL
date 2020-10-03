#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>
// #include <glfw3.h>

#include <vector>
#include <utility>
#include <string>
#include <set>
#include <memory>

struct Texture_comp;

class Texture {
public:

    // ex: sampler_base_identifier = "material.diffuse", "material.specular"
    // Should be the same as the sampler2D identifier used in shader.
    // TODO: Poor design. Switching variable
    static std::shared_ptr<Texture> construct_texture_from_img_file(
        const std::string& _path,
        const std::string& _sampler_base_identifier,
        bool flip_vertically);

    // ex: identifier = "material.cube"
    // Paths must be in this open gl defined ordered:
    // POSITIVE_X Right, NEGATIVE_X Left, POSITIVE_Y Top, NEGATIVE_Y Bottom, POSITIVE_Z Back, NEGATIVE_Z Front
    static std::shared_ptr<Texture> construct_cube_texture_from_img_file(
        const std::vector<std::string>& _paths,
        const std::string& _sampler_base_identifier);

    // ex: identifier = "position", "normal", "albedo_spec"
    static std::shared_ptr<Texture> construct_texture_from_completed_id(
        unsigned int completed_texture_id,
        const std::string& _sampler_base_identifier);

    const std::string& get_sampler_base_identifier() const;
    unsigned int get_id() const;

    const std::vector<std::string>& get_paths() const;

    friend class std::shared_ptr<Texture>;

private:

    unsigned int texture_id;

    std::string sampler_base_identifier;

    // Relative to dir main is in. Includes the file name.
    // Size should be 1 for regular images, 6 for cube maps, 0 for pre-completed textures.
    std::vector<std::string> paths;

    Texture(
        unsigned int _texture_id,
        const std::string& _sampler_base_identifier,
        const std::vector<std::string>& _paths);

    // TODO: dtor. And with lots of other objects that are referred to by shared_ptr...

};


#endif