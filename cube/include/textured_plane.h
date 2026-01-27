#pragma once
#ifndef TEXTURED_PLANE_H
#define TEXTURED_PLANE_H

#include "shape.h"
#include "texture.h"
#include <GL/glew.h>

class TexturedPlane : public Shape {
public:
    TexturedPlane(Shader* shader_program, Texture* texture);
    ~TexturedPlane();

    void draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection) override;
    void key_handler(int key) override;

private:
    GLuint VAO;
    GLuint buffers[3];  // vertex positions, texture coords, indices
    Texture* texture;
    GLint loc_diffuse_map;
};

#endif // TEXTURED_PLANE_H