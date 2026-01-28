#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec3 LightPos;
in vec2 TexCoord;

uniform sampler2D diffuse_map; 
uniform vec3 lightColor;

void main()
{
    // On calcule la distance de l'objet par rapport à la caméra
    float distance = length(FragPos);

    // GESTION SPÉCIALE DU CIEL
    if(distance > 300.0) {
        // On affiche juste la texture du ciel, sans lumière et sans brouillard (Full Bright)
        FragColor = texture(diffuse_map, TexCoord);
        return; // On arrête la fonction ici pour ce pixel
    }

    // CALCUL DE LA LUMIÈRE (Pour Joueur, Sol, Obstacles)
    
    // Ambient
    float ambientStrength = 0.4;
    vec3 ambient = ambientStrength * lightColor;
  
    // Diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(LightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    // Specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(-FragPos); 
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;  
    
    vec4 texColor = texture(diffuse_map, TexCoord);
    vec3 lightingResult = (ambient + diffuse + specular) * texColor.rgb;

    // BROUILLARD 

    float density = 0.015; 
    
    // Couleur du brouillard (Bleu sombre)
    vec3 fogColor = vec3(0.05, 0.05, 0.1); 

    float fogFactor = 1.0 / exp(pow(distance * density, 2));
    fogFactor = clamp(fogFactor, 0.0, 1.0);

    vec3 finalColor = mix(fogColor, lightingResult, fogFactor);
    
    FragColor = vec4(finalColor, 1.0);
}