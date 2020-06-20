#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

// unused. potential problem: need to put these in?
uniform mat4 model;
uniform mat3 normal;
// uniform block
// matrices point of maintenance.
layout (std140) uniform matrices {
    mat4 view;
    mat4 projection;
};

out vec2 TexCoords;

void main(){

    gl_Position = projection * view * model * vec4(aPos, 1.0);

    // Quad UV does not have 0, 0 in bottom left. Flip y axis.
    TexCoords = vec2(aTexCoord.x, 1 - aTexCoord.y);
}