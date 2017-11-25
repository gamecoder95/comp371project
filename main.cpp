#include "stdafx.h"

#include "Shader.h" // Includes all of the important libraries for OpenGL and GLM (including GLFW and GLEW)
#include <iostream>
#include <vector>
#include <string>
#include "ObjectContainer.h"
#include "Cube.h"
#include <cstdlib> // for rand() and srand() -> testing object generation
#include <ctime> // for time() -> testing object generation
#include "Terrain.h"
using namespace std;

const GLuint WIDTH = 800;
const GLuint HEIGHT = 800;

// The projection and view matrices here are just testing things, will remove when we have 
// the official camera class
glm::mat4 projection_matrix;
const glm::vec3 center(0.0f, 0.0f, 0.0f);
const glm::vec3 up(0.0f, 1.0f, 0.0f);
const glm::vec3 eye(0.0f, 0.0f, 3.0f);

//Grid stuff
GLuint VAO_grid, VBO_grid, EBO;
glm::mat4 grid_matrix;

//seed
float seed;
int main()
{
	cout << "Please input a seed: ";
	cin >> seed;
	Terrain grid = Terrain(0, 0, seed);

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Arctic Adventure", nullptr, nullptr);
	//glfwSetKeyCallback(window, key_callback);
	//glfwSetCursorPosCallback(window, mouse_callback);
	//glfwSetMouseButtonCallback(window, mouse_button_callback);
	//glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	
	int screen_width, screen_height;
	glfwGetFramebufferSize(window, &screen_width, &screen_height);

	if (window == nullptr)
	{
		cout << "Failed to create GLFW window!" << endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		cout << "Failed to initialize GLEW!" << endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}

	// For projection matrix aspect ratio (line directly below)
	//GLfloat screen_width_height_ratio = static_cast<GLfloat>(screen_width) / screen_height;
	glViewport(0, 0, screen_width, screen_height);

	// Shader
	Shader mainShader("res/shaders/vertex.shader", "res/shaders/fragment.shader");
	mainShader.useProgram();

	ObjectContainer obj_container;
	obj_container.addObject(new Cube(&mainShader, glm::vec3(5.0f, 0.0f, 0.0f)));
	obj_container.addObject(new Cube(&mainShader, glm::vec3(-5.0f, 0.0f, 0.0f)));

	projection_matrix = glm::perspective(glm::radians(45.0f), (GLfloat)screen_width / (GLfloat)screen_height, 0.1f, 100.0f);

	// TEST
	float init_time = static_cast<float>(glfwGetTime());
	srand(static_cast<unsigned int>(time(0)));
	int min = -10, max = 10;
	double fraction = 1.0 / (static_cast<double>(RAND_MAX) + 1.0);


	//TEST STUFF
	glGenVertexArrays(1, &VAO_grid);
	glGenBuffers(1, &EBO);
	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	GLuint vertices_VBO, normals_VBO;
	glGenVertexArrays(1, &VAO_grid);

	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	//Binding for grid VAO and VBO
	glBindVertexArray(VAO_grid);
	glGenBuffers(1, &VBO_grid);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_grid);
	glBufferData(GL_ARRAY_BUFFER, grid.flatten().size()*sizeof(float), &grid.flatten().front(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	GLuint gridLoc = mainShader.getUniLoc("grid_matrix");

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glm::mat4 view_matrix = glm::lookAt(eye, center, up);
		//view_matrix = glm::translate(view_matrix, glm::vec3(0.0f, 0.0f, 1000.0f));
		mainShader.setMat4("view_matrix", view_matrix);
		mainShader.setMat4("projection_matrix", projection_matrix);


		// Draw and manipulate stuff here

		glBindVertexArray(VAO_grid);
		glUniformMatrix4fv(gridLoc, 1, GL_FALSE, glm::value_ptr(grid_matrix));
		glDrawArrays(GL_LINES, 0, 121);
		// TEST STUFF!
		if (static_cast<float>(glfwGetTime()) - init_time >= 5.0f)
		{
			GLfloat x = static_cast<float>(min + (max - min + 1) * rand() * fraction);
			GLfloat y = static_cast<float>(min + (max - min + 1) * rand() * fraction);
			obj_container.addObject(new Cube(&mainShader, glm::vec3(x, y, 0.0f)));
			init_time = glfwGetTime();
		}
		
		obj_container.updateAll();

		// End draw and manipulate stuff here

		glfwSwapBuffers(window);
	}

	// Deallocate resources here (glDeleteVertexArrays and glDeleteBuffers)
	glfwTerminate();

	return EXIT_SUCCESS;
}
