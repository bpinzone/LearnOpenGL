#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

uniform float time_seconds;

in VS_OUT {
   // Remember, this is the normal of a vertex in WORLD space!
   vec3 Normal;
   vec3 WorldPos;
   vec2 TexCoords;

} gs_in[];

out VS_OUT {
   vec3 Normal;
   vec3 WorldPos;
   vec2 TexCoords;
} gs_out;

// Important NOTE:
// Must also write to gl_Position (frag position in clip space)
// Do not mix up gs_in and gl_in
// gs_in is an array now. gs_out is not an array, it is continually assigned for emission.

// Cannot use Normal from gs_in, thats in world space, not clip space!
vec3 get_clip_space_normal() {
   vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
   vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
   return normalize(cross(a, b));
}

/*
NOTE: I think tutorial is technically incorrect. Final W value of some_gl_Position will be incorrect because it is assigned
   to be -z (view space) during perspective projection. (Before this differential is added.)

From the perspective projection article: http://www.songho.ca/opengl/gl_projectionmatrix.html
"Therefore, we can set the w-component of the clip coordinates as -ze. And, the 4th of GL_PROJECTION matrix becomes (0, 0, -1, 0)."

TODO: You could probably fix it with some math here, bc perspective division hasn't happened yet.
You would need to reassign w to be the final z value in view space.
*/
vec4 explode_gl_position(vec4 some_gl_Position, vec3 clip_normal) {
   float magnitude = 2.0;
   vec3 differential = clip_normal * magnitude * ((sin(time_seconds) + 1.0) / 2.0);
   return some_gl_Position + vec4(differential, 0.0);
}

void main() {

   vec3 clip_normal = get_clip_space_normal();

   for(int i = 0; i < 3; ++i){
      gl_Position = explode_gl_position(gl_in[i].gl_Position, clip_normal);
      gs_out.Normal = gs_in[i].Normal;
      gs_out.WorldPos = gs_in[i].WorldPos;
      gs_out.TexCoords = gs_in[i].TexCoords;
      EmitVertex();
   }

   EndPrimitive();
}
