#version 330 core

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

in vec2 TexCoords;

uniform Material material;

struct Light {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light light;
uniform vec3 viewPosition;


in vec3 Normal;
in vec3 FragmentPosition;

out vec4 FragColor;


vec3 calculateAmbientLight(){
    return light.ambient * texture(material.diffuse, TexCoords).rgb;
}

vec3 calculateDiffuseColor(vec3 normal, vec3 lightDirection){
    float diffuse = max(dot(normal,lightDirection),0.0f);
    return light.diffuse * diffuse * texture(material.diffuse, TexCoords).rgb;
}

vec3 calculateSpecularLight(vec3 viewDirection, vec3 reflectDirection){
    float spec = pow(max(dot(viewDirection, reflectDirection),0.0f),material.shininess);
    return light.specular * spec  * texture(material.specular, TexCoords).rgb;
}


void main(){

    vec3 ambientLight = calculateAmbientLight();

    vec3 norm = normalize(Normal);
    vec3 lightDirection = normalize(-light.direction);
    vec3 diffuseLight = calculateDiffuseColor(norm, lightDirection);

    vec3 viewDirection = normalize(viewPosition - FragmentPosition);
    vec3 reflectDirection = reflect(-lightDirection, norm);
    vec3 specularLight = calculateSpecularLight(viewDirection, reflectDirection);

/*
    vec3 emission = vec3(0.0);
    if (texture(material.specular, TexCoords).r == 0.0){ 

        emission = texture(material.emission, TexCoords * sin(time) + vec2(0.0,sin(time))).rgb;  
        //emission = emission * (sin(time) * 0.5 + 0.5) * 2.0;                 
    }*/

    //vec3 emission = vec3(texture(material.emission, TexCoords));       
    //emission = emission * vec3(texture(material.specular, TexCoords));

    vec3 result = ambientLight + diffuseLight + specularLight;// + emission;
    FragColor = vec4(result, 1.0f); 
}