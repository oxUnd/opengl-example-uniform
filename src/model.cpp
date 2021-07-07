#include <iostream>
#include <fstream>
#include <cstring>
#include <cmath>
#include <glad/glad.h>
#include "debug.h"
#include "model.h"


Model::Model(const char *vertexShaderSourcePath, const char *fragmentShaderSourcePath) {
    char vPath[128] = "\0", fPath[128] = "\0";
    fullPath(vertexShaderSourcePath, vPath);
    fullPath(fragmentShaderSourcePath, fPath);
    
    char vSource[10240] = "\0", fSource[10240] = "\0";
    loadSource(vPath, vSource);
    loadSource(fPath, fSource);
    compileVertexShader(vSource);
    compileFragmentShader(fSource);
    compileLinkProgram();
}

Model::~Model() {
    // delete all?
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(program);
}

void Model::render() {
    glUseProgram(program);
    glBindVertexArray(VAO);
    draw();
}

void Model::render(float time) {
    float red = (sin(time) / 2.0f) + 0.5f;
    float green = rand() * red;
    float blue = rand() + time;
    int vertexColorLocation = glGetUniformLocation(program, "ourColor");
    glUseProgram(program);
    glUniform4f(vertexColorLocation, red, green, blue, 1.0f);
    glBindVertexArray(VAO);
    draw();
}

void Model::draw() {
    if (useIndex) {
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    } else {
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }
}

void Model::toX(float *vertices, unsigned int size) {
    createVBO(vertices, size, NULL, 0);
}

void Model::toXWithIndex(float *vertices, unsigned int vSize, unsigned int *indices, unsigned int iSize) {
    useIndex = true;
    createVBO(vertices, vSize, indices, iSize);
}

void Model::fullPath(const char *path, char *full) {
    strcat(full, __DIR__); //@TODO
    strcat(full, path);
    strcat(full, ".txt");
    
    DEBUG("MODEL::FULLPATH", full, DEBUG_L::INFO);
}

void Model::loadSource(const char *path, char *source) {
    std::ifstream file(path);
    if (!file) {
        DEBUG("MODEL::LOADSOURCE", "failed open file", DEBUG_L::ERROR);
        return; 
    }
    
    // get length of file
    file.seekg(0, file.end);
    int length = file.tellg();
    file.seekg(0, file.beg);
    
    // if length > 10k, throw ERROR
    if (length > 10240) {
        DEBUG("MODEL::LOADSOURCE ", "length must < 1KB", DEBUG_L::ERROR);
        return;
    }
    
    // @TODO
    file.read(source, length);
    
    DEBUG("MODEL::LOADSOURCE", source, DEBUG_L::INFO);
    file.close();
}

unsigned int Model::getShaderWithSource(const char *source, unsigned int type) {
    if (!source) return -1;
    //@TODO type check
    
    int  result = 0;
    char message[512];
    unsigned int shader;
    
    shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);
    
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);

    if (!result) {
        glGetShaderInfoLog(shader, 512, NULL, message);
        // @TODO
        DEBUG("MODEL::COMPILE", message, DEBUG_L::ERROR);
    }
    
    return shader;
}

void Model::compileVertexShader(const char *source) {
    vertexShader = getShaderWithSource(source, GL_VERTEX_SHADER);
}

void Model::compileFragmentShader(const char *source) {
    fragmentShader = getShaderWithSource(source, GL_FRAGMENT_SHADER);
}

void Model::compileLinkProgram() {
    
    int result = 0;
    char message[512];
    
    program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &result);
    if (!result) {
        glGetProgramInfoLog(program, 512, NULL, message);
        DEBUG("MODEL::COMPILELINKPROGRAM", message, DEBUG_L::ERROR);
    }
    //@TODO link error
}

void Model::createVBO(float *vertices, unsigned int vSize, unsigned int *indices, unsigned int iSize) {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vSize, vertices, GL_STATIC_DRAW);

    if (useIndex) {
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, iSize, indices, GL_STATIC_DRAW);
    }

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
