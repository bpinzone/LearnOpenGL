#ifndef SHADER_GLOBALS_H
#define SHADER_GLOBALS_H

#include <glm/glm.hpp>

// Singleton for global variables provided to shader

class Shader_globals {
public:

    using mat4 = glm::mat4;

    // similar idea to a texture unit number.
    // matrices point of maintenance.
    const int c_matrices_uniform_block_binding_point = 0;

    static Shader_globals& get_instance(){
        static Shader_globals sg;
        return sg;
    }

    void set_model(const mat4& model_in);
    void set_view(const mat4& view_in);
    void set_projection(const mat4& projection_in);
    void set_normal(const mat4& normal_in);

    const mat4& get_model() const;
    const mat4& get_view() const;
    const mat4& get_projection() const;
    const mat4& get_normal() const;

	Shader_globals(const Shader_globals&) = delete;
	Shader_globals(Shader_globals&&) = delete;
	Shader_globals& operator= (const Shader_globals&)  = delete;
	Shader_globals& operator= (Shader_globals&&) = delete;

private:

    Shader_globals();

    mat4 model;
    mat4 normal;

    // matrices point of maintenance.
    mat4 view;
    mat4 projection;

    // view and projection matrix will be pushed into here.
    // NOT model or normal.
    // TODO: better name?
    // matrices point of maintenance.
    unsigned int matrices_ubo;
};

#endif
