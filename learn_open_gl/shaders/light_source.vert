#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;

uniform mat4 model;
uniform mat3 normal;

// uniform block
// matrices point of maintenance.
layout (std140) uniform matrices {
    mat4 view;
    mat4 projection;
};

void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}