#ifndef SHARED_SHADERS_H
#define SHARED_SHADERS_H

#include <memory>

class Shader;
class Dir_light_depth_material;

// TODO: Rename because we now have a material in here.
class Shared_shaders {

public:

    static Shared_shaders& get_instance();

    // Disable copy and move.
    Shared_shaders& operator=(const Shared_shaders& other) = delete;
    Shared_shaders& operator=(Shared_shaders&& other) = delete;
    Shared_shaders(const Shared_shaders& other) = delete;
    Shared_shaders(Shared_shaders&& other) = delete;

    std::shared_ptr<Shader> geo_pass_shader;
    std::shared_ptr<Dir_light_depth_material> dir_light_depth_material;

private:

    Shared_shaders();
    ~Shared_shaders() = default;
};


#endif