#version 330 core

in vec3 Normal;
in vec3 WorldPos;
in vec2 TexCoords;

uniform vec3 camera_pos;

// All this lighting stuff will go into a much more generic (post processing like) shader once we do deferred shading.
// Not going to bother cleaning it up here.

struct Material {
    // colors reflected for each light component
    sampler2D diffuse1;  // serves as diffuse AND ambient color.
    sampler2D specular1;
    sampler2D emission;
    float shininess;
};
uniform Material material;


struct Point_light {

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    vec3 position;

    float constant;
    float linear;
    float quadratic;

};
uniform Point_light point_light;

out vec4 FragColor;

vec3 calc_point_light(vec3 normal_n, vec3 frag_to_camera_n);


// REQUIRES: normal_n, and frag_to_camera_n are normalized.
vec3 calc_point_light(vec3 normal_n, vec3 frag_to_camera_n){

    vec3 frag_to_light = point_light.position - WorldPos;
    vec3 frag_to_light_n = normalize(frag_to_light);

    float diff = max(dot(normal_n, frag_to_light_n), 0.0);

    vec3 light_ref_n = normalize(reflect(-frag_to_light_n, normal_n));
    float spec = pow(max(dot(frag_to_camera_n, light_ref_n), 0.0), material.shininess);

    float distance = length(frag_to_light);
    float attenuation = 1.0 / (
        point_light.constant +
        point_light.linear * distance +
        point_light.quadratic * (distance * distance)
    );
    // todo: again, stop casting to vector 3 if you want transparency.
    vec3 ambient  = point_light.ambient  *        vec3(texture(material.diffuse1, TexCoords));
    vec3 diffuse  = point_light.diffuse  * diff * vec3(texture(material.diffuse1, TexCoords));
    vec3 specular = point_light.specular * spec * vec3(texture(material.specular1, TexCoords));

    return (ambient + diffuse + specular) * attenuation;
}


void main() {

    // Visualize UV
    // FragColor = vec4(TexCoords.x, TexCoords.y, 0, 1);
    // return;

    vec3 norm = normalize(Normal);
    vec3 frag_to_camera_n = normalize(camera_pos - WorldPos);

    vec3 result = vec3(0);

    result += calc_point_light(norm, frag_to_camera_n);

    FragColor = vec4(result, 1.0);

    // TODO: For some reason, on mac, something is causing cubes to be all black.
    // But if you use this line, they show up fully lit.
    // So something about lighting on mac is broken...
    // Windows works when xcode build is clean and you rebuild the VS solution...wtf.
    // FragColor = vec4(vec3(texture(material.diffuse1, TexCoords)), 1.0);
}