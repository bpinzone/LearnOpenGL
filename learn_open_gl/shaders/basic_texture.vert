#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;

out vec2 TexCoord;

// potential problem: has no normal.
// potential problem: CANNOT currently use the uniform buffer thats in place.
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// must assign to gl_Position the clip space coord.
/*
Once all the vertices are transformed to clip space a final operation called perspective division is performed where we divide the x, y and z components of the position vectors by the vector's homogeneous w component; perspective division is what transforms the 4D clip space coordinates to 3D normalized device coordinates. This step is performed automatically at the end of the vertex shader step.

More specifically, pretty sure it happens between the geometry shader and fragment shader.
*/
void main() {

    TexCoord = aTexCoord;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}