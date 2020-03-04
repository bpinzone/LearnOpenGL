#version 330 core

in vec3 Normal;
in vec3 WorldPos;
in vec2 TexCoords;

uniform vec3 camera_pos;

struct Material {
    // colors reflected for each light component
    sampler2D diffuse1;  // serves as diffuse AND ambient color.
    sampler2D specular1;
    sampler2D emission;
    float shininess;
};
uniform Material material;

struct Dir_light {

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    vec3 direction;

};
uniform Dir_light dir_light;

struct Point_light {

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    vec3 position;

    float constant;
    float linear;
    float quadratic;

};
#define NR_POINT_LIGHTS 4
uniform Point_light point_lights[NR_POINT_LIGHTS];

struct Spot_light {

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    vec3 position;
    vec3 direction;

    float inner_cutoff;
    float outer_cutoff;

};
uniform Spot_light spot_light;

out vec4 FragColor;

vec3 calc_dir_light(Dir_light dir_light, vec3 normal_n, vec3 frag_to_camera_n);
vec3 calc_point_light(Point_light point_light, vec3 normal_n, vec3 frag_to_camera_n);
vec3 calc_spot_light(Spot_light spot_light, vec3 normal_n, vec3 frag_to_camera_n);

// proto
// REQUIRES: dir_light.direction, normal_n, and frag_to_camera_n are normalized.
vec3 calc_dir_light(Dir_light dir_light, vec3 normal_n, vec3 frag_to_camera_n) {

    float diff = max(dot(normal_n, -dir_light.direction), 0.0);

    vec3 light_ref = normalize(reflect(dir_light.direction, normal_n));
    float spec = pow(max(dot(light_ref, frag_to_camera_n), 0.0), material.shininess);

    vec3 ambient  = dir_light.ambient  *        vec3(texture(material.diffuse1, TexCoords));
    vec3 diffuse  = dir_light.diffuse  * diff * vec3(texture(material.diffuse1, TexCoords));
    vec3 specular = dir_light.specular * spec * vec3(texture(material.specular1, TexCoords));
    return ambient + diffuse + specular;
}

// REQUIRES: normal_n, and frag_to_camera_n are normalized.
vec3 calc_point_light(Point_light point_light, vec3 normal_n, vec3 frag_to_camera_n){

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
    vec3 ambient  = point_light.ambient  *        vec3(texture(material.diffuse1, TexCoords));
    vec3 diffuse  = point_light.diffuse  * diff * vec3(texture(material.diffuse1, TexCoords));
    vec3 specular = point_light.specular * spec * vec3(texture(material.specular1, TexCoords));

    return (ambient + diffuse + specular) * attenuation;
}

// REQUIRES: spot_light.direction, normal_n, and frag_to_camera_n are normalized.
vec3 calc_spot_light(Spot_light spot_light, vec3 normal_n, vec3 frag_to_camera_n){

    vec3 frag_to_light_n = normalize(spot_light.position - WorldPos);

    float diff = max(dot(normal_n, frag_to_light_n), 0.0);

    vec3 light_to_frag = WorldPos - spot_light.position;
    vec3 light_ref_n = normalize(reflect(light_to_frag, normal_n));
    float spec = pow(max(dot(light_ref_n, frag_to_camera_n), 0.0f), material.shininess);

    // Represents theta in the picture.
    float spot_alignment = dot(normalize(light_to_frag), normalize(spot_light.direction));
    // Represents the "size" of the area between the cutoff points.
    float epsilon = spot_light.inner_cutoff - spot_light.outer_cutoff;
    // Fraction represents how much of episilon you have.
    // Fragments inside the inner cutoff have values above one.
    // Fragments outside the outer cutoff have values below zero.
    // Subtraction removes the portion outside the outer_cutoff, as it doesn't count towards you getting
    // into the inner ring.
    float intensity = clamp((spot_alignment - spot_light.outer_cutoff) / epsilon, 0.0, 1.0);

    vec3 ambient  = spot_light.ambient  *                    vec3(texture(material.diffuse1, TexCoords));
    vec3 diffuse  = spot_light.diffuse  * diff * intensity * vec3(texture(material.diffuse1, TexCoords));
    vec3 specular = spot_light.specular * spec * intensity * vec3(texture(material.specular1, TexCoords));

    return ambient + diffuse + specular;
}

void main() {

    vec3 norm = normalize(Normal);
    vec3 frag_to_camera_n = normalize(camera_pos - WorldPos);

    vec3 result = vec3(0);

    // dir
    result += calc_dir_light(dir_light, norm, frag_to_camera_n);

    // point
    for(int i = 0; i < NR_POINT_LIGHTS; ++i){
        // result += calc_point_light(point_lights[i], norm, frag_to_camera_n);
    }

    // spot
    // result += calc_spot_light(spot_light, norm, frag_to_camera_n);

    FragColor = vec4(result, 1.0);

    // emission
    // vec3 specular_pixel = vec3(texture(material.specular1, TexCoords));
    // float specular_distance = specular_pixel.x - 0.5f;
    // specular_distance *= -1;
    // vec3 emission = 0.6f *
    //     specular_distance *
    //     vec3(texture(material.emission, TexCoords));
}