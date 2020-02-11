#version 330 core
layout (location = 0) in vec3 aPos;
// layout (location = 1) in vec3 aColor;
layout (location = 1) in vec2 aTexCoord;

out vec3 ourColor;
out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// must assign to gl_Position the clip space coord. It is clip space right?
/*
Once all the vertices are transformed to clip space a final operation called perspective division is performed where we divide the x, y and z components of the position vectors by the vector's homogeneous w component; perspective division is what transforms the 4D clip space coordinates to 3D normalized device coordinates. This step is performed automatically at the end of the vertex shader step.
*/
void main() {

    gl_Position = projection * view * model * vec4(aPos, 1.0);

    // ourColor = aColor;
    TexCoord = aTexCoord;
}