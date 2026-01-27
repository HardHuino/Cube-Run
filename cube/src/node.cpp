#include "node.h"
#include "shape.h"
#include <iostream>

Node::Node(const glm::mat4& transform) :
    transform_(transform) {

        children_ = std::vector<Node*>();
}

void Node::add(Node* node) {
    children_.push_back(node);
}

void Node::add(Shape* shape) {
    children_shape_.push_back(shape);
}

void Node::draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection) {
    glm::mat4 updatedModel = model * transform_;

    // Appliquer rotations
    updatedModel = glm::rotate(updatedModel, glm::radians(rotX), glm::vec3(1, 0, 0));
    updatedModel = glm::rotate(updatedModel, glm::radians(rotY), glm::vec3(0, 1, 0));
    updatedModel = glm::rotate(updatedModel, glm::radians(rotZ), glm::vec3(0, 0, 1));

    for (auto child : children_) {
        child->draw(updatedModel, view, projection);
    }

    for (auto child : children_shape_) {
        child->draw(updatedModel, view, projection);
    }
}

void Node::key_handler(int key) const {
    for (const auto& child : children_) {
            child->key_handler(key);
    }
}

void Node::translate(const glm::vec3& delta) {
    // On applique une translation globale
    transform_ = glm::translate(glm::mat4(1.0f), delta) * transform_;
}

glm::vec3 Node::getPosition() const {
    // La position correspond à la dernière colonne de la matrice (x, y, z)
    return glm::vec3(transform_[3]);
}

void Node::setRotationX(float angle)
{
    rotX = angle;
}

void Node::setRotationY(float angle)
{
    rotY = angle;
}

void Node::setRotationZ(float angle)
{
    rotZ = angle;
}