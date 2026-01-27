#version 330 core

// On attend 3 attributs du C++ (Position, Normale, Texture)
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

// On envoie tout ça au Fragment Shader
out vec3 FragPos;
out vec3 Normal;
out vec3 LightPos;
out vec2 TexCoord;

uniform vec3 lightPos; // Position de la lumière
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    // Calcul de la position du sommet à l'écran
    gl_Position = projection * view * model * vec4(position, 1.0);
    
    // Position du fragment dans le monde (pour calculer l'angle de lumière)
    FragPos = vec3(view * model * vec4(position, 1.0));
    
    // La normale doit aussi tourner si l'objet tourne (View Space)
    Normal = mat3(transpose(inverse(view * model))) * aNormal;
    
    // On passe la position de la lumière en View Space
    LightPos = vec3(view * vec4(lightPos, 1.0));
    
    // On passe les coordonnées de texture
    TexCoord = aTexCoord;
}