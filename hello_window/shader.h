#ifndef SHADER_H
#define SHADER_H

#include <glm.hpp>
#include <type_ptr.hpp>

#include <string>

class Shader {
public:

    Shader(const char* vertex_path, const char* fragment_path);

    void use(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection) const;

    void set_bool(const std::string& name, bool value) const;
    void set_int(const std::string& name, int value) const;
    void set_float(const std::string& name, float value) const;
    void set_mat4fv(const std::string& name, const glm::mat4& value) const;

    unsigned int program_id;
};

#endif
