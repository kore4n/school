#define GLM_ENABLE_EXPERIMENTAL

// Include standard headers
#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;


// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include <vector>
#include <iostream>

GLfloat g_vert_buffer[] = {
	-0.5f, -0.5f, 0.0f,
	-0.5f, 0.5f, 0.0f,
	0.5f, 0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,
};

GLfloat g_color_buffer[] = {
	1.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.5f, 0.5f,
};

GLuint indexArray[] = {
	0, 2, 1, 0, 3, 2
};


int main(int argc, char* argv[])
{

	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		return -1;
	}

	// Open a window and create i 	ts OpenGL context
	float screenW = 1400;
	float screenH = 900;
	window = glfwCreateWindow(screenW, screenH, "P7_Exercise_1", NULL, NULL);
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

	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	//TODO FILL IN SHADER CODE ///////////////////////////////////////////////////////////
	std::string VertexShaderCode = "\
    	#version 440\n\
		//SHADER VARIABLES\n\
		layout(location = 0) in vec4 position;\n\
		layout (location = 1) in vec4 originalColor;\n\
		layout (location = 2) out vec4 vertexColor;\n\
		void main(){ \n\
			//SHADER CODE\n\
			gl_Position = position; \n\
			vertexColor = originalColor; \n\
			//outputOriginalColor = originalColor; \n\
		}\n";

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode = "\
		#version 440\n\
		//SHADER VARIABLES\n\
		layout (location = 2) in vec4 vertexColor;\n\
		layout(location = 0) out vec4 color;\n\
		\n\
		void main() {\n\
			//SHADER CODE\n\
			color = vertexColor;\n\
			if (vertexColor.x > 0.5) { \n\
				color = vec4(0.0, 0.0, 0.0, 0.0);\n\
			}\n\
			\n\
		}\n";


	char const* VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
	glCompileShader(VertexShaderID);

	GLint Result = GL_FALSE;
	int InfoLogLength;
	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}

	// Compile Fragment Shader
	char const* FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
	glCompileShader(FragmentShaderID);

	// Link the program
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Dark blue background
	glClearColor(0.2f, 0.2f, 0.3f, 0.0f);

	do {
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(ProgramID);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(
			0,
			3,
			GL_FLOAT,
			GL_FALSE,
			0,
			g_vert_buffer
		);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(
			1,
			3,
			GL_FLOAT,
			GL_FALSE,
			0,
			g_color_buffer
		);

		// glDrawArrays(GL_TRIANGLES, 0, 3);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, indexArray);


		glUseProgram(0);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();


	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

