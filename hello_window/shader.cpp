#include "shader.h"

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

Shader::Shader(const char* vertex_path, const char* fragment_path){

    // Compile
    unsigned int vertex_id = glCreateShader(GL_VERTEX_SHADER);
    unsigned int fragment_id = glCreateShader(GL_FRAGMENT_SHADER);
    compile_shader(vertex_id, vertex_path);
    compile_shader(fragment_id, fragment_path);

    // Link
    program_id = glCreateProgram();
    glAttachShader(program_id, vertex_id);
    glAttachShader(program_id, fragment_id);
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
}

void Shader::use(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection) const {

    glUseProgram(program_id);
    set_mat4fv("model", model);
    set_mat4fv("view", view);
    set_mat4fv("projection", projection);
}

void Shader::set_bool(const string& name, bool value) const {
    glUniform1i(glGetUniformLocation(program_id, name.c_str()), static_cast<int>(value));
}
void Shader::set_int(const string& name, int value) const {
    glUniform1i(glGetUniformLocation(program_id, name.c_str()), value);
}
void Shader::set_float(const string& name, float value) const {
    glUniform1f(glGetUniformLocation(program_id, name.c_str()), value);
}
void Shader::set_mat4fv(const string& name, const glm::mat4& value) const {
    glUniformMatrix4fv(glGetUniformLocation(program_id, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
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
