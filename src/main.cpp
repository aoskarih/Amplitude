#include <stdio.h>
#include <iostream>
#include <cstdlib>

#include <glad/glad.h>
#include <glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <shader.h>


GLFWwindow* window;
Shader * field2_shader;
Shader * field1_shader;

const unsigned int WIDTH = 1920;
const unsigned int HEIGHT = 1080;

const bool fullscreen = false;

const int GRID_W = 160;
const int GRID_H = 80;
const int GRID_BUFFER = 10;
int GRID_C = 0;

const int max_f = 128;

float game_time = 0.0f;         // time
float dt = 0.0f;
float last_frame = 0.0f;

GLfloat active_f[max_f*3] = {};        // active functions

float px, py;                   // player position


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
    
    if (fullscreen) {
        window = glfwCreateWindow(WIDTH, HEIGHT, "Amplitude", glfwGetPrimaryMonitor(), NULL);
    } else {
        window = glfwCreateWindow(WIDTH, HEIGHT, "Amplitude", NULL, NULL);
    }

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
  
    glEnable(GL_LINE_SMOOTH);
    glLineWidth(2.0);
    
    glEnable(GL_DEPTH_TEST);
  //  glEnable(GL_BLEND);
  //  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
    
    return 1;
}

void close() {

    glfwTerminate();

}

void processInput(GLFWwindow *window) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    float speed = dt*1000.0;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        py += speed/HEIGHT;
    } 
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        py -= speed/HEIGHT;
    } 
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        px += speed/WIDTH;
    } 
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        px -= speed/WIDTH;
    }

}

void update() {

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    
    // Field 2 update and draw
    // ---------------------------
    // wireframe
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    field2_shader->use();
    glBindVertexArray(VAO);

    active_f[0] = 5.0;
    active_f[1] = 0.0;
    active_f[2] = 0.0;
    
    // Set center position
    field2_shader->setVec2("center", px, py);
    // Set active functions in shader
    glUniform1fv(glGetUniformLocation(field2_shader->ID, "active_f"), 3*max_f, active_f);
    // Set time in shader
    field2_shader->setFloat("time", 2*game_time);
    // Draw field 2
    glDrawElements(GL_TRIANGLES, GRID_C*3, GL_UNSIGNED_INT, 0);
    //glDrawArrays(GL_TRIANGLES, 0, 3);
    // ---------------------------

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    // Field 1 update and draw
    // ---------------------------
    field1_shader->use(); 
    
    field1_shader->setVec2("center", px, py);

    glDrawElements(GL_TRIANGLES, GRID_C*3, GL_UNSIGNED_INT, 0);

    // ---------------------------


}

int main() {
    
    init(); 

    field1_shader = new Shader("src/shaders/field1.vs", "src/shaders/field1.fs");
    field2_shader = new Shader("src/shaders/field2.vs", "src/shaders/field2.fs");
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
            vert[(j*grid_w + i) * 3 + 2] = vert[(j*grid_w + i) * 3 + 2];
        }
    }
    
    printf("y1: %f \n", (vert[(2*grid_w)*3 + 1]-vert[1]));
    printf("x1: %f \n", (vert[6]-vert[0]));

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

    px = 0.0;
    py = 0.0;


    active_f[3] = 1.0;
    active_f[4] = 0.0;
    active_f[5] = 0.0;

    while(!glfwWindowShouldClose(window))
    {

        game_time = glfwGetTime();
        dt = game_time - last_frame;
        last_frame = game_time;
        
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

