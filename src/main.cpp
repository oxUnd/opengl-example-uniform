#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "debug.h"
#include "model.h"

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);
void errorCallback(int, const char *errStr);
GLFWwindow *getGLFWWindow();
void run(GLFWwindow *, Model *);

const GLuint WIDTH = 800, HEIGHT = 600;

int main(int argc, char *argv[]) {
    std::cout << "PROGRAM::ARGUMENTS::COUNT = " << argc << std::endl;
    std::cout << "PROGRAM::ARGUMENTS::ARGV  = " << argv << std::endl;
    std::cout << "Staring GLFW context, OpenGL 3.3" << std::endl;
    
    GLFWwindow *window = getGLFWWindow();
    
    Model * model = new Model("triangle_vertex_shader", "triangle_fragment_shader_uniform");
    float vertices[] = {
        0.5f, 0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        -0.5f, 0.5f, 0.0f
    };

    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3
    };
    
    model->toXWithIndex(vertices, sizeof(vertices), indices, sizeof(indices));
    
    run(window, model);
    
    delete model;
    
    glfwTerminate();
    return 0;
}

void run(GLFWwindow *window, Model *model) {
    while(!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        model->render(glfwGetTime());
        
        glfwSwapBuffers(window);
    }
}

GLFWwindow *getGLFWWindow() {
    glfwSetErrorCallback(errorCallback);
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    
    // only macos
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    
    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", NULL, NULL);
    glfwMakeContextCurrent(window);
    if (window == NULL) {
        DEBUG("GLFWINIT::ERROR", "Failed to create GLFW window.", DEBUG_L::ERROR);
        return NULL;
    }
    
    glfwSetKeyCallback(window, keyCallback);
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        DEBUG("GLFWINIT::ERROR", "Failed to initialize OpenGL context", DEBUG_L::ERROR);
        return NULL;
    }
    
    return window;
}

void errorCallback(int status, const char *errStr) {
    DEBUG("GLFW::ERRORCALLBACK", errStr, DEBUG_L::ERROR);
}

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mode) {
    std::cout << key << std::endl;
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}
