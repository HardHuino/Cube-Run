#ifndef PYRAMID_H
#define PYRAMID_H

#include "shape.h"
#include "texture.h"
#include <GL/glew.h>
#include <glm/glm.hpp>

class Pyramid : public Shape {
private:
    GLuint VAO;
    GLuint buffers[2];
    GLint loc_diffuse_map;
    GLint loc_emissive_color;
    Texture* texture_;
    glm::vec3 emissiveColor_; // Couleur d'émission de la pyramide

public:
    Pyramid(Shader* shader_program, Texture* texture, glm::vec3 emissiveColor = glm::vec3(1.0f, 0.5f, 0.0f));
    ~Pyramid();
    
    void draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection) override;
    void key_handler(int key) override;
    
    glm::vec3 getEmissiveColor() const;
};

#endif
