#ifndef PLAYGROUND_H
#define PLAYGROUND_H

// Include GLEW
#include <GL/glew.h>

// Include GLFW3
#include <GLFW/glfw3.h>

// Include GLM
#include <glm/glm.hpp>

#include <vector>

//some global variables for handling the vertex buffer
GLuint vertexbuffer[2];
GLuint VertexArrayID;
GLuint vertexbuffer_size;

//some global variables for handling the vertex buffer
GLuint vertexbuffer2[2];
GLuint VertexArrayID2;
GLuint vertexbuffer_size2;

//some global variables for handling the vertex buffer
GLuint vertexbuffer3[2];
GLuint VertexArrayID3;
GLuint vertexbuffer_size3;

//some global variables for handling the vertex buffer
GLuint vertexbuffer4[2];
GLuint VertexArrayID4;
GLuint vertexbuffer_size4;

//some global variables for handling the vertex buffer
GLuint Lightvertexbuffer[2];
GLuint LightVertexArrayID;
GLuint Lightvertexbuffer_size;

//program ID of the shaders, required for handling the shaders with OpenGL
GLuint programID;
GLuint lightShader;

//global variables to handle the MVP matrix
GLuint MatrixID;
glm::mat4 MVP;
GLuint MatrixIDMV;
glm::mat4 MV;

GLuint LightID;
GLuint LightMatrixIDMV;

//global variables to handle the object pose
float curr_x;
float curr_y;
float curr_angle;
float curr_rotate;
float curr_rotate2;
float curr_rotate3;
float curr_rotate4;

bool up;

float curr_translate;
float curr_lightAngle;

//Camera movement
glm::vec3 cameraPos = glm::vec3(20.0f, 15.0f, 23.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

//Globald transfomration vcariables
glm::mat4 Projection;
glm::mat4 View;

// Movement speed
float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

// Allow mouse input
bool firstMouse = true;

// Pitch
float pitch = 0.0f;

// yaw
float yaw = -90.0f;

// Mouse x position
float lastX = 400;

// Mouse y position
float lastY = 300;

// Field of view
float fov = 90.0f;


int main( void ); //<<< main function, called at startup
void updateAnimationLoop(); //<<< updates the animation loop
bool initializeWindow(); //<<< initializes the window using GLFW and GLEW
bool updateMVPTransformation(); //<<< updates the MVP transform with the current pose
bool updateLightTransformation();
bool rotate();
bool rotate2();
bool rotate3();
bool initializeVertexbuffer(); //<<< initializes the vertex buffer array and binds it OpenGL
bool initializeVertexbuffer2();
bool initializeVertexbuffer3();
bool initializeVertexbuffer4();
bool cleanupVertexbuffer(); //<<< frees all recources from the vertex buffer
bool closeWindow(); //<<< Closes the OpenGL window and terminates GLFW
void processPlayerInput();//<<< Gets Player input for a moving camera
void mouse_callback(GLFWwindow* window, double xpos, double ypos); //<<<moving the camera with the mouse
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset); //<<<zoom in and out

#endif
