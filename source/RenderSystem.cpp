#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "RenderSystem.h"
#include <iostream>
#include <stdexcept>
#include <unordered_map>

namespace GLFW
{
	GLFWwindow* CreateWindow();
	void framebuffer_size_callback(GLFWwindow* window, int height, int width);
}

namespace GLAD
{
	void LoadOpenGLFunPtr();
}

namespace shaders
{
	void CompileShaders(std::unordered_map<std::string, unsigned int>& shaders);
	std::unordered_map<std::string, unsigned int>& CreateShaderProgram();
	const char* vertexShaderSource =
		"#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"void main()\n"
		"{\n"
		"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}\0";

	const char* fragmentShaderSource = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
		"}\n\0";

	const char* yellowShaderSource = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"   FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
		"}\n\0";
}
void processInput(GLFWwindow* window);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;



void OpenGLPractice()
{
	GLFWwindow* window = GLFW::CreateWindow();

	
	GLAD::LoadOpenGLFunPtr();
	

	auto shaders = shaders::CreateShaderProgram();

	//set up vertex data and buffers and configure vertex attributes
	// -------------------------------------------------------------
	float firstTriangle[] =
	{
		-0.9f, -0.5f, 0.0f,  // left 
		-0.0f, -0.5f, 0.0f,  // right
		-0.45f, 0.5f, 0.0f,  // top 
	};
	float secondTriangle[] = {
		0.0f, -0.5f, 0.0f,  // left
		0.9f, -0.5f, 0.0f,  // right
		0.45f, 0.5f, 0.0f   // top 
	};


	unsigned int VBO[2], VAO[2];
	glGenVertexArrays(2, VAO);
	glGenBuffers(2, VBO);

	//bind the vertex array object first, then bind and set the vertex buffers,
	//and then configure vertex attributes
	glBindVertexArray(VAO[0]);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(firstTriangle), firstTriangle, GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(VAO[1]);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(secondTriangle), secondTriangle, GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);

	//render loop
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaders["ShaderProgram"]);
		glBindVertexArray(VAO[0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glUseProgram(shaders["ShaderProgram2"]);
		glBindVertexArray(VAO[1]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//de-allocate all resources
	glDeleteVertexArrays(2, VAO);
	glDeleteBuffers(2, VBO);
	glDeleteProgram(shaders["ShaderProgram"]);
	glDeleteProgram(shaders["ShaderProgram2"]);
	glfwTerminate();
	return;
}

// glfw: Window Creation
GLFWwindow* GLFW::CreateWindow()
{
	glfwInit();
	//tell glfw version and core profile
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Creating the actual window itself
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		throw std::runtime_error("Failed to create GLFW window");
	}
	glfwMakeContextCurrent(window);
	//set the callback for resizing
	glfwSetFramebufferSizeCallback(window, GLFW::framebuffer_size_callback);

	return window;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void GLFW::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// glad: load all opengl pointers
// // --------------------------
void GLAD::LoadOpenGLFunPtr()
{
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		throw std::runtime_error("Failed to initialize GLAD");
	}
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

//Compile all of the shaders
void shaders::CompileShaders(std::unordered_map<std::string, unsigned int>& shaders)
{
	//vertex shader
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	int success;
	char infoLog[512];

	//check for shader compile errors
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	shaders.insert(std::make_pair("vertexShader", vertexShader));

	//fragment shader
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	//check for shader compile errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT_COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	shaders.insert(std::make_pair("fragmentShader", fragmentShader));

	//yellow shader
	unsigned int yellowShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(yellowShader, 1, &yellowShaderSource, NULL);
	glCompileShader(yellowShader);

	//chec for shader compile errors
	glGetShaderiv(yellowShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::YELLOW_COMPILATION_FAILED\n" << infoLog << std::endl;	
	}
	shaders.insert(std::make_pair("yellowShader", yellowShader));
}

std::unordered_map<std::string, unsigned int>& shaders::CreateShaderProgram()
{
	std::unordered_map<std::string, unsigned int>shaders;
	CompileShaders(shaders);
	//link shaders
	unsigned int shaderProgram = glCreateProgram();
	unsigned int shaderProgram2 = glCreateProgram();
	unsigned int vShader = shaders["vertexShader"];
	unsigned int fShader = shaders["fragmentShader"];
	unsigned int yShader = shaders["yellowShader"];
	glAttachShader(shaderProgram, vShader);
	glAttachShader(shaderProgram, fShader);
	glAttachShader(shaderProgram2, vShader);
	glAttachShader(shaderProgram2, fShader);
	glLinkProgram(shaderProgram);
	glLinkProgram(shaderProgram2);

	//check for linking errors
	int success;
	char infoLog[512];
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	shaders.insert(std::make_pair("ShaderProgram", shaderProgram));
	glGetProgramiv(shaderProgram2, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram2, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	shaders.insert(std::make_pair("ShaderProgram2", shaderProgram2));
	glDeleteShader(vShader);
	glDeleteShader(fShader);
	glDeleteShader(yShader);
	return shaders;
}
