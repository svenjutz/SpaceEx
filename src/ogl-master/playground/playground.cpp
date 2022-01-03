// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include <common/shader.hpp>
#include <common/texture.hpp>
#include <common/controls.hpp>
#include <common/objloader.hpp>
#include <common/vboindexer.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <common/quaternion_utils.hpp>
#include <common/space.h>



int main(void)
{
	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		return -1;
	}
	
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(1024, 768, "Space Explorer", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}
	// Ensure we can capture the escape key being pressed below
	//glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	// Hide the mouse and enable unlimited mouvement
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set the mouse at the center of the screen
	glfwPollEvents();
	glfwSetCursorPos(window, 1024 / 2, 768 / 2);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	// Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);

	// Create and compile our GLSL program from the shaders
	programID = LoadShaders("StandardShading.vertexshader", "StandardShading.fragmentshader");

	// Get a handle for our "MVP" uniform
	MatrixID = glGetUniformLocation(programID, "MVP");
	ViewMatrixID = glGetUniformLocation(programID, "V");
	ModelMatrixID = glGetUniformLocation(programID, "M");

	bool vertexbufferInitializedSun = initSun();
	if (!vertexbufferInitializedSun) return -1;

	bool vertexbufferInitializedEarth = initEarth();
	if (!vertexbufferInitializedEarth) return -1;
	
	bool vertexbufferInitializedMoon = initMoon();
	if (!vertexbufferInitializedMoon) return -1;

	bool vertexbufferInitializedMercury = initMercury();
	if (!vertexbufferInitializedMercury) return -1;

	bool vertexbufferInitializedVenus = initVenus();
	if (!vertexbufferInitializedVenus) return -1;

	bool vertexbufferInitializedMars = initMars();
	if (!vertexbufferInitializedMars) return -1;


	// Get a handle for Light uniform + camara position
	glUseProgram(programID);
	
	GLuint LightID = glGetUniformLocation(programID, "LightPosition_worldspace");
	GLuint LightColor = glGetUniformLocation(programID, "LightColor");
	GLuint CamPosID = glGetUniformLocation(programID, "CamPos");
	GLuint LightMode = glGetUniformLocation(programID, "LightMode");


	glUniform3f(LightColor, 1, 1, 1);
	glUniform1i(LightMode, Mode1);

	// For speed computation
	lastTime = glfwGetTime();
	lastFrameTime = lastTime;
	nbFrames = 0;
	float lastFrame = 0.0f;
	
	do {
		// Measure speed
		currentTime = glfwGetTime();
		deltaTime = (float)(currentTime - lastFrameTime);
		lastFrameTime = currentTime;
		nbFrames++;

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use our shader
		glUseProgram(programID);

		//enables switching between lightmodes
		switchLight();
		
		// Compute the MVP matrix from keyboard and mouse input
		computeMatricesFromInputs();
		ProjectionMatrixEarth = getProjectionMatrix();
		ViewMatrixEarth = getViewMatrix();
		ModelMatrixEarth = glm::mat4(1.0);
		MVPEarth = ProjectionMatrixEarth * ViewMatrixEarth * ModelMatrixEarth;

		ProjectionMatrixMoon= getProjectionMatrix();
		ViewMatrixMoon = getViewMatrix();
		ModelMatrixMoon = glm::mat4(1.0);
		MVPMoon = ProjectionMatrixMoon * ViewMatrixMoon * ModelMatrixMoon;

		ProjectionMatrixSun = getProjectionMatrix();
		ViewMatrixSun = getViewMatrix();
		ModelMatrixSun = glm::mat4(1.0);
		MVPSun = ProjectionMatrixSun * ViewMatrixSun * ModelMatrixSun;

		ProjectionMatrixMercury = getProjectionMatrix();
		ViewMatrixMercury = getViewMatrix();
		ModelMatrixMercury = glm::mat4(1.0);
		MVPMercury = ProjectionMatrixMercury * ViewMatrixMercury * ModelMatrixMercury;

		ProjectionMatrixVenus = getProjectionMatrix();
		ViewMatrixVenus = getViewMatrix();
		ModelMatrixVenus = glm::mat4(1.0);
		MVPVenus = ProjectionMatrixVenus * ViewMatrixVenus * ModelMatrixVenus;

		ProjectionMatrixMars = getProjectionMatrix();
		ViewMatrixMars = getViewMatrix();
		ModelMatrixMars = glm::mat4(1.0);
		MVPMars = ProjectionMatrixMars * ViewMatrixMars * ModelMatrixMars;

		// Send our transformation to the currently bound shader, 
		// in the "MVP" uniform
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVPEarth[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrixEarth[0][0]);
		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrixEarth[0][0]);

		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVPMoon[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrixMoon[0][0]);
		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrixMoon[0][0]);

		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVPSun[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrixSun[0][0]);
		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrixSun[0][0]);

		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVPMercury[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrixMercury[0][0]);
		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrixMercury[0][0]);

		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVPVenus[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrixVenus[0][0]);
		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrixVenus[0][0]);

		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVPMars[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrixMars[0][0]);
		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrixMars[0][0]);

		//Set up the Light with lightpos,lightcolor and camerapos
		glm::vec3 campos = getCameraPos();
		glUniform3f(LightID, campos.x, campos.y, campos.z);
		
	
		// if discolight is activate rotate trough rgb to change light color depending on time passed
		if (specularDisco) {
			//Change the color  of the shader in dependency of the passed time
			float timeValue = glfwGetTime();
			float redValue = (tan(timeValue)/2.0f);
			float greenValue = (sin(timeValue)/2.0f);
			float blueValue = (cos(timeValue)/2.0f);

			glUniform3f(LightColor, redValue, greenValue, blueValue);

		}
		//if ambient light set lightcolor to white 
		else if(ambientLight)
		{
		glm::vec3 lightColor = glm::vec3(1, 1, 1);
		glUniform3f(LightColor, lightColor.x, lightColor.y, lightColor.z);
		}
		// if specualr light is on u get somewhat like a flashlight
		else if (specularLight)
		{
			glm::vec3 lightColor = glm::vec3(1, 1, 1);
			glUniform3f(LightColor, lightColor.x, lightColor.y, lightColor.z);
		}

		// Bind our texture in Texture Unit 0
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, TextureEarth);
		// Set our "myTextureSampler" sampler to use Texture Unit 0
		glUniform1i(TextureIDEarth, 0);

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbufferEarth);
		glVertexAttribPointer(
			0,                  // attribute
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		// 2nd attribute buffer : UVs
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, uvbufferEarth);
		glVertexAttribPointer(
			1,                                // attribute
			2,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		// 3rd attribute buffer : normals
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, normalbufferEarth);
		glVertexAttribPointer(
			2,                                // attribute
			3,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		rotateEarth();
		// Draw the triangles !
		glDrawArrays(GL_TRIANGLES, 0, verticesEarth.size());
	
		// Bind our texture in Texture Unit 1
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, TextureMoon);
		// Set our "myTextureSampler" sampler to use Texture Unit 1
		glUniform1i(TextureIDMoon, 1);

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbufferMoon);
		glVertexAttribPointer(
			0,                  // attribute
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		// 2nd attribute buffer : UVs
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, uvbufferMoon);
		glVertexAttribPointer(
			1,                                // attribute
			2,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		// 3rd attribute buffer : normals
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, normalbufferMoon);
		glVertexAttribPointer(
			2,                                // attribute
			3,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		rotateMoon();
		
		//// Draw the triangles !
		glDrawArrays(GL_TRIANGLES, 0, verticesMoon.size());


		// Bind our texture in Texture Unit 2
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, TextureSun);
		// Set our "myTextureSampler" sampler to use Texture Unit 2
		glUniform1i(TextureIDSun, 2);

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbufferSun);
		glVertexAttribPointer(
			0,                  // attribute
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		// 2nd attribute buffer : UVs
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, uvbufferSun);
		glVertexAttribPointer(
			1,                                // attribute
			2,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		// 3rd attribute buffer : normals
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, normalbufferSun);
		glVertexAttribPointer(
			2,                                // attribute
			3,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		rotateSun();
		//Draw the triangles !
		glDrawArrays(GL_TRIANGLES, 0, verticesSun.size());


		// Bind our texture in Texture Unit 3
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, TextureMercury);
		// Set our "myTextureSampler" sampler to use Texture Unit 3
		glUniform1i(TextureIDMercury, 3);

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbufferMercury);
		glVertexAttribPointer(
			0,                  // attribute
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		// 2nd attribute buffer : UVs
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, uvbufferMercury);
		glVertexAttribPointer(
			1,                                // attribute
			2,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		// 3rd attribute buffer : normals
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, normalbufferMercury);
		glVertexAttribPointer(
			2,                                // attribute
			3,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		rotateMercury();
		//Draw the triangles !
		glDrawArrays(GL_TRIANGLES, 0, verticesMercury.size());


		// Bind our texture in Texture Unit 4
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, TextureVenus);
		// Set our "myTextureSampler" sampler to use Texture Unit 4
		glUniform1i(TextureIDVenus, 4);

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbufferVenus);
		glVertexAttribPointer(
			0,                  // attribute
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		// 2nd attribute buffer : UVs
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, uvbufferVenus);
		glVertexAttribPointer(
			1,                                // attribute
			2,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		// 3rd attribute buffer : normals
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, normalbufferVenus);
		glVertexAttribPointer(
			2,                                // attribute
			3,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		rotateVenus();
		//Draw the triangles !
		glDrawArrays(GL_TRIANGLES, 0, verticesVenus.size());

		// Bind our texture in Texture Unit 5
		glActiveTexture(GL_TEXTURE5);
		glBindTexture(GL_TEXTURE_2D, TextureMars);
		// Set our "myTextureSampler" sampler to use Texture Unit 
		glUniform1i(TextureIDMars, 5);

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbufferMars);
		glVertexAttribPointer(
			0,                  // attribute
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		// 2nd attribute buffer : UVs
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, uvbufferMars);
		glVertexAttribPointer(
			1,                                // attribute
			2,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		// 3rd attribute buffer : normals
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, normalbufferMars);
		glVertexAttribPointer(
			2,                                // attribute
			3,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		rotateMars();
		//Draw the triangles !
		glDrawArrays(GL_TRIANGLES, 0, verticesMars.size());


		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
	
		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
		} // Check if the ESC key was pressed or the window was closed
		while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
			glfwWindowShouldClose(window) == 0);

		// Cleanup VBO and shader
		glDeleteBuffers(1, &vertexbufferEarth);
		glDeleteBuffers(1, &uvbufferEarth);
		glDeleteBuffers(1, &normalbufferEarth);
		glDeleteProgram(programID);
		glDeleteTextures(1, &TextureEarth);
		glDeleteVertexArrays(1, &VertexArrayIDEarth);

		glDeleteBuffers(1, &vertexbufferMoon);
		glDeleteBuffers(1, &uvbufferMoon);
		glDeleteBuffers(1, &normalbufferMoon);
		glDeleteTextures(1, &TextureMoon);
		glDeleteVertexArrays(1, &VertexArrayIDMoon);


		glDeleteBuffers(1, &vertexbufferSun);
		glDeleteBuffers(1, &uvbufferSun);
		glDeleteBuffers(1, &normalbufferSun);
		glDeleteTextures(1, &TextureSun);
		glDeleteVertexArrays(1, &VertexArrayIDSun);

		glDeleteBuffers(1, &vertexbufferMercury);
		glDeleteBuffers(1, &uvbufferMercury);
		glDeleteBuffers(1, &normalbufferMercury);
		glDeleteTextures(1, &TextureMercury);
		glDeleteVertexArrays(1, &VertexArrayIDMercury);

		glDeleteBuffers(1, &vertexbufferVenus);
		glDeleteBuffers(1, &uvbufferVenus);
		glDeleteBuffers(1, &normalbufferVenus);
		glDeleteTextures(1, &TextureVenus);
		glDeleteVertexArrays(1, &VertexArrayIDVenus);

		glDeleteBuffers(1, &vertexbufferMars);
		glDeleteBuffers(1, &uvbufferMars);
		glDeleteBuffers(1, &normalbufferMars);
		glDeleteTextures(1, &TextureMars);
		glDeleteVertexArrays(1, &VertexArrayIDMars);

		// Close OpenGL window and terminate GLFW
		glfwTerminate();

		return 0;
	}

	bool initEarth() {
		GLuint VertexArrayID;
	
		glGenVertexArrays(1, &VertexArrayID);
		glBindVertexArray(VertexArrayID);

		// Load the texture
		TextureEarth = loadDDS("erde_dds.DDS");

		// Get a handle for our "myTextureSampler" uniform
		TextureIDEarth = glGetUniformLocation(programID, "myTextureSampler");

		bool res = loadOBJ("erde.obj", verticesEarth, uvsEarth, normalsEarth);

		std::vector<unsigned short> indices;
		std::vector<glm::vec3> indexed_vertices;
		std::vector<glm::vec2> indexed_uvs;
		std::vector<glm::vec3> indexed_normals;
		indexVBO(verticesEarth, uvsEarth, normalsEarth, indices, indexed_vertices, indexed_uvs, indexed_normals);

		glGenBuffers(1, &vertexbufferEarth);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbufferEarth);
		glBufferData(GL_ARRAY_BUFFER, verticesEarth.size() * sizeof(glm::vec3), &verticesEarth[0], GL_STATIC_DRAW);

	
		glGenBuffers(1, &uvbufferEarth);
		glBindBuffer(GL_ARRAY_BUFFER, uvbufferEarth);
		glBufferData(GL_ARRAY_BUFFER, uvsEarth.size() * sizeof(glm::vec2), &uvsEarth[0], GL_STATIC_DRAW);

		glGenBuffers(1, &normalbufferEarth);
		glBindBuffer(GL_ARRAY_BUFFER, normalbufferEarth);
		glBufferData(GL_ARRAY_BUFFER, normalsEarth.size() * sizeof(glm::vec3), &normalsEarth[0], GL_STATIC_DRAW);
		return true;
	}

	bool initMoon() {
		GLuint VertexArrayID;
		glGenVertexArrays(1, &VertexArrayID);
		glBindVertexArray(VertexArrayID);

		// Load the texture
		TextureMoon = loadDDS("mond_dds.DDS");

		// Get a handle for our "myTextureSampler" uniform
		TextureIDMoon = glGetUniformLocation(programID, "myTextureSampler");

		//use the same 3obj to scale it down
		bool resMoon = loadOBJ("erde.obj", verticesMoon, uvsMoon, normalsMoon);

		std::vector<unsigned short> indices;
		std::vector<glm::vec3> indexed_vertices;
		std::vector<glm::vec2> indexed_uvs;
		std::vector<glm::vec3> indexed_normals;
		indexVBO(verticesMoon, uvsMoon, normalsMoon, indices, indexed_vertices, indexed_uvs, indexed_normals);

		glGenBuffers(1, &vertexbufferMoon);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbufferMoon);
		glBufferData(GL_ARRAY_BUFFER, verticesMoon.size() * sizeof(glm::vec3), &verticesMoon[0], GL_STATIC_DRAW);


		glGenBuffers(1, &uvbufferMoon);
		glBindBuffer(GL_ARRAY_BUFFER, uvbufferMoon);
		glBufferData(GL_ARRAY_BUFFER, uvsMoon.size() * sizeof(glm::vec2), &uvsMoon[0], GL_STATIC_DRAW);

		glGenBuffers(1, &normalbufferMoon);
		glBindBuffer(GL_ARRAY_BUFFER, normalbufferMoon);
		glBufferData(GL_ARRAY_BUFFER, normalsMoon.size() * sizeof(glm::vec3), &normalsMoon[0], GL_STATIC_DRAW);
		return true;
	}

	bool initSun() {
		GLuint VertexArrayID;

		glGenVertexArrays(1, &VertexArrayID);
		glBindVertexArray(VertexArrayID);

		// Load the texture
		TextureSun = loadDDS("sun_dds.DDS");

		// Get a handle for our "myTextureSampler" uniform
		TextureIDSun = glGetUniformLocation(programID, "myTextureSampler");

		bool resSun = loadOBJ("sun.obj", verticesSun, uvsSun, normalsSun);

		std::vector<unsigned short> indices;
		std::vector<glm::vec3> indexed_vertices;
		std::vector<glm::vec2> indexed_uvs;
		std::vector<glm::vec3> indexed_normals;
		indexVBO(verticesSun, uvsSun, normalsSun, indices, indexed_vertices, indexed_uvs, indexed_normals);

		glGenBuffers(1, &vertexbufferSun);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbufferSun);
		glBufferData(GL_ARRAY_BUFFER, verticesSun.size() * sizeof(glm::vec3), &verticesSun[0], GL_STATIC_DRAW);


		glGenBuffers(1, &uvbufferSun);
		glBindBuffer(GL_ARRAY_BUFFER, uvbufferSun);
		glBufferData(GL_ARRAY_BUFFER, uvsSun.size() * sizeof(glm::vec2), &uvsSun[0], GL_STATIC_DRAW);

		glGenBuffers(1, &normalbufferSun);
		glBindBuffer(GL_ARRAY_BUFFER, normalbufferSun);
		glBufferData(GL_ARRAY_BUFFER, normalsSun.size() * sizeof(glm::vec3), &normalsSun[0], GL_STATIC_DRAW);
		return true;
	}

	bool initMercury() {
		GLuint VertexArrayID;

		glGenVertexArrays(1, &VertexArrayID);
		glBindVertexArray(VertexArrayID);

		// Load the texture
		TextureMercury = loadDDS("mercury_dds.DDS");

		// Get a handle for our "myTextureSampler" uniform
		TextureIDMercury = glGetUniformLocation(programID, "myTextureSampler");
		//use earth object to scale it properly
		bool resMercury = loadOBJ("erde.obj", verticesMercury, uvsMercury, normalsMercury);

		std::vector<unsigned short> indices;
		std::vector<glm::vec3> indexed_vertices;
		std::vector<glm::vec2> indexed_uvs;
		std::vector<glm::vec3> indexed_normals;
		indexVBO(verticesMercury, uvsMercury, normalsMercury, indices, indexed_vertices, indexed_uvs, indexed_normals);

		glGenBuffers(1, &vertexbufferMercury);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbufferMercury);
		glBufferData(GL_ARRAY_BUFFER, verticesMercury.size() * sizeof(glm::vec3), &verticesMercury[0], GL_STATIC_DRAW);


		glGenBuffers(1, &uvbufferMercury);
		glBindBuffer(GL_ARRAY_BUFFER, uvbufferMercury);
		glBufferData(GL_ARRAY_BUFFER, uvsMercury.size() * sizeof(glm::vec2), &uvsMercury[0], GL_STATIC_DRAW);

		glGenBuffers(1, &normalbufferMercury);
		glBindBuffer(GL_ARRAY_BUFFER, normalbufferMercury);
		glBufferData(GL_ARRAY_BUFFER, normalsMercury.size() * sizeof(glm::vec3), &normalsMercury[0], GL_STATIC_DRAW);
		return true;
	}

	bool initVenus() {
		GLuint VertexArrayID;

		glGenVertexArrays(1, &VertexArrayID);
		glBindVertexArray(VertexArrayID);

		// Load the texture
		TextureVenus = loadDDS("venus_dds.DDS");

		// Get a handle for our "myTextureSampler" uniform
		TextureIDVenus = glGetUniformLocation(programID, "myTextureSampler");
		//use earth object to scale it properly
		bool resVenus = loadOBJ("erde.obj", verticesVenus, uvsVenus, normalsVenus);

		std::vector<unsigned short> indices;
		std::vector<glm::vec3> indexed_vertices;
		std::vector<glm::vec2> indexed_uvs;
		std::vector<glm::vec3> indexed_normals;
		indexVBO(verticesVenus, uvsVenus, normalsVenus, indices, indexed_vertices, indexed_uvs, indexed_normals);

		glGenBuffers(1, &vertexbufferVenus);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbufferVenus);
		glBufferData(GL_ARRAY_BUFFER, verticesVenus.size() * sizeof(glm::vec3), &verticesVenus[0], GL_STATIC_DRAW);


		glGenBuffers(1, &uvbufferVenus);
		glBindBuffer(GL_ARRAY_BUFFER, uvbufferVenus);
		glBufferData(GL_ARRAY_BUFFER, uvsVenus.size() * sizeof(glm::vec2), &uvsVenus[0], GL_STATIC_DRAW);

		glGenBuffers(1, &normalbufferVenus);
		glBindBuffer(GL_ARRAY_BUFFER, normalbufferVenus);
		glBufferData(GL_ARRAY_BUFFER, normalsVenus.size() * sizeof(glm::vec3), &normalsVenus[0], GL_STATIC_DRAW);
		return true;
	}

	bool initMars() {
		GLuint VertexArrayID;

		glGenVertexArrays(1, &VertexArrayID);
		glBindVertexArray(VertexArrayID);

		// Load the texture
		TextureMars = loadDDS("mars_dds.DDS");

		// Get a handle for our "myTextureSampler" uniform
		TextureIDMars = glGetUniformLocation(programID, "myTextureSampler");
		//use earth object to scale it properly
		bool resMars = loadOBJ("erde.obj", verticesMars, uvsMars, normalsMars);

		std::vector<unsigned short> indices;
		std::vector<glm::vec3> indexed_vertices;
		std::vector<glm::vec2> indexed_uvs;
		std::vector<glm::vec3> indexed_normals;
		indexVBO(verticesMars, uvsMars, normalsMars, indices, indexed_vertices, indexed_uvs, indexed_normals);

		glGenBuffers(1, &vertexbufferMars);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbufferMars);
		glBufferData(GL_ARRAY_BUFFER, verticesMars.size() * sizeof(glm::vec3), &verticesMars[0], GL_STATIC_DRAW);


		glGenBuffers(1, &uvbufferMars);
		glBindBuffer(GL_ARRAY_BUFFER, uvbufferMars);
		glBufferData(GL_ARRAY_BUFFER, uvsMars.size() * sizeof(glm::vec2), &uvsMars[0], GL_STATIC_DRAW);

		glGenBuffers(1, &normalbufferMars);
		glBindBuffer(GL_ARRAY_BUFFER, normalbufferMars);
		glBufferData(GL_ARRAY_BUFFER, normalsMars.size() * sizeof(glm::vec3), &normalsMars[0], GL_STATIC_DRAW);
		return true;
	}

	void rotateEarth(){
	
		//rotate earth with 360°= pi in one Minute = 60sec so 1 day = 1 Minute for full rotation around the vertcial axis
		//need detaTime *2 to do so (at least on on my pc, can vary on otherones)
		gOrientationEarth.y += 3.14159f * (deltaTime*2 / 60.0f);

		// Build the model matrix
		glm::mat4 RotationMatrix = eulerAngleYXZ(gOrientationEarth.y, gOrientationEarth.x, gOrientationEarth.z);
		glm::mat4 TranslationMatrix = translate(mat4(), gPositionEarth);
		glm::mat4 ScalingMatrix = scale(mat4(), vec3(1.0f, 1.0f, 1.0f));
		ModelMatrixEarth = TranslationMatrix * RotationMatrix * ScalingMatrix;

		MVPEarth = ProjectionMatrixEarth * ViewMatrixEarth * ModelMatrixEarth;

		// Send our transformation to the currently bound shader, 
		// in the "MVP" uniform
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVPEarth[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrixEarth[0][0]);
		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrixEarth[0][0]);
	
	}

	void rotateMoon() {
	
		//rotate moon with 360°/60 sec * 27 because it needs 27 days for one rotation
		gOrientationMoon.y += 3.14159f * (deltaTime * 2 /(60.0f *27.0f));
		
		//// Build the model matrix
		
		glm::mat4 TranslationMatrix = translate(mat4(), gPositionMoon); // A bit to the right so its beside the earth
		glm::mat4 RotationMatrix = eulerAngleYXZ(gOrientationMoon.y, gOrientationMoon.x, gOrientationMoon.z);;
		glm::mat4 ScalingMatrix = scale(mat4(), vec3(0.25f, 0.25f, 0.25f));
		ModelMatrixMoon = TranslationMatrix * RotationMatrix * ScalingMatrix ;

		MVPMoon = ProjectionMatrixMoon * ViewMatrixMoon * ModelMatrixMoon;


		// Send our transformation to the currently bound shader, 
		// in the "MVP" uniform
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVPMoon[0][0]);

		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrixMoon[0][0]);
		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrixMoon[0][0]);

	}

	void rotateSun() {

		//rotate sun with 360°/60sec * 25 because it needs 25 days for one rotation
		gOrientationSun.y += 3.14159f * (deltaTime * 2 / (60.0f * 25.0f));

		// Build the model matrix
		glm::mat4 RotationMatrix = eulerAngleYXZ(gOrientationSun.y, gOrientationSun.x, gOrientationSun.z);
		glm::mat4 TranslationMatrix = translate(mat4(), gPositionSun);
		glm::mat4 ScalingMatrix = scale(mat4(), vec3(1.0f, 1.0f, 1.0f));
		ModelMatrixSun = TranslationMatrix * RotationMatrix * ScalingMatrix;

		MVPSun = ProjectionMatrixSun * ViewMatrixSun * ModelMatrixSun;

		// Send our transformation to the currently bound shader, 
		// in the "MVP" uniform
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVPSun[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrixSun[0][0]);
		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrixSun[0][0]);
	}

	void rotateMercury() {

		//rotate Mercury with 360°/60sec * 88 because its needs 88 days for one rotation
		gOrientationMercury.y += 3.14159f * (deltaTime * 2 / (60.0f * 88.0f));

		// Build the model matrix
		glm::mat4 RotationMatrix = eulerAngleYXZ(gOrientationMercury.y, gOrientationMercury.x, gOrientationMercury.z);
		glm::mat4 TranslationMatrix = translate(mat4(), gPositionMercury);
		//scale to 0.4f to match the actual size compared to earth
		glm::mat4 ScalingMatrix = scale(mat4(), vec3(0.4f, 0.4f, 0.4f));
		ModelMatrixMercury = TranslationMatrix * RotationMatrix * ScalingMatrix;

		MVPMercury = ProjectionMatrixMercury * ViewMatrixMercury * ModelMatrixMercury;

		// Send our transformation to the currently bound shader, 
		// in the "MVP" uniform
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVPMercury[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrixMercury[0][0]);
		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrixMercury[0][0]);
	}

	void rotateVenus() {

		//rotate Venus with 360°/60sec * 243 because it needs 243 days for one rotation
		gOrientationVenus.y += -3.14159f * (deltaTime * 2 / (60.0f * 27.0f));;

		// Build the model matrix
		glm::mat4 RotationMatrix = eulerAngleYXZ(gOrientationVenus.y, gOrientationVenus.x, gOrientationVenus.z);
		glm::mat4 TranslationMatrix = translate(mat4(), gPositionVenus);

		//scale to 0.9f to match the actual size compared to earth
		glm::mat4 ScalingMatrix = scale(mat4(), vec3(0.9f, 0.9f, 0.9f));
		ModelMatrixVenus = TranslationMatrix * RotationMatrix * ScalingMatrix;

		MVPVenus = ProjectionMatrixVenus * ViewMatrixVenus * ModelMatrixVenus;

		// Send our transformation to the currently bound shader, 
		// in the "MVP" uniform
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVPVenus[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrixVenus[0][0]);
		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrixVenus[0][0]);
	}

	void rotateMars() {

		//rotate Venus with 360° /60sec  because it needs 1 day for one rotation
		gOrientationMars.y += 3.14159f * (deltaTime * 2 / (60.0f * 27.0f));

		// Build the model matrix
		glm::mat4 RotationMatrix = eulerAngleYXZ(gOrientationMars.y, gOrientationMars.x, gOrientationMars.z);
		glm::mat4 TranslationMatrix = translate(mat4(), gPositionMars);

		//scale to 0.5f to match the actual size compared to earth
		glm::mat4 ScalingMatrix = scale(mat4(), vec3(0.5f, 0.5f, 0.5f));
		ModelMatrixMars = TranslationMatrix * RotationMatrix * ScalingMatrix;

		MVPMars = ProjectionMatrixMars * ViewMatrixMars * ModelMatrixMars;

		// Send our transformation to the currently bound shader, 
		// in the "MVP" uniform
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVPMars[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrixMars[0][0]);
		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrixMars[0][0]);
	}

	void switchLight() {
		GLuint LightColor = glGetUniformLocation(programID, "LightColor");
		GLuint LightMode = glGetUniformLocation(programID, "LightMode");
		//check for press and repeat to delay the input
		if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS && GLFW_REPEAT) {
			ambientLight = !ambientLight;
			glUniform1i(LightMode, Mode1);
		}
		//check for press and repeat to delay the input
		if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS && GLFW_REPEAT) {
			specularLight = !specularLight;
			glUniform1i(LightMode, Mode2);
		}
		//check for press and repeat to delay the input
		if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS && GLFW_REPEAT) {
			specularDisco = !specularDisco;
			glUniform1i(LightMode, Mode2);
		}

	}
