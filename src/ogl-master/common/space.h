#ifndef PLAYGROUND_H
#define PLAYGROUND_H

//define all Variables needed in playground
#pragma region Variables
//attributes for position in solar system 1.0f = 100 000km
//distance to sun ~150 000 000 km + radius of sun model of ~55.0f so we measure from the surface not the center
vec3 gPositionEarth(205.0f, 0.0f, 0.0f);
vec3 gOrientationEarth;
//distance to earth ~385 000km + earth model of ~0.55f
vec3 gPositionMoon(206.0f, 0.0f, 0.0f);
vec3 gOrientationMoon;
//center of sunsystem
vec3 gPositionSun(0.0f, 0.0f, 0.0f);
vec3 gOrientationSun;
//distance to sun 58 000 000 km +radius of sun model of ~55.0f
vec3 gPositionMercury(113.0f, 0.0f, 0.0f);
vec3 gOrientationMercury;

//distance to sun 108 000 000 km + radius of sun model of ~55.0f
vec3 gPositionVenus(173.0f, 0.0f, 0.0f);
vec3 gOrientationVenus;

//distance to sun 228 000 000 km + radius of sun model of ~55.0f
vec3 gPositionMars(283.0f, 0.0f, 0.0f);
vec3 gOrientationMars;

// For speed computation
double lastTime = glfwGetTime();
double lastFrameTime = lastTime;
double currentTime;
float deltaTime;
int nbFrames = 0;

GLuint programID;


//init Modevariables for Lightmode
GLuint Mode1 = 1;
GLuint Mode2 = 2;
bool ambientLight;
bool specularLight;
bool specularDisco;

// init variables for Earth
GLuint TextureEarth;
GLuint TextureIDEarth;
GLuint VertexArrayIDEarth;
GLuint vertexbufferEarth;
GLuint uvbufferEarth;
GLuint normalbufferEarth;
std::vector<glm::vec3> verticesEarth;
std::vector<glm::vec2> uvsEarth;
std::vector<glm::vec3> normalsEarth;

// init variables for Moon
GLuint TextureMoon;
GLuint TextureIDMoon;
GLuint VertexArrayIDMoon;
GLuint vertexbufferMoon;
GLuint uvbufferMoon;
GLuint normalbufferMoon;
std::vector<glm::vec3> verticesMoon;
std::vector<glm::vec2> uvsMoon;
std::vector<glm::vec3> normalsMoon;

// init variables for Sun
GLuint TextureSun;
GLuint TextureIDSun;
GLuint VertexArrayIDSun;
GLuint vertexbufferSun;
GLuint uvbufferSun;
GLuint normalbufferSun;
std::vector<glm::vec3> verticesSun;
std::vector<glm::vec2> uvsSun;
std::vector<glm::vec3> normalsSun;

// init variables for Mercury
GLuint TextureMercury;
GLuint TextureIDMercury;
GLuint VertexArrayIDMercury;
GLuint vertexbufferMercury;
GLuint uvbufferMercury;
GLuint normalbufferMercury;
std::vector<glm::vec3> verticesMercury;
std::vector<glm::vec2> uvsMercury;
std::vector<glm::vec3> normalsMercury;

// init variables for Venus
GLuint TextureVenus;
GLuint TextureIDVenus;
GLuint VertexArrayIDVenus;
GLuint vertexbufferVenus;
GLuint uvbufferVenus;
GLuint normalbufferVenus;
std::vector<glm::vec3> verticesVenus;
std::vector<glm::vec2> uvsVenus;
std::vector<glm::vec3> normalsVenus;


// init variables for Mars
GLuint TextureMars;
GLuint TextureIDMars;
GLuint VertexArrayIDMars;
GLuint vertexbufferMars;
GLuint uvbufferMars;
GLuint normalbufferMars;
std::vector<glm::vec3> verticesMars;
std::vector<glm::vec2> uvsMars;
std::vector<glm::vec3> normalsMars;

// Get a handle for our "MVP" uniform
GLuint MatrixID;
GLuint ViewMatrixID;
GLuint ModelMatrixID;

// MVP for Earth
glm::mat4 ProjectionMatrixEarth;
glm::mat4 ViewMatrixEarth;
glm::mat4 ModelMatrixEarth;
glm::mat4 MVPEarth;

// MVP for Moon
glm::mat4 ProjectionMatrixMoon;
glm::mat4 ViewMatrixMoon;
glm::mat4 ModelMatrixMoon;
glm::mat4 MVPMoon;

// MVP for Sun
glm::mat4 ProjectionMatrixSun;
glm::mat4 ViewMatrixSun;
glm::mat4 ModelMatrixSun;
glm::mat4 MVPSun;

// MVP for Mercury
glm::mat4 ProjectionMatrixMercury;
glm::mat4 ViewMatrixMercury;
glm::mat4 ModelMatrixMercury;
glm::mat4 MVPMercury;

// MVP for Venus
glm::mat4 ProjectionMatrixVenus;
glm::mat4 ViewMatrixVenus;
glm::mat4 ModelMatrixVenus;
glm::mat4 MVPVenus;

// MVP for Mars
glm::mat4 ProjectionMatrixMars;
glm::mat4 ViewMatrixMars;
glm::mat4 ModelMatrixMars;
glm::mat4 MVPMars;
#pragma endregion


int main(void); //<<< main function, called at startup

// initializes the vertex buffer array for all planets and binds it OpenGL
bool initSun();
void rotateSun();
bool initEarth(); 
void rotateEarth();
bool initMoon();
void rotateMoon();
bool initMercury();
void rotateMercury();
bool initVenus();
void rotateVenus();
bool initMars();
void rotateMars();
void switchLight();
bool drawPlanets();
#endif

