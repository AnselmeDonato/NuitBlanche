//
//  Chunk.hpp
//  NuitBlanche
//
//  Created by Anselme Donato on 18/01/2022.
//

#ifndef Chunk_h
#define Chunk_h

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <vector>
#include "Shader.hpp"

class Chunk {
    
private:
    std::vector<glm::vec3> m_cubePositions;
    unsigned int m_VBO, m_VAO, m_texture;
    
public:
    Chunk() {
        
        float m_vertices[] = {
            // positions          // texture coords
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
             0.5f, -0.5f, -0.5f,  0.1f, 0.0f,
             0.5f,  0.5f, -0.5f,  0.1f, 0.1f,
             0.5f,  0.5f, -0.5f,  0.1f, 0.1f,
            -0.5f,  0.5f, -0.5f,  0.0f, 0.1f,
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
             0.5f, -0.5f,  0.5f,  0.1f, 0.0f,
             0.5f,  0.5f,  0.5f,  0.1f, 0.1f,
             0.5f,  0.5f,  0.5f,  0.1f, 0.1f,
            -0.5f,  0.5f,  0.5f,  0.0f, 0.1f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

            -0.5f,  0.5f,  0.5f,  0.1f, 0.0f,
            -0.5f,  0.5f, -0.5f,  0.1f, 0.1f,
            -0.5f, -0.5f, -0.5f,  0.0f, 0.1f,
            -0.5f, -0.5f, -0.5f,  0.0f, 0.1f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  0.1f, 0.0f,
            
             0.5f,  0.5f,  0.5f,  0.1f, 0.0f,
             0.5f,  0.5f, -0.5f,  0.1f, 0.1f,
             0.5f, -0.5f, -0.5f,  0.0f, 0.1f,
             0.5f, -0.5f, -0.5f,  0.0f, 0.1f,
             0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  0.1f, 0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, 0.1f,
             0.5f, -0.5f, -0.5f,  0.1f, 0.1f,
             0.5f, -0.5f,  0.5f,  0.1f, 0.0f,
             0.5f, -0.5f,  0.5f,  0.1f, 0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 0.1f,
            
            -0.5f,  0.5f, -0.5f,  0.0f, 0.1f,
             0.5f,  0.5f, -0.5f,  0.1f, 0.1f,
             0.5f,  0.5f,  0.5f,  0.1f, 0.0f,
             0.5f,  0.5f,  0.5f,  0.1f, 0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 0.1f
        };
        
        // world space positions of our cubes
        for (float x = 0.0f; x < 16.0; x += 1.0f){
            for(float z = 0.0f; z < 16.0; z += 1.0f){
                m_cubePositions.push_back(glm::vec3( x, 0.0f, z));
            }
        }
    
        glGenVertexArrays(1, &m_VAO);
        glGenBuffers(1, &m_VBO);

        glBindVertexArray(m_VAO);

        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices), m_vertices, GL_STATIC_DRAW);

        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // texture coord attribute
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        
        // Textures
        glGenTextures(1, &m_texture);
        glBindTexture(GL_TEXTURE_2D, m_texture);
        // set the texture wrapping/filtering options (on currently bound texture)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        
        // load and generate the texture
        int width, height, nrChannels;
        unsigned char *data = stbi_load("/Users/anselmedonato/Desktop/Pas Telecom/OpenGL/NuitBlanche/Externals/data/textureMap.png", &width, &height, &nrChannels, 0);
        if (data) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else {
            std::cout << "Faild to load texture" <<std::endl;
        }
        stbi_image_free(data);
    };
    
    void render(Shader _shader){
        
        // bind textures on corresponding texture units
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_texture);
        
        glBindVertexArray(m_VAO);
        for (unsigned int i = 0; i < m_cubePositions.size(); i++)
        {
            // calculate the model matrix for each object and pass it to shader before drawing
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, m_cubePositions[i]);
            _shader.setMatrix4fv("model", glm::value_ptr(model));

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        
    };
};

#endif /* Camera_h */
