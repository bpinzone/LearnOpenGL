#version 330 core
layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

in VS_OUT {
    vec3 ViewSpaceNormal;
} gs_in[]; // notice array

// uniform block
// matrices point of maintenance.
layout (std140) uniform matrices {
    mat4 view;
    mat4 projection;
};

const float c_magnitude = 0.4;

void generate_line(int vertex_x){

    vec4 base_view_pos = gl_in[vertex_x].gl_Position;

    gl_Position = projection * base_view_pos;
    EmitVertex();

    // In view space.
    vec4 view_space_offset = vec4(
        normalize(gs_in[vertex_x].ViewSpaceNormal),
        0.0  // keep as zero. base_view_pos already has the 1 needed for w component.
    ) * c_magnitude;
    gl_Position = projection * (base_view_pos + view_space_offset);
    EmitVertex();

    EndPrimitive();
}

void main(){
    for(int i = 0; i < 3; ++i){
        generate_line(i);
    }
}