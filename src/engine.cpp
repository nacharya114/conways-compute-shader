#include "engine.h"

#include "stb_image.h"

//Constants
// Vertex Data
float vertices[] = {
    // vert points  //texCoords
    -1.0f, -1.0f,   0.0f, 0.0f,     // bottom left
    -1.0f,  1.0f,   0.0f, 1.0f,     // top left
     1.0f, -1.0f,   1.0f, 0.0f,     // bottom right
     1.0f,  1.0f,   1.0f, 1.0f,     // top right
};

//Indices
unsigned int indices[] ={
    0, 1, 2,
    1, 2, 3
};

Engine::Engine(int a_width, int a_height, const char* a_windowName)
{
    this->screenWidth = a_width;
    this->screenHeight = a_height;
    this->windowName = a_windowName;
}

int Engine::Initialize()
{
    // Initialize GLFW.
    glfwInit();

    // Tell GLFW that we want to use OpenGL 4.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    // Tell GLFW that we want to use the OpenGL's core profile.
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

    // Do this for mac compatability.
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Create Window.

    // Instantiate the window object.
    this->window = glfwCreateWindow(this->screenWidth, this->screenHeight, this->windowName, NULL, NULL);

    // Make sure that the window is created.
    if(window == NULL)
    {
        std::cout << "Failed to create GLFW window." << std::endl;
        glfwTerminate();

        std::cin.get();
        return 0;
    }

    glfwMakeContextCurrent(window);

    // Initialize GLAD.

    // Make sure that glad has been initialized successfully.
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD." << std::endl;
        
        std::cin.get();
        return 0;
    }

    // Set the viewport

    glViewport(0, 0, this->screenWidth, this->screenHeight);

    // Setup callbacks.

    // Binds the 'framebuffer_size_callback' method to the window resize event.
    glfwSetFramebufferSizeCallback(window, WindowResize);

    this->SetupOpenGlRendering();

    // Start game loop.
    while(!glfwWindowShouldClose(this->window))
    {
        // Calculate the elapsed time between the current and previous frame.
        float m_frameTime = (float)glfwGetTime();
        float m_deltaTime = m_frameTime - this->lastFrameTime;
        this->lastFrameTime = m_frameTime;

        glfwPollEvents();
        this->ProcessInput(this->window);

        // Application logic
        this->Update(m_deltaTime);
        this->Draw();

        glfwSwapBuffers(this->window);
    }

    glfwTerminate();

    return 1;
}

void WindowResize(GLFWwindow* a_window, int a_width, int a_height)
{
    glViewport(0, 0, a_width, a_height);

    // TODO: Do your resize logic here...
}

void Engine::ProcessInput(GLFWwindow* a_window)
{
    // TODO: Process your input here...
    // std::cin.get(); 

    // If the escape key gets pressed, close the window.
    if(glfwGetKey(a_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(a_window, true);
}

void Engine::SetupOpenGlRendering()
{
    // Load image data for startup and input texture

    // glGenTextures(1, &input_texture);
    // glActiveTexture(GL_TEXTURE0);
    // glBindTexture(GL_TEXTURE_2D, input_texture);

    // // set the texture wrapping/filtering options (on the currently bound texture object)
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);  
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // int nrChannels;
    // unsigned char *data = stbi_load("resources/images/gameoflife.jpg", &screenWidth, &screenHeight, &nrChannels, 0);

    // if (data)
    // {   
    //     //Load image data to texture
    //     glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, screenWidth, screenHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data); // replace data gere
    //     glBindImageTexture(0, input_texture, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA);
    //     // glGenerateMipmap(GL_TEXTURE_2D);

    // }
    // else
    // {
    //     std::cout << "Failed to load texture" << std::endl;
    // }
    // stbi_image_free(data);
    // glBindTexture(GL_TEXTURE_2D, 0);

    // Set up output texture
    glGenTextures(1, &output_texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, output_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, screenWidth, screenHeight, 0, GL_RGBA, GL_FLOAT,
    NULL);
    glBindImageTexture(1, output_texture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);


    glViewport(0, 0, screenWidth, screenHeight);

    //Make comp shader
    static Shader c_shader("src/shaders/shader.comp");
    this->compShader = &c_shader;
    this->compShader->setInt("output_texture", 0);

    //Make Quad Shader
    static Shader shader ("src/shaders/shader.vert", "src/shaders/shader.frag");
    this->quadshader = &shader;

    // ..:: Initialization code (done once (unless your object frequently changes)) :: ..
    glGenVertexArrays(1, &VAO); 
    glGenBuffers(1, &VBO);

    // 1. bind Vertex Array Object
    glBindVertexArray(VAO);
    // 2. copy our vertices array in a buffer for OpenGL to use
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // vertex position attribute
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //vertex texture position attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1); 
    
    glGenBuffers(1, &EBO);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); 

    this->computeInfo();

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, output_texture);
    this->quadshader->setInt("myTexture", 0);
}

void Engine::Update(float a_deltaTime)
{
    // TODO: Update your logic here...


    { // launch compute shaders!

    this->compShader->use();
    glDispatchCompute((GLuint)screenWidth, (GLuint)screenHeight, 1);
    }

    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}

void Engine::Draw()
{
    // TODO: Render your stuff here..
    glClearColor(this->clearColor.x, this->clearColor.y, this->clearColor.z, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    this->quadshader->use();
    // glActiveTexture(GL_TEXTURE0);
    // glBindTexture(GL_TEXTURE_2D, input_texture);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Engine::computeInfo() {
  // show compute shader related info
  // work group handling
  // work group count
  GLint work_group_count[3];
  glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 0, &work_group_count[0]);
  glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 1, &work_group_count[1]);
  glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 2, &work_group_count[2]);
  std::cout << "total work group count x: " << work_group_count[0] << std::endl;
  std::cout << "total work group count y: " << work_group_count[1] << std::endl;
  std::cout << "total work group count z: " << work_group_count[2] << std::endl;

  // work group size
  GLint work_group_size[3];
  glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &work_group_size[0]);
  glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 1, &work_group_size[1]);
  glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 2, &work_group_size[2]);
  std::cout << "total work group size x: " << work_group_size[0] << std::endl;
  std::cout << "total work group size y: " << work_group_size[1] << std::endl;
  std::cout << "total work group size z: " << work_group_size[2] << std::endl;
  // global work group size is 512 * 512 == texture width * texture height
  // local work group size is 1 since 1 pixel at a time

  // work group invocation
  GLint work_group_inv;
  glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &work_group_inv);
  std::cout << "max work group invocation: " << work_group_inv << std::endl;
  // end of work group
}
