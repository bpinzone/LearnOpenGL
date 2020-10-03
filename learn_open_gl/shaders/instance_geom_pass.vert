#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in mat4 instance_model; // locations 3, 4, 5, 6
layout (location = 7) in mat3 instance_normal; // locations 7, 8, 9.

// uniform block
// matrices point of maintenance.
layout (std140) uniform matrices {
    mat4 view;
    mat4 projection;
};

// interface block
out VS_OUT {

    // In world space
    vec3 WorldNormal;
    vec3 WorldPos;
    vec2 TexCoord;
} vs_out;

void main() {

    vec4 world_pos = instance_model * vec4(aPos, 1.0);

    gl_Position = projection * view * world_pos;

    vs_out.WorldNormal = instance_normal * aNormal;
    vs_out.WorldPos = world_pos.xyz;
    vs_out.TexCoord = aTexCoord;
}