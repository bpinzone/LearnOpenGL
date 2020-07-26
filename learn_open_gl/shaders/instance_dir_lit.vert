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
    // Recall: As computed below, this is the normal in world-space.
    vec3 Normal;
    vec3 WorldPos;
    vec2 TexCoords;
} vs_out;

void main() {
    // Perspective division happens BETWEEEN the geometry and fragment shader.
    gl_Position = projection * view * instance_model * vec4(aPos, 1.0);
    vs_out.Normal = instance_normal * aNormal;
    vs_out.WorldPos = vec3(instance_model * vec4(aPos, 1.0));
    vs_out.TexCoords = aTexCoord;
}