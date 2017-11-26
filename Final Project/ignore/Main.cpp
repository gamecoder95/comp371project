//Lab 7 - Skybox

#include "stdafx.h"

#include <glew.h>
#include <glfw3.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"

#include "objloader.hpp"
#include "SOIL.h"

using namespace std;

// Window dimensions
const GLuint WINDOW_WIDTH = 800, WINDOW_HEIGHT = 800;
const GLfloat CAMERA_MOVEMENT_STEP = 2.20f;
const float ANGLE_ROTATION_STEP = 0.15f;

GLuint textureID_day, textureID_night;
int timeOfDay = 1;

glm::vec3 camera_position = glm::vec3(0.0f, 0.0f, -15.0f);
glm::mat4 projection_matrix;

float y_rotation_angle = 0.0f, x_rotation_angle = 0.0f; // of the model

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	projection_matrix = glm::perspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 1000.0f); // Update the Projection matrix after a window resize event
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key == GLFW_KEY_W && action == GLFW_PRESS)
		camera_position.z -= CAMERA_MOVEMENT_STEP;

	if (key == GLFW_KEY_S && action == GLFW_PRESS)
		camera_position.z += CAMERA_MOVEMENT_STEP;

	if (key == GLFW_KEY_A && action == GLFW_PRESS)
		camera_position.x -= CAMERA_MOVEMENT_STEP;

	if (key == GLFW_KEY_D && action == GLFW_PRESS)
		camera_position.x += CAMERA_MOVEMENT_STEP;

	//rotate model
	if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
		projection_matrix = glm::rotate(projection_matrix, glm::radians(5.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	if (key == GLFW_KEY_UP && action == GLFW_PRESS)
		projection_matrix = glm::rotate(projection_matrix, glm::radians(5.0f), glm::vec3(-1.0f, 0.0f, 0.0f));

	if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
		projection_matrix = glm::rotate(projection_matrix, glm::radians(5.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
		projection_matrix = glm::rotate(projection_matrix, glm::radians(5.0f), glm::vec3(0.0f, -1.0f, 0.0f));

	//Box mode
	if (key == GLFW_KEY_1 && action == GLFW_PRESS)
		timeOfDay = 1;
	if (key == GLFW_KEY_2 && action == GLFW_PRESS)
		timeOfDay = 0;
}

GLuint loadShaders(std::string vertex_shader_path, std::string fragment_shader_path) {

	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_shader_path, std::ios::in);

	if (VertexShaderStream.is_open()) {
		std::string Line = "";
		while (getline(VertexShaderStream, Line))
			VertexShaderCode += "\n" + Line;
		VertexShaderStream.close();
	}
	else {
		printf("Impossible to open %s. Are you in the right directory ?\n", vertex_shader_path.c_str());
		getchar();
		exit(-1);
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_shader_path, std::ios::in);

	if (FragmentShaderStream.is_open()) {
		std::string Line = "";
		while (getline(FragmentShaderStream, Line))
			FragmentShaderCode += "\n" + Line;
		FragmentShaderStream.close();
	}
	else {
		printf("Impossible to open %s. Are you in the right directory ?\n", fragment_shader_path.c_str());
		getchar();
		exit(-1);
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Compile Vertex Shader
	printf("Compiling shader : %s", vertex_shader_path.c_str());
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, nullptr);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, nullptr, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}

	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_shader_path.c_str());
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, nullptr);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, nullptr, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}

	// Link the program
	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);

	glBindAttribLocation(ProgramID, 0, "in_Position");

	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, nullptr, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}

	glDeleteShader(VertexShaderID); //free up memory
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}

