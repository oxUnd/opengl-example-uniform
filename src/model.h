#ifndef __GL_MODEL_H__
#define __GL_MODEL_H__

// #define __DIR__ std::parseLastDir(__FILE__)

class Model {
public:
    Model(
        const char *vertexShaderSourcePath,
        const char *fragmentShaderSourcePath);
        
    ~Model();
    // render model to display
    void render();
    void render(float time);
    void toX(float *vertices, unsigned int size);
    void toXWithIndex(float *vertices, unsigned int vSize, unsigned int *indices, unsigned int iSize);
    
private:
    unsigned int VBO;
    unsigned int VAO;
    unsigned int EBO;
    unsigned int program;
    unsigned int vertexShader;
    unsigned int fragmentShader;

    bool useIndex = false;
    
    void draw();

    void fullPath(const char *path, char *full);
    void loadSource(const char *path, char *source);
    unsigned int getShaderWithSource(const char *source, unsigned int type);
    void compileFragmentShader(const char *source);
    void compileVertexShader(const char *source);
    void compileLinkProgram();
    void createVBO(float *vertices, unsigned int vSize, unsigned int *indices, unsigned int iSize);
};

#endif
