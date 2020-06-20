#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 model;
uniform mat3 normal;

// uniform block
// matrices point of maintenance.
layout (std140) uniform matrices {
    mat4 view;
    mat4 projection;
};

// interface block
out VS_OUT {
    vec3 Normal;
    vec3 WorldPos;
    vec2 TexCoords;
} vs_out;

void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    vs_out.Normal = normal * aNormal;
    vs_out.WorldPos = vec3(model * vec4(aPos, 1.0));
    vs_out.TexCoords = aTexCoord;
}