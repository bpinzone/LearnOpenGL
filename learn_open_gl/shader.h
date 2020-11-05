#ifndef SHADER_H
#define SHADER_H

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <memory>

// Philosophy for instanced variants of shaders:
// Just construct your non-instanced version, then AUTOMATICALLY:
    // Instanced version will be constructed if it is available.
    // Any uniform updates to non-instance version will be forwared to instance version.
    // Instance versino wil be used when appropriate.
class Shader {
public:

    // Paths relative to main's dir.
    Shader(
        const std::string& vertex_path, const std::string& fragment_path,
        const std::string& geom_path = "");

    void use() const;
    // Throws if there is no instanced variant.
    void use_instance_variant() const;

    std::shared_ptr<Shader> get_instance_variant();

    void load_uniforms_with_shader_globals() const;

    void set_bool(const std::string& name, bool value) const;
    void set_int(const std::string& name, int value) const;
    void set_float(const std::string& name, float value) const;
    void set_vec3(const std::string& name, const glm::vec3& value) const;
    void set_mat4fv(const std::string& name, const glm::mat4& value) const;
    void set_mat3fv(const std::string& name, const glm::mat3& value) const;

    unsigned int program_id;

private:

    void set_constant_uniforms();

    std::shared_ptr<Shader> instance_variant;
    std::string vert_path;
};

#endif