GLuint loadCubemap(vector<const GLchar*> faces, GLuint texture)
{
	glGenTextures(1, &texture);

	int width, height;
	unsigned char* image;

	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
	for (GLuint i = 0; i < faces.size(); i++)
	{
		image = SOIL_load_image(faces[i], &width, &height, 0, SOIL_LOAD_RGB);
		glTexImage2D(
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
			GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image
		);

		SOIL_free_image_data(image); //free resources
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	cout << texture << endl;
	return texture;
}

int main()
{
	std::cout << "Starting GLFW context, OpenGL 3.3" << std::endl;
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4);
	glEnable(GL_MULTISAMPLE);

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Skybox", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	// Set the required callback functions
	glfwSetKeyCallback(window, key_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}

	// Define the viewport dimensions
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);

	glViewport(0, 0, width, height);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	GLuint cubeShaderProgram = loadShaders("cube_vertex.shader", "cube_fragment.shader");//PATH CHANGED
	GLuint skyboxShaderProgram = loadShaders("skybox_vertex.shader", "skybox_fragment.shader");//PATH CHANGED
	GLuint fogShaderProgram = loadShaders("fogVertex.shader", "fogFrag.shader");

	projection_matrix = glm::perspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);

	std::vector<glm::vec3> model_vertices;
	std::vector<glm::vec3> model_normals;
	std::vector<glm::vec2> model_UVs;

	loadOBJ("cube.obj", model_vertices, model_normals, model_UVs);

	//prepare model VAO
	GLuint modelVAO, modelVerticesVBO, modelNormalsVBO, modelUVsVBO;

	glGenVertexArrays(1, &modelVAO);
	glBindVertexArray(modelVAO);

	glGenBuffers(1, &modelVerticesVBO);
	glBindBuffer(GL_ARRAY_BUFFER, modelVerticesVBO);
	glBufferData(GL_ARRAY_BUFFER, model_vertices.size() * sizeof(glm::vec3), &model_vertices.front(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &modelNormalsVBO);
	glBindBuffer(GL_ARRAY_BUFFER, modelNormalsVBO);
	glBufferData(GL_ARRAY_BUFFER, model_normals.size() * sizeof(glm::vec3), &model_normals.front(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(1);

	glGenBuffers(1, &modelUVsVBO);
	glBindBuffer(GL_ARRAY_BUFFER, modelUVsVBO);
	glBufferData(GL_ARRAY_BUFFER, model_UVs.size() * sizeof(glm::vec2), &model_UVs.front(), GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	//-------------------------------------------------------------------------------
	//setup model texture
	glActiveTexture(GL_TEXTURE0); //select texture unit 0

	GLuint cube_texture;
	glGenTextures(1, &cube_texture);
	glBindTexture(GL_TEXTURE_2D, cube_texture); //bind this texture to the currently bound texture unit

												// Set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Load image, create texture
	int cube_texture_width, cube_texture_height;
	unsigned char* cube_image = SOIL_load_image("brick.jpg", &cube_texture_width, &cube_texture_height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, cube_texture_width, cube_texture_height, 0, GL_RGB, GL_UNSIGNED_BYTE, cube_image);

	SOIL_free_image_data(cube_image); //free resources
	//-------------------------------------------------------------------------------
	
	std::vector<glm::vec3> skybox_day_vertices;
	std::vector<glm::vec3> skybox_day_normals; //unused
	std::vector<glm::vec2> skybox_day_UVs; //unused

	loadOBJ("cube.obj", skybox_day_vertices, skybox_day_normals, skybox_day_UVs);

	//prepare skybox VAO
	GLuint skybox_day_VAO, skybox_day_VerticesVBO;

	glGenVertexArrays(1, &skybox_day_VAO);
	glBindVertexArray(skybox_day_VAO);

	glGenBuffers(1, &skybox_day_VerticesVBO);
	glBindBuffer(GL_ARRAY_BUFFER, skybox_day_VerticesVBO);
	glBufferData(GL_ARRAY_BUFFER, skybox_day_vertices.size() * sizeof(glm::vec3), &skybox_day_vertices.front(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	//prepare skybox cubemap
	vector<const GLchar*> faces_day;
	faces_day.push_back("DayBox/right.jpg");
	faces_day.push_back("DayBox/left.jpg");
	faces_day.push_back("DayBox/top.jpg");
	faces_day.push_back("DayBox/bottom.jpg");
	faces_day.push_back("DayBox/back.jpg");
	faces_day.push_back("DayBox/front.jpg");

	glActiveTexture(GL_TEXTURE1);
	GLuint cubemapTexture = loadCubemap(faces_day, textureID_day);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);

	//NIGHT VERSION//
	std::vector<glm::vec3> skybox_night_vertices;
	std::vector<glm::vec3> skybox_night_normals; //unused
	std::vector<glm::vec2> skybox_night_UVs; //unused

	loadOBJ("cube.obj", skybox_night_vertices, skybox_night_normals, skybox_night_UVs);

	//prepare skybox VAO
	GLuint skybox_night_VAO, skybox_night_VerticesVBO;

	glGenVertexArrays(1, &skybox_night_VAO);
	glBindVertexArray(skybox_night_VAO);

	glGenBuffers(1, &skybox_night_VerticesVBO);
	glBindBuffer(GL_ARRAY_BUFFER, skybox_night_VerticesVBO);
	glBufferData(GL_ARRAY_BUFFER, skybox_night_vertices.size() * sizeof(glm::vec3), &skybox_night_vertices.front(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	//prepare skybox cubemap
	vector<const GLchar*> faces_night;
	faces_night.push_back("NightBox/right.jpg");
	faces_night.push_back("NightBox/left.jpg");
	faces_night.push_back("NightBox/top.jpg");
	faces_night.push_back("NightBox/bottom.jpg");
	faces_night.push_back("NightBox/back.jpg");
	faces_night.push_back("NightBox/front.jpg");

	glActiveTexture(GL_TEXTURE2);
	GLuint cubemapTexture_night = loadCubemap(faces_night, textureID_night);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture_night);

	//---------------------------------------------------------------------------------------------------------------------------------------------------------------

	//references to uniforms
	GLuint projectionLoc = glGetUniformLocation(cubeShaderProgram, "projection_matrix");
	GLuint viewMatrixLoc = glGetUniformLocation(cubeShaderProgram, "view_matrix");
	GLuint transformLoc = glGetUniformLocation(cubeShaderProgram, "model_matrix");

	//---------------------------------------------------------------------------------------------------------------------------------------------------------------
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClearDepth(1);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_FOG);
	glFogi(GL_FOG_MODE, GL_LINEAR);
	glFogf(GL_FOG_START, 1.0);
	glFogf(GL_FOG_END, 1.0);
	float fogColor[] = { 0.5, 0.5, 0.5, 1.0 };
	glFogfv(GL_FOG_COLOR, fogColor);
	float otherCol[] = { 1.0, 1.0, 1.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, otherCol);
	//---------------------------------------------------------------------------------------------------------------------------------------------------------------

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 view_matrix;
		view_matrix = translate(view_matrix, camera_position);
//---------------------------------------------------------------------------------------------------------------------------------------------------------------
		
//draw skybox
		glUseProgram(skyboxShaderProgram);
		glm::mat4 skybox_view = glm::mat4(glm::mat3(view_matrix)); //remove the translation data
		glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, glm::value_ptr(skybox_view));

		glUniformMatrix4fv(glGetUniformLocation(skyboxShaderProgram, "view_matrix"), 1, GL_FALSE, glm::value_ptr(skybox_view));
		glUniformMatrix4fv(glGetUniformLocation(skyboxShaderProgram, "projection_matrix"), 1, GL_FALSE, glm::value_ptr(projection_matrix));

		glUniform1i(glGetUniformLocation(skyboxShaderProgram, "skyboxTexture"), timeOfDay); //use texture unit 

		glDepthMask(GL_FALSE);
		if (timeOfDay == 1) {
			glBindVertexArray(skybox_day_VAO);
			glDrawArrays(GL_TRIANGLES, 0, skybox_day_vertices.size());
			glBindVertexArray(0);
		}
		if (timeOfDay == 2) {
			glBindVertexArray(skybox_night_VAO);
			glDrawArrays(GL_TRIANGLES, 0, skybox_night_vertices.size());
			glBindVertexArray(0);
		}	
		glDepthMask(GL_TRUE);
//---------------------------------------------------------------------------------------------------------------------------------------------------------------

		//draw model
		glUseProgram(cubeShaderProgram);

		glm::mat4 model_matrix;
		model_matrix = glm::rotate(model_matrix, y_rotation_angle, glm::vec3(0.0f, 1.0f, 0.0f));
		model_matrix = glm::rotate(model_matrix, x_rotation_angle, glm::vec3(1.0f, 0.0f, 0.0f));

		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(model_matrix));
		glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, glm::value_ptr(view_matrix));
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection_matrix));

		glUniform1i(glGetUniformLocation(cubeShaderProgram, "cubeTexture"), 0); //use texture unit 0

		glBindVertexArray(modelVAO);
		glDrawArrays(GL_TRIANGLES, 0, model_vertices.size());
		glBindVertexArray(0);

		// Swap the screen buffers
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}
