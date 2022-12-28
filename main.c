#include <GL/glew.h>
#include <GL/glut.h>
#include <GLFW/glfw3.h>

#include <stdio.h>

#define WINDOW_TITLE    "OpenGL Test Window"
#define WINDOW_WIDTH    800
#define WINDOW_HEIGHT   600

void glfwPrintError(int e, const char *s) {
    fprintf(stderr, "%s\n", s);
}

int main (void) {
    GLenum err;

	if(!glfwInit()) {
		fprintf(stderr, "GLFW: failed to initialize\n");
		return 1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

	GLFWwindow *win = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT,
            WINDOW_TITLE, NULL, NULL);

	if(!win) {
		fprintf(stderr, "GLFW: failed to create window\n");
		glfwTerminate();
	}

	glfwSetErrorCallback(&glfwPrintError);

	glfwMakeContextCurrent(win);


	if((err = glewInit()) != 0) {
        fprintf(stderr, "GLEW: %s\n", glewGetErrorString(err));
        return 2;
    }

	/*  Vertex Input (or Vertex Data)  */
	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f,
	};

	unsigned int VBO;     /* Vertex Buffer Objects */
	glGenBuffers(1, &VBO); /* Generate an unique ID to VBO */
	glBindBuffer(GL_ARRAY_BUFFER, VBO); /* bind VBO to a Vertex Buffer type */


	/* Copy the previously defined Vertex Data (vertices) into the buffer's 
	 * memory.
	 * The fourth parameter specifies how we want the GPU to manage the given
	 * data: 
	 * - GL_STREAM_DRAW: the data is set only once and used by the GPU. 
	 * - GL_STATIC_DRAW: the data is set only once and used many times.
	 * - GL_DYNAMIC_DRAW: the data is changed a lot and used many times. */
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	/* Vertex Shader */
	const char *vertexShaderSource = "#version 410 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"void main() {\n"
		"    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}\0";

	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	/* Check if the Vertex Shader compiled succesfully */
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if(!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		fprintf(stderr, "OpenGL: Vertex Shader Compilation failed\n%s\n",
				infoLog);
	}

	/* Fragment Shader */
	const char *fragmentShaderSource = "#version 410 core\n"
		"out vec4 fragColor;\n"
		"void main() {\n"
		"fragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);\n"
		"}\n";

	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	/* Check if the Fragment Shader compiled succesfully */
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if(!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		fprintf(stderr, "OpenGL: Fragment Shader Compilation failed\n%s\n",
				infoLog);
	}

	/* Shader Program */
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	/* Check if linking program failed */
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if(!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		fprintf(stderr, "OpenGL: Linking Program with Shaders failed\n%s\n",
				infoLog);
	}

	/* Free memory */
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	/* Linking Vertex attributes */
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
			3 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
	glUseProgram(shaderProgram);
	glBindVertexArray(VAO);

	while (!glfwWindowShouldClose(win)) {
		glClear(GL_COLOR_BUFFER_BIT);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(win);
		glfwPollEvents();
	}

	return 0;
}
