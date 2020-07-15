#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 model;

// Converts model space normals to world space normals.
// (not what we want in this case, want them in view space.)
//     Probably dont' really need them in view space, point is we are modifying a clip space coord on geom shader.
// unused in this shader.
uniform mat3 normal;

// NOTE: gl_Position will be output as the VIEW SPACE position.

// uniform block
// matrices point of maintenance.
layout (std140) uniform matrices {
    mat4 view;
    mat4 projection;
};

// interface block
out VS_OUT {
    vec3 ViewSpaceNormal;
} vs_out;

void main() {
    // Perspective division happens BETWEEEN the geometry and fragment shader.
    gl_Position = view * model * vec4(aPos, 1.0);

    // "normal" takes a local space normal into world space.
    // Whereas this view space normal takes a local space normal into view space.
    mat3 view_space_normal_mat = mat3(transpose(inverse(view  * model)));
    vs_out.ViewSpaceNormal = view_space_normal_mat * aNormal;
}