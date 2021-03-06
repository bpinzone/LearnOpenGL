#ifndef SHADER_H
#define SHADER_H

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>

class Shader {
public:

    // Paths relative to main's dir.
    Shader(
        const char* vertex_path, const char* fragment_path,
        const char* geom_path = nullptr);

    void use() const;
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
};

#endif
