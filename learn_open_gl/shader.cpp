#include "shader.h"

#include "shader_globals.h"

#include <glad/glad.h>
#include <glm.hpp>
#include <type_ptr.hpp>

#include <iostream>
#include <sstream>
#include <fstream>
#include <stdexcept>

using std::string;
using std::ostringstream;
using std::ifstream;

using std::ios;
using std::runtime_error;

static constexpr int c_info_log_size = 512;

static string read_file(const char* const path);
static void compile_shader(unsigned int shader_id, const char* path);

Shader::Shader(
        const char* vertex_path, const char* fragment_path,
        const char* geom_path){

    // Compile
    unsigned int vertex_id = glCreateShader(GL_VERTEX_SHADER);
    unsigned int fragment_id = glCreateShader(GL_FRAGMENT_SHADER);
    unsigned int geom_id;
    compile_shader(vertex_id, vertex_path);
    compile_shader(fragment_id, fragment_path);

    if(geom_path){
        geom_id = glCreateShader(GL_GEOMETRY_SHADER);
        compile_shader(geom_id, geom_path);
    }

    // Link
    program_id = glCreateProgram();
    glAttachShader(program_id, vertex_id);
    glAttachShader(program_id, fragment_id);
    if(geom_path){
        glAttachShader(program_id, geom_id);
    }
    glLinkProgram(program_id);

    // Check Link
    int success;
    glGetProgramiv(program_id, GL_LINK_STATUS, &success);
    if(!success){
        char info_log[c_info_log_size];
        glGetProgramInfoLog(program_id, c_info_log_size, nullptr, info_log);
        ostringstream oss;
        oss << "Error Linking Shader Program:\n" << info_log;
        throw runtime_error{oss.str()};
    }

    glDeleteShader(vertex_id);
    glDeleteShader(fragment_id);

    set_constant_uniforms();

}

void Shader::use() const {
    glUseProgram(program_id);
}

// Put shader globals into this shader's uniforms.
void Shader::forward_shader_globals_to_uniforms() const {

    set_mat4fv("model", Shader_globals::get_instance().get_model());
    set_mat3fv("normal", Shader_globals::get_instance().get_normal());
}

void Shader::set_bool(const string& name, bool value) const {

    // NOTE Example on why you call use first:
    // glUniform<> gets the uniform on the currently active shader program.
    // You need to make it the active program first.
    // The program_id passed is only used for the uni location.
    use();
    glUniform1i(glGetUniformLocation(program_id, name.c_str()), static_cast<int>(value));
}
void Shader::set_int(const string& name, int value) const {

    use();
    glUniform1i(glGetUniformLocation(program_id, name.c_str()), value);
}
void Shader::set_float(const string& name, float value) const {

    use();
    glUniform1f(glGetUniformLocation(program_id, name.c_str()), value);
}

void Shader::set_vec3(const std::string& name, const glm::vec3& value) const{

    use();
    glUniform3fv(
        glGetUniformLocation(program_id, name.c_str()),
        1,
        // todo better way?
        &value[0]
    );
}

void Shader::set_mat4fv(const string& name, const glm::mat4& value) const {

    // NOTE: I don't like this compared to this pattern for uniform blocks
    /*
        unsigned int lights_index = glGetUniformBlockIndex(shaderA.ID, "Lights");
        glUniformBlockBinding(shaderA.ID, lights_index, 2);
    */
    use();
    glUniformMatrix4fv(glGetUniformLocation(program_id, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::set_mat3fv(const string& name, const glm::mat3& value) const {

    use();
    glUniformMatrix3fv(glGetUniformLocation(program_id, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::set_constant_uniforms(){

    // For now, the only constant uniform is a uniform block binding for less-freq-changing matrices.

    // matrices_uniform_block_idx is NOT the same as: static const int c_matrices_uniform_block_binding_point;
    // c_matrices... is a binding point, which the shader instance and shader_globals both need to know.
    // matrices_uni_idx is an idx wrt this specific shader.

    // String must match declaration in: layout (std140) uniform matrices {
    // matrices point of maintenance.
    unsigned int matrices_uniform_block_idx = glGetUniformBlockIndex(program_id, "matrices");
    if(matrices_uniform_block_idx != GL_INVALID_INDEX){
        glUniformBlockBinding(
            program_id, matrices_uniform_block_idx,
            Shader_globals::get_instance().c_matrices_uniform_block_binding_point);
        // set_mat4fv("view", Shader_globals::get_instance().get_view());
        // set_mat4fv("projection", Shader_globals::get_instance().get_projection());
    }

}

// Returns a string representing the entire file.
string read_file(const char* const path) {

    ifstream fin(path, ios::in | ios::binary);
    if(!fin){
        throw runtime_error("File could not be opened");
    }
    string contents;
    fin.seekg(0, ios::end);
    contents.resize(fin.tellg());
    fin.seekg(0, ios::beg);
    fin.read(&contents[0], contents.size());
    fin.close();
    return contents;
}

// Compiles shader from source and checks for errors.
// Requires: Shader is already created with shader_id. (Not the program_id)
void compile_shader(unsigned int shader_id, const char* path){

    const string code = read_file(path);
    const char* code_c_str = code.c_str();
    glShaderSource(shader_id, 1, &code_c_str, nullptr);
    glCompileShader(shader_id);

    int success;
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
    if(!success){
        char info_log[c_info_log_size];
        glGetShaderInfoLog(shader_id, c_info_log_size, nullptr, info_log);
        ostringstream oss;
        oss << "ERROR Compiling Shader: " << path << "\n" << info_log;
        throw runtime_error{oss.str()};
    }
}
