#ifndef SHAPE_H
#define SHAPE_H

#include "shader.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shape {
public:
    GLuint shader_program_; // Public pour permettre l'accès depuis Viewer
    
    Shape(Shader* shader_program);
    virtual ~Shape() = default;
    
    virtual void draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection);
    virtual void key_handler(int key);
};

#endif
