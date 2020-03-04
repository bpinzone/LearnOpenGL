#ifndef SHADER_GLOBALS_H
#define SHADER_GLOBALS_H

#include <glm.hpp>

// Singleton for global variables provided to shader

class Shader_globals {
public:

    using mat4 = glm::mat4;

    static Shader_globals& get_instance();

    void set_model(const mat4& model_in);
    void set_view(const mat4& view_in);
    void set_projection(const mat4& projection_in);
    void set_normal(const mat4& normal_in);

    const mat4& get_model() const;
    const mat4& get_view() const;
    const mat4& get_projection() const;
    const mat4& get_normal() const;

private:
    Shader_globals();

    mat4 model;
    mat4 view;
    mat4 projection;

    mat4 normal;

};

#endif
