#ifndef VIEWER_H
#define VIEWER_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "node.h"
#include "Camera.h"
#include "pyramid.h"
#include "shape.h"
#include <vector>

class Viewer {
public:
    Viewer(int width, int height);
    void run();
    
    Node* scene_root;
    Node* player_node;
    Node* ground1;
    Node* ground2;
    Node* sky_node;
    
    Shape* player_shape;
    Shape* ground_shape;
    
    std::vector<Node*> obstacles;
    std::vector<Pyramid*> obstacle_shapes;
    
    float ground_length;
    Camera* camera;

	// Vitesse de course 
    float run_speed;                  // Initiale
	float speed_increment;            // Accelération

    // Variables pour le système de lignes 
    int num_lanes;                    // Nombre de lignes (8)
    float lane_width;                 // Largeur entre les lignes
    int current_lane;                 // Ligne actuelle du joueur
    int target_lane;                  // Ligne cible (destination)
    bool is_moving;                   // Indique qu'il se dépace horizontalement
    float lane_transition_time;       // Temps écoulé dans la transition
    float lane_transition_duration;   // Durée totale de la transition

    // Variables pour la rotation du cube
    float player_rotation_z;          // Rotation actuelle autour de l'axe Y
    float target_rotation_z;          // Rotation cible
    float start_rotation_z;           // Rotation au d�but de l'animation

    // Fonctions pour le système de lignes
    float get_lane_x_position(int lane_index);
    int get_random_lane();
    // 

private:
    GLFWwindow* win;
    int width_;
    int height_;
    
    void reset_game();
    void update_pyramid_lights(glm::mat4& view);
    static void key_callback_static(GLFWwindow* window, int key, int scancode, int action, int mods);
    void on_key(int key);
};

#endif
