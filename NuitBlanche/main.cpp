#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include "Shader.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define CAMERA_IMPLEMENTATION
#include "Camera.h"

#define CHUNK_IMPLEMENTATION
#include "Chunk.hpp"



//--------------------------------- Declarations ------------------------------------------//


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

//Variables
float deltaTime = 0.0f;
float lastFrame = 0.0f;
float PI = 3.1415f;

//Camera initialization
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
Camera camera = Camera(cameraPos, cameraUp);

int wireframeMode = 0;

//----------------------------------------------------------------------------------------//

//------------------------------- Other functions ----------------------------------------//

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow *window)
{
    //Commands
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    if(glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
        if(wireframeMode){
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            wireframeMode = 0;
        }
        else {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            wireframeMode = 1;
        }
    }
    
    //Camera movement
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.processKey(FORWARD, deltaTime);
    }
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.processKey(LEFT, deltaTime);
    }
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.processKey(BACKWARD, deltaTime);
    }
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.processKey(RIGHT, deltaTime);
    }
    if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        camera.processKey(UP, deltaTime);
    }
    if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        camera.processKey(DOWN, deltaTime);
    }
    
}


void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    camera.processLook(xpos, ypos);
}


// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int _width, int _height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, _width, _height);
}
//----------------------------------------------------------------------------------------//

//----------------------------------- Main -----------------------------------------------//

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    
    //Hiding and capturing the cursor
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    Chunk m_chunk = Chunk();
    
    //Shaders
    Shader _shader("/Users/anselmedonato/Desktop/Pas Telecom/OpenGL/NuitBlanche/NuitBlanche/vertexShader.glsl","/Users/anselmedonato/Desktop/Pas Telecom/OpenGL/NuitBlanche/NuitBlanche/fragmentShader.glsl");

    //Activate the shader
    _shader.use();
     
    //Z-buffer
    glEnable(GL_DEPTH_TEST);

    //---------------------------------------------- ------------- -------------------------------------------------------------//
    
    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);
        
        //Time since the last frame
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // render
        // ------
        glClearColor(245.f/255, 214.f/255, 175.f/255, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // activate shader
        _shader.use();

        //View
        //glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        glm::mat4 view = camera.getView();
        _shader.setMatrix4fv("view", glm::value_ptr(view));
        
        //Projection
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
        _shader.setMatrix4fv("projection", glm::value_ptr(projection));
        
        m_chunk.render(_shader);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}
