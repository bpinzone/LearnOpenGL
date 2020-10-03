#ifndef SHARED_SHADERS_H
#define SHARED_SHADERS_H

#include <memory>

class Shader;

class Shared_shaders {

public:

    static Shared_shaders& get_instance();

    // Disable copy and move.
    Shared_shaders& operator=(const Shared_shaders& other) = delete;
    Shared_shaders& operator=(Shared_shaders&& other) = delete;
    Shared_shaders(const Shared_shaders& other) = delete;
    Shared_shaders(Shared_shaders&& other) = delete;

    std::shared_ptr<Shader> geo_pass_shader;
    std::shared_ptr<Shader> instance_geo_pass_shader;

private:

    Shared_shaders();
    ~Shared_shaders() = default;
};


#endif