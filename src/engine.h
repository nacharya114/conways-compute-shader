// System libs.
#ifndef ENGINE_H
#define ENGINE_H
#include <iostream>
#include <vector>

// Include libs.
#include <glad\glad.h>
#include <GLFW\glfw3.h>

#include <glm/glm.hpp>

// Header includes.
#include "shader.h"

// Function prototypes.
void WindowResize(GLFWwindow* a_window, int a_width, int a_height);

// Classes.
class Engine
{
public:
    int Initialize();
    float gameSpeed = 0.1f;

private:
    int screenWidth = 512;
    int screenHeight = 512;

    const char* windowName = "Default Name";

    GLFWwindow* window;
    
    //Textures
    unsigned int input_image, output_texture;

    unsigned int frame_buffer;

    //Shaders
    Shader* quadshader;
    Shader* compShader;

    // Buffers and array objects
    unsigned int VAO, VBO, EBO;

    // Game loop.
    float lastFrameTime = 0.0f;
    glm::vec3 clearColor = glm::vec3(0.0f, 0.0f, 0.0f);

public:
    Engine(int a_width, int a_height, const char* a_windowName);

    void Update(float a_deltaTime);
    void Draw();

    void ProcessInput(GLFWwindow* a_window);

private:
    // OpenGL
    void SetupOpenGlRendering();
    void computeInfo();
};

#endif