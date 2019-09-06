#include <stdio.h>
#include <iostream>
#include <glad/glad.h>
#include <glfw3.h>


GLFWwindow* window;

const unsigned int WIDTH = 1280;
const unsigned int HEIGHT = 720;

int init() {
    
    if (!glfwInit()) {
        printf("glfw init failed");
    }
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //for mac
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    window = glfwCreateWindow(WIDTH, HEIGHT, "Amplitude", NULL, NULL);
    if (window == NULL)
    {
	printf("glfw window creation filed");
	return 0;
    }
    glfwMakeContextCurrent(window);
	
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        printf("glad init failed");
	return 0;
    } 
    
    glViewport(0, 0, WIDTH, HEIGHT);

    // wireframe
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    return 1;
}

void close() {

    glfwTerminate();

}

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

void update() {

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);
    glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
    glDrawArrays(GL_TRIANGLES, 0, 3);

}

int main() {
    
    init();
    
    printf("Hello, world!\n");
    
    while(!glfwWindowShouldClose(window))
    {
        // input
        processInput(window);

        // rendering, logic etc.
        update();

        // other
        glfwSwapBuffers(window);
        glfwPollEvents();    
    }
    
    close();

    return 0;
}

