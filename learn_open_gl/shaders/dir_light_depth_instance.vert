#version 330 core
layout (location = 0) in vec3 aPos;

layout (location = 3) in mat4 instance_model; // locations 3, 4, 5, 6
layout (location = 7) in mat3 instance_normal; // locations 7, 8, 9.
// Other vertex inputs will be populated here, but its fine that we don't use them.

// Its fine that we don't have the std140 uniform block.

uniform mat4 world_space_to_light_clip;

void main(){
    gl_Position = world_space_to_light_clip * instance_model * vec4(aPos, 1.0);
}