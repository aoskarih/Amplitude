#include <stdio.h>
#include <iostream>
#include <cstdlib>

#include <glad/glad.h>
#include <glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <shader.h>


GLFWwindow* window;
Shader * fieldShader;

const unsigned int WIDTH = 1280;
const unsigned int HEIGHT = 720;

const int GRID_W = 80;
const int GRID_H = 45;
const int GRID_BUFFER = 10;
int GRID_C = 0;

float game_time = 0.0f;

unsigned int VBO, VAO, EBO;

int init() {
    
    if (!glfwInit()) {
        printf("glfw init failed");
    }
    printf("glfw initialized\n");

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //for mac
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    window = glfwCreateWindow(WIDTH, HEIGHT, "Amplitude", NULL, NULL);
    if (window == NULL)
    {
	printf("glfw window creation failed");
	return 0;
    }
    glfwMakeContextCurrent(window);
	
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        printf("glad init failed");
	return 0;
    }
    printf("glad initialized\n");
    
    glViewport(0, 0, WIDTH, HEIGHT);

    // wireframe
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    return 1;
}

void close() {

    glfwTerminate();

}

void processInput(GLFWwindow *window) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

void update() {

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    fieldShader->use();
    glBindVertexArray(VAO);
    
    fieldShader->setFloat("time", game_time);

    glDrawElements(GL_TRIANGLES, GRID_C*3, GL_UNSIGNED_INT, 0);
    //glDrawArrays(GL_TRIANGLES, 0, 3);

}

int main() {
    
    init(); 

    fieldShader = new Shader("src/shaders/field.vs", "src/shaders/field.fs");
    /*
    float vert [] = {
        0.0f, -0.5f, 0.5f,
        0.5f, -0.5f, 1.0f,
        0.0f, 0.5f, 0.2f
    };
    */
    int grid_w = GRID_W+2*GRID_BUFFER;
    int grid_h = GRID_H+2*GRID_BUFFER;
    float vert [grid_w * grid_h * 3] = {};
    for (int j = 0; j < grid_h; j++) {
        for (int i = 0; i < grid_w; i++) {
            vert[(j*grid_w + i) * 3] = 2.0*(((float) GRID_W)/2.0 + 4.0 - (i + 0.5*(j%2))) / (GRID_W);
            vert[(j*grid_w + i) * 3 + 1] = 2.0*(((float) GRID_H)/2.0 + 4.5 - j) / (GRID_H);
            vert[(j*grid_w + i) * 3 + 2] = ((float) rand()/2) / (RAND_MAX);
            // printf("%f, %f\n", vert[(j*grid_w + i) * 3], vert[(j*grid_w + i) * 3 + 1]);
        }
    }

    unsigned int indicies[(grid_w-1)*(grid_h-1)*2*3] = {};
    GRID_C = (grid_w-1)*(grid_h-1)*2;
    for (int j = 0; j < grid_h-1; j++) {
        for (int i = 0; i < grid_w-1; i++) {
            indicies[(j*(grid_w-1) + i)*6 + 0] = j*grid_w + i;
            indicies[(j*(grid_w-1) + i)*6 + 1] = j*grid_w + i + 1;
            indicies[(j*(grid_w-1) + i)*6 + 2] = (j+1)*grid_w + i + j%2;

            indicies[(j*(grid_w-1) + i)*6 + 3] = (j+1)*grid_w + i;
            indicies[(j*(grid_w-1) + i)*6 + 4] = (j+1)*grid_w + i + 1;
            indicies[(j*(grid_w-1) + i)*6 + 5] = j*grid_w + i + (j+1)%2;
        }
    }

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vert), vert, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glm::mat4 projection = glm::mat4(1.0f);
    //projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);
    //projection = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
    projection = glm::rotate(projection, glm::radians(30.0f), glm::vec3(1.0, 0.0, 0.0));

    glm::mat4 view = glm::mat4(1.0f);
    /*view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f),
                       glm::vec3(0.0f, 0.0f, 0.0f),
                       glm::vec3(0.0f, 1.0f, 0.0f));
    */
    //view = glm::translate(view, glm::vec3(0.0f, 0.0f, 0.0f));
    
    fieldShader->setMat4("projection", projection);
    fieldShader->setMat4("view", view);
    
    glm::vec4 te = glm::vec4(0.0f, 0.0f, 0.1f, 1.0f);
    printf("%f %f %f\n", te.x, te.y, te.z);
    te = projection * view * te;
    printf("%f %f %f\n", te.x, te.y, te.z);


    while(!glfwWindowShouldClose(window))
    {
        // input
        processInput(window);

        // rendering, logic etc.
        update();

        game_time = game_time + 0.1f;

        // other
        glfwSwapBuffers(window);
        glfwPollEvents();    
    }
    
    close();

    return 0;
}

