#include "shader.h"

#include "shader_globals.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <sstream>
#include <fstream>
#include <stdexcept>
#include <filesystem>
#include <memory>

using std::string;
using std::ostringstream;
using std::ifstream;

using std::make_shared;
using std::shared_ptr;

using std::ios;
using std::runtime_error;

static constexpr int c_info_log_size = 512;

static string read_file(const string& path);
static void compile_shader(unsigned int shader_id, const string& path);
static string convert_to_instance_path(const string& path);

Shader::Shader(
        const std::string& vertex_path, const std::string& fragment_path,
        const std::string& geom_path){

    const bool geom = !geom_path.empty();

    // Compile
    unsigned int vertex_id = glCreateShader(GL_VERTEX_SHADER);
    unsigned int fragment_id = glCreateShader(GL_FRAGMENT_SHADER);
    unsigned int geom_id;
    compile_shader(vertex_id, vertex_path);
    compile_shader(fragment_id, fragment_path);

    if(geom){
        geom_id = glCreateShader(GL_GEOMETRY_SHADER);
        compile_shader(geom_id, geom_path);
    }

    // Link
    program_id = glCreateProgram();
    glAttachShader(program_id, vertex_id);
    glAttachShader(program_id, fragment_id);
    if(geom){
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

    // Check if there is an instanced version.
    const string instance_vertex_path = convert_to_instance_path(vertex_path);
    const string instance_fragment_path = convert_to_instance_path(fragment_path);
    const string instance_geom_path = geom ? convert_to_instance_path(geom_path) : "";
    bool has_instanced_variant =
        std::filesystem::exists(instance_vertex_path)
        && std::filesystem::exists(instance_fragment_path)
        && (!geom || std::filesystem::exists(instance_geom_path));
    if(has_instanced_variant){
        instance_variant = make_shared<Shader>(
            instance_vertex_path, instance_fragment_path, instance_geom_path);
    }

    set_constant_uniforms();

    vert_path = vertex_path;

}

void Shader::use() const {
    glUseProgram(program_id);
}

void Shader::use_instance_variant() const {
    if(!instance_variant){
        throw std::runtime_error{"There is no instanced version of this shader!"};
    }
    instance_variant->use();
}

shared_ptr<Shader> Shader::get_instance_variant(){
    return instance_variant;
}

// Put shader globals into this shader's uniforms.
void Shader::load_uniforms_with_shader_globals() const {

    set_mat4fv("model", Shader_globals::get_instance().get_model());
    set_mat3fv("normal", Shader_globals::get_instance().get_normal());

    if(instance_variant){
        instance_variant->load_uniforms_with_shader_globals();
    }
}

void Shader::set_bool(const string& name, bool value) const {

    // NOTE Example on why you call use first:
    // glUniform* gets the uniform on the currently active shader program.
    // You need to make it the active program first.
    // The program_id passed is only used for the uni location.
    use();
    glUniform1i(glGetUniformLocation(program_id, name.c_str()), static_cast<int>(value));
    if(instance_variant){
        instance_variant->set_bool(name, value);
    }
}
void Shader::set_int(const string& name, int value) const {

    use();
    glUniform1i(glGetUniformLocation(program_id, name.c_str()), value);
    if(instance_variant){
        instance_variant->set_int(name, value);
    }
}
void Shader::set_float(const string& name, float value) const {

    use();
    glUniform1f(glGetUniformLocation(program_id, name.c_str()), value);
    if(instance_variant){
        instance_variant->set_float(name, value);
    }
}

void Shader::set_vec3(const std::string& name, const glm::vec3& value) const{

    use();
    glUniform3fv(
        glGetUniformLocation(program_id, name.c_str()),
        1,
        // todo better way?
        &value[0]
    );
    if(instance_variant){
        instance_variant->set_vec3(name, value);
    }
}

void Shader::set_mat4fv(const string& name, const glm::mat4& value) const {

    // NOTE: I don't like this compared to this pattern for uniform blocks
    /*
        unsigned int lights_index = glGetUniformBlockIndex(shaderA.ID, "Lights");
        glUniformBlockBinding(shaderA.ID, lights_index, 2);
    */
    use();
    glUniformMatrix4fv(glGetUniformLocation(program_id, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
    if(instance_variant){
        instance_variant->set_mat4fv(name, value);
    }
}

void Shader::set_mat3fv(const string& name, const glm::mat3& value) const {

    use();
    glUniformMatrix3fv(glGetUniformLocation(program_id, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
    if(instance_variant){
        instance_variant->set_mat3fv(name, value);
    }
}

void Shader::set_constant_uniforms(){

    // For now, the only constant uniform is a uniform block binding for less-freq-changing matrices.

    // matrices_uniform_block_idx is NOT the same as: static const int c_matrices_uniform_block_binding_point;
    // c_matrices... is a binding point, which the shader instance and shader_globals both need to know.
    // matrices_uni_idx is an idx wrt this specific shader. (similar feel to GlGetUniformLocation)

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

    if(instance_variant){
        instance_variant->set_constant_uniforms();
    }
}

// Returns a string representing the entire file.
string read_file(const string& path) {

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
void compile_shader(unsigned int shader_id, const string& path){

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

static string convert_to_instance_path(const string& path){

    size_t dot_idx = path.find_last_of('.');
    const size_t name_length = dot_idx;

    string name = path.substr(0, name_length);
    string extension = path.substr(dot_idx);

    return name + "_instance" + extension;
}