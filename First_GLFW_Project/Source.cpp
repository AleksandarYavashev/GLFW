#include<glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "myShader.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
void generateStairPos(float* coordinates, glm::vec3* arrayPos, int size);

const unsigned SCR_HEIGHT = 720;
const unsigned SCR_WIDTH = 1280;

// camera
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

bool firstMouse = true;
float yaw = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch = 0.0f;
float lastX = 800.0f / 2.0;
float lastY = 600.0 / 2.0;
float fov = 45.0f;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

// number of stairs
const int numStairs = 10;

int main()
{

	//Initializing GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //statement to fix compilation on OS X
#endif

	//Window creation
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH,SCR_HEIGHT , "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);


	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


	//Initializing GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}


	// configure global opengl state
	glEnable(GL_DEPTH_TEST);


	//build and compile shader program
	Shader myShader("C:\\Users\\Acer\\\source\\repos\\First_GLFW_Project\\First_GLFW_Project\\shader1.vs.txt",
		"C:\\Users\\Acer\\\source\\repos\\First_GLFW_Project\\First_GLFW_Project\\shader1.fs.txt");
	myShader.use();


	//vertex coordinates
	/*
	-0.2f,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f,	// bottom left
	0.2f,	0.0f, 0.0f, 0.0f, 0.0f, 0.0f,	// bottom right   //front
	0.2f,  0.2f, 0.0f,  0.0f, 0.0f, 0.0f,	// top right
	-0.2f,	0.2f, 0.0f,  0.0f, 0.0f, 0.0f,	// top left
	-0.1f,  0.3f, 0.0f,  1.0f, 1.0f, 1.0f,	// top left     // top
	0.3f,	0.3f, 0.0f,  1.0f, 1.0f, 1.0f,	// top right
	0.3f,	0.1f, 0.0f,  1.0f, 1.0f, 1.0f	// bottom	//right side
	*/
	float vertices[] = {
		-0.2f,  0.0f, 0.0f,  1.0f, 1.0f, 1.0f,	// bottom left
		0.2f,	0.0f, 0.0f,  1.0f, 1.0f, 1.0f,	// bottom right   //front
		0.2f,  0.2f, 0.0f,   1.0f, 1.0f, 1.0f,	// top right
		0.2f,  0.2f, 0.0f,   1.0f, 1.0f, 1.0f,	// top right
		-0.2f,	0.2f, 0.0f,  1.0f, 1.0f, 1.0f,	// top left
		-0.2f,  0.0f, 0.0f,  1.0f, 1.0f, 1.0f,	// bottom left

		0.2f,  0.2f, 0.0f,   1.0f, 1.0f, 1.0f,	// top right
		-0.2f,	0.2f, 0.0f,  1.0f, 1.0f, 1.0f,	// top left 
		-0.1f,  0.3f, 0.0f,  1.0f, 1.0f, 1.0f,	// top left     // top
		-0.1f,  0.3f, 0.0f,  1.0f, 1.0f, 1.0f,	// top left     // top
		0.3f,	0.3f, 0.0f,  1.0f, 1.0f, 1.0f,	// top right   //top
		0.2f,  0.2f, 0.0f,   1.0f, 1.0f, 1.0f,	// top right

		0.3f,	0.3f, 0.0f,  0.5f, 0.5f, 0.5f,	// top right //top
		0.3f,	0.1f, 0.0f,  0.5f, 0.5f, 0.5f,	// bottom	//right side
		0.2f,	0.0f, 0.0f, 0.5f, 0.5f, 0.5f,	// bottom right   //front
		0.2f,	0.0f, 0.0f, 0.5f, 0.5f, 0.5f,	// bottom right   //front
		0.2f,  0.2f, 0.0f,   0.5f, 0.5f, 0.5f,	// top right
		0.3f,	0.3f, 0.0f,  0.5f, 0.5f, 0.5f,	// top right//top
	};

	// generating stairs from stair
	glm::vec3 stairPos[numStairs];
	generateStairPos(vertices, stairPos, numStairs);

	unsigned int VBO;
	unsigned VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//location attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//colour attributes
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	
	//Render loop
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		//input
		processInput(window);

		//rendering commands
		//------------------
		//set background colour
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// activate shader
		myShader.use();

		// pass projection matrix to shader (note that in this case it could change every frame)
		glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		myShader.setMat4("projection", projection);

		// camera/view transformation
		glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		myShader.setMat4("view", view);

		//render stairs
		glBindVertexArray(VAO);
		for (int i = 0; i < numStairs; ++i)
		{
			glm::mat4 model;
			model = glm::translate(model, stairPos[i]);
			//model = glm::rotate(model, (float)glfwGetTime() / 2.0f, glm::vec3(0.0f, 1.0f, 0.0f));
			myShader.setMat4("model", model);

			glDrawArrays(GL_TRIANGLES, 0, 18);
		}

		//chek and call events and swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//cleaning
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glfwTerminate();

	//system("pause");
	return 0;
}


//Resizing window
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}


//ESC, 1, 2 buttons pressed
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	float cameraSpeed = 2.5 * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.1f; // change this value to your liking
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	// make sure that when pitch is out of bounds, screen doesn't get flipped
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (fov >= 1.0f && fov <= 45.0f)
		fov -= yoffset;
	if (fov <= 1.0f)
		fov = 1.0f;
	if (fov >= 45.0f)
		fov = 45.0f;
}

void generateStairPos(float* coordinates, glm::vec3* arrayPos, int size)
{
	float xDif = coordinates[48] - coordinates[0];
	float yDif = coordinates[49] - coordinates[1];

	float xInit = -0.8f;
	float yInit = -0.8f;
	const float zInit = 0.0f;
	arrayPos[0] = glm::vec3(xInit, yInit, zInit);

	for (int i = 1; i < size; ++i)
	{
		xInit += xDif;
		yInit += yDif;
		arrayPos[i] = glm::vec3(xInit, yInit, zInit);
	}
}