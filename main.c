#include <GL/glew.h>
#include <GL/glut.h>
#include <GLFW/glfw3.h>

#include <stdio.h>

#define WIN_TITLE    "OpenGL Test Window"
#define WIN_WIDTH    800
#define WIN_HEIGHT   600

#define FRAGMENT_SHADER_FILE_NAME	"main.frag"
#define VERTEX_SHADER_FILE_NAME		"main.vert"

void glfwPrintError(int e, const char *s) {
    fprintf(stderr, "%s\n", s);
}

char *loadShaderFromFile(const char *filename) {
	char *buffer;
	long len;
	FILE *f = fopen(filename, "rb");

	if(f) {
		fseek(f, 0, SEEK_END);
		len = ftell(f);
		fseek(f, 0, SEEK_SET);
		buffer = (char *)malloc(len + 1);
		buffer[len] = '\0';

		if(buffer)
			fread (buffer, 1, len, f);

		fclose (f);
	} else return NULL;

	return buffer;
}

int main (void) {
    GLenum err;
	GLFWwindow *win;
	unsigned int VBO, VAO; /* Vertex Buffer Objects, Vertex Array Objects */
	const char *vertexShaderSource, *fragmentShaderSource;
	unsigned int vertexShader, fragmentShader, shaderProgram;
	int success;
	char infoLog[512];

	if(!glfwInit()) {
		fprintf(stderr, "GLFW: failed to initialize\n");
		return 1;
	}

	if(!(win = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, WIN_TITLE, NULL, NULL)))
		glfwTerminate();

	glfwSetErrorCallback(&glfwPrintError);
	glfwMakeContextCurrent(win);

	if((err = glewInit())) {
        fprintf(stderr, "GLEW: %s\n", glewGetErrorString(err));
        return 2;
    }

	/*  Vertex Input (or Vertex Data)  */
	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f,
	};

	/* Generate an unique ID to VBO */
	glGenBuffers(1, &VBO);

	/* Bind VBO to a Vertex Buffer type */
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	/* Copy the previously defined Vertex Data (vertices) into the buffer's 
	 * memory.
	 * The fourth parameter specifies how we want the GPU to manage the given
	 * data: 
	 * - GL_STREAM_DRAW: the data is set only once and used by the GPU. 
	 * - GL_STATIC_DRAW: the data is set only once and used many times.
	 * - GL_DYNAMIC_DRAW: the data is changed a lot and used many times. */
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	/* Vertex Shader */
	if(!(vertexShaderSource = loadShaderFromFile(VERTEX_SHADER_FILE_NAME))) {
		fprintf(stderr, "could not load shader from file \"%s\n\"",
				VERTEX_SHADER_FILE_NAME);
		return 1;
	}

	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	/* Check if the Vertex Shader compiled succesfully */
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if(!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		fprintf(stderr, "OpenGL: Vertex Shader Compilation failed\n%s\n",
				infoLog);
	}

	/* Fragment Shader */
	if(!(fragmentShaderSource = loadShaderFromFile(FRAGMENT_SHADER_FILE_NAME))) {
		fprintf(stderr, "could not load shader from file \"%s\n\"",
				FRAGMENT_SHADER_FILE_NAME);
		return 1;
	}

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

	free(vertexShaderSource);
	free(fragmentShaderSource);
	glfwTerminate();
	return 0;
}
