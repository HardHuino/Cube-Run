#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "shape.h"

class Shape;

class Node {
public:
    Node(const glm::mat4& transform = glm::mat4(1.0f));
    void add(Node* node);
    void add(Shape* shape);
    void draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection);
    void key_handler(int key) const;
    void translate(const glm::vec3& delta);
    // ROTATIONS 
    void setRotationX(float angle);
    void setRotationY(float angle);
    void setRotationZ(float angle);
    glm::vec3 getPosition() const;
    
private:
    glm::mat4 transform_;
    //
    float rotX = 0.0f;
    float rotY = 0.0f;
    float rotZ = 0.0f;
    //
    std::vector<Node *> children_;
    std::vector<Shape *> children_shape_;
};
