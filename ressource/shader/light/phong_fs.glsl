#version 330 core

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};
uniform Material material;

struct DirirectionalLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform DirirectionalLight directionalLight;

#define NR_POINT_LIGHTS 4
struct PointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform PointLight pointLight[NR_POINT_LIGHTS];


struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform SpotLight spotLight;

uniform vec3 viewPosition;

in vec3 Normal;
in vec3 FragmentPosition;
in vec2 TexCoords;
out vec4 FragColor;



vec3 calculateDiffuseColor(vec3 diffuse, vec3 normal, vec3 lightDirection){
    float diff = max(dot(normal,lightDirection),0.0f);
    return diffuse * (diff * vec3(texture(material.diffuse, TexCoords)));
}

vec3 calculateSpecularLight(vec3 specular, vec3 viewDirection, vec3 normal, vec3 lightDirection){
    vec3 reflectDirection = reflect(-lightDirection, normal);
    float spec = pow(max(dot(viewDirection, reflectDirection),0.0f),material.shininess);
    return specular * (spec * vec3(texture(material.specular, TexCoords)));
}

vec3 calculateAmbientLight(vec3 ambient){
    return ambient * vec3(texture(material.diffuse, TexCoords));
}


vec3 calculateDirectionalLight(DirirectionalLight dirLight, vec3 normal, vec3 viewDirection){
    vec3 lightDirection = normalize(-dirLight.direction);
    
    // diffuse shading
    vec3 diffuseLight = calculateDiffuseColor(dirLight.diffuse, normal, lightDirection);
    // specular shading
    vec3 specularLight = calculateSpecularLight(dirLight.specular, viewDirection, normal, lightDirection);
    // ambient shading
    vec3 ambientLight = calculateAmbientLight(dirLight.ambient);

    // combine results
    return ambientLight + diffuseLight + specularLight;
}


vec3 calculatePointLight(PointLight pointLight, vec3 normal, vec3 fragmentPosition, vec3 viewDirection){

    float distance = length(pointLight.position - fragmentPosition);
    float attenuation = 1.0f / (pointLight.constant + pointLight.linear * distance + pointLight.quadratic * (distance * distance));
    vec3 ambientLight = calculateAmbientLight(pointLight.ambient);
    vec3 diffuseLight = calculateDiffuseColor(pointLight.diffuse, normal, normalize(pointLight.position - fragmentPosition));
    vec3 specularLight = calculateSpecularLight(pointLight.specular, viewDirection, normal, normalize(pointLight.position - fragmentPosition));

    ambientLight *= attenuation;
    diffuseLight *= attenuation;
    specularLight *= attenuation;
    return ambientLight + diffuseLight + specularLight;
}


vec3 calculateSpotLight(SpotLight spotLight, vec3 normal, vec3 fragmentPosition, vec3 viewDirection){
    vec3 lightDirection = normalize(spotLight.position - fragmentPosition);
    float theta = dot(lightDirection, normalize(-spotLight.direction));
    float epsilon = spotLight.cutOff - spotLight.outerCutOff;
    float distance = length(spotLight.position - fragmentPosition);
    float attenuation = 1.0f / (spotLight.constant + spotLight.linear * distance + spotLight.quadratic * pow(distance, 2.0f));

    float intensity = clamp((theta - spotLight.outerCutOff) / epsilon, 0.0f, 1.0f);


    vec3 color = vec3(0.0f);

    vec3 ambientLight = calculateAmbientLight(spotLight.ambient);
    vec3 diffuseLight = calculateDiffuseColor(spotLight.diffuse, normal, lightDirection);
    vec3 specularLight = calculateSpecularLight(spotLight.specular, viewDirection, normal, lightDirection);

    diffuseLight *= intensity;
    specularLight *= intensity;

    ambientLight *= attenuation;
    diffuseLight *= attenuation;
    specularLight *= attenuation;

    color = ambientLight + diffuseLight + specularLight;


    return color;
}

void main(){


    vec3 normal = normalize(Normal);
    vec3 viewDirection = normalize(viewPosition - FragmentPosition);
    vec3 result = calculateDirectionalLight(directionalLight, 
                                            normal, 
                                            viewDirection);

    for(int i=0; i < NR_POINT_LIGHTS; i++){
        result += calculatePointLight(pointLight[i], 
                                    normal, 
                                    FragmentPosition, 
                                    viewDirection);
    }

    result += calculateSpotLight(spotLight, 
                                normal, 
                                FragmentPosition, 
                                viewDirection);
    FragColor = vec4(result, 1.0f); 
}