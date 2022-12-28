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

	return 0;
}
