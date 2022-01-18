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
    glm::vec3 m_chunkPosition;
    std::vector<glm::vec3> m_xFacePositions;
    std::vector<glm::vec3> m_yFacePositions;
    std::vector<glm::vec3> m_zFacePositions;
    std::vector<std::vector<std::vector<int>>> m_listCubes; //3D list
    unsigned int m_texture;
    unsigned int m_xVBO, m_xVAO, m_yVBO, m_yVAO, m_zVBO, m_zVAO;
    long int m_posX, m_posY;
    
    
    // ---------- J'en ai rien à foutre je vais faire 3 passes pour rendre le chunk de manière "optimisée". Les chiens aboient, la caravane passe --------//
    //Les 3 fonctions suivantes, buildX, buildY et buildZ fonctionnent de la même manière et diffèrent juste par la face qu'elles vont rendre
    //Même chose pour les fonctions renderX, renderY et renderZ.
    
    void buildX(){
        float _vertices[] = {
            // positions          // texture coords
            0.5f,  0.5f,  0.5f,  0.1f, 0.0f,
            0.5f,  0.5f, -0.5f,  0.1f, 0.1f,
            0.5f, -0.5f, -0.5f,  0.0f, 0.1f,
            0.5f, -0.5f, -0.5f,  0.0f, 0.1f,
            0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  0.1f, 0.0f,
        };
        
        m_xFacePositions.clear();
        
        for (int x = 0; x < 16; x++){
            for (int y = 0; y < 16; y ++){
                for (int z = 0; z < 16; z ++){
                    
                    if (x == 15){//Top of the chunk, always rendered
                        if(m_listCubes[x][y][z] == 1){
                            m_xFacePositions.push_back(glm::vec3(x,y,z) + m_chunkPosition);
                        }
                    }
                    
                    else { //Middle of the chunk
                        if(m_listCubes[x][y][z] + m_listCubes[x+1][y][z] == 1){
                            m_xFacePositions.push_back(glm::vec3(x,y,z) + m_chunkPosition);
                        }
                    }
                }
            }
        }
        
        glGenVertexArrays(1, &m_xVAO);
        glGenBuffers(1, &m_xVBO);

        glBindVertexArray(m_xVAO);

        glBindBuffer(GL_ARRAY_BUFFER, m_xVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(_vertices), _vertices, GL_STATIC_DRAW);

        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // texture coord attribute
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
    };
    
    
    
    void buildY(){
        float _vertices[] = {
            // positions          // texture coords
            -0.5f,  0.5f, -0.5f,  0.0f, 0.1f,
             0.5f,  0.5f, -0.5f,  0.1f, 0.1f,
             0.5f,  0.5f,  0.5f,  0.1f, 0.0f,
             0.5f,  0.5f,  0.5f,  0.1f, 0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 0.1f
        };
        
        m_yFacePositions.clear();
        
        for (int x = 0; x < 16; x++){
            for (int y = 0; y < 16; y ++){
                for (int z = 0; z < 16; z ++){
                    
                    if (y == 15){//Top of the chunk, always rendered
                        if(m_listCubes[x][y][z] == 1){
                            m_yFacePositions.push_back(glm::vec3(x,y,z) + m_chunkPosition);
                        }
                    }
                    
                    else { //Middle of the chunk
                        if(m_listCubes[x][y][z] + m_listCubes[x][y+1][z] == 1){
                            m_yFacePositions.push_back(glm::vec3(x,y,z) + m_chunkPosition);
                        }
                    }
                }
            }
        }
        
        glGenVertexArrays(1, &m_yVAO);
        glGenBuffers(1, &m_yVBO);

        glBindVertexArray(m_yVAO);

        glBindBuffer(GL_ARRAY_BUFFER, m_yVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(_vertices), _vertices, GL_STATIC_DRAW);

        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // texture coord attribute
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
    };
    
    
    
    
    void buildZ(){
        float _vertices[] = {
            // positions          // texture coords
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
             0.5f, -0.5f,  0.5f,  0.1f, 0.0f,
             0.5f,  0.5f,  0.5f,  0.1f, 0.1f,
             0.5f,  0.5f,  0.5f,  0.1f, 0.1f,
            -0.5f,  0.5f,  0.5f,  0.0f, 0.1f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f
        };
        
        m_zFacePositions.clear();
        
        for (int x = 0; x < 16; x++){
            for (int y = 0; y < 16; y ++){
                for (int z = 0; z < 16; z ++){
                    
                    if (z == 15){//Top of the chunk, always rendered
                        if(m_listCubes[x][y][z] == 1){
                            m_zFacePositions.push_back(glm::vec3(x,y,z) + m_chunkPosition);
                        }
                    }
                    
                    else { //Middle of the chunk
                        if(m_listCubes[x][y][z] + m_listCubes[x][y][z+1] == 1){
                            m_zFacePositions.push_back(glm::vec3(x,y,z) + m_chunkPosition);
                        }
                    }
                }
            }
        }
        
        glGenVertexArrays(1, &m_zVAO);
        glGenBuffers(1, &m_zVBO);

        glBindVertexArray(m_zVAO);

        glBindBuffer(GL_ARRAY_BUFFER, m_zVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(_vertices), _vertices, GL_STATIC_DRAW);

        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // texture coord attribute
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
    };
    
    
    void renderX(Shader _shader){
        glBindVertexArray(m_xVAO);
        for (unsigned int i = 0; i < m_xFacePositions.size(); i++)
        {
            // calculate the model matrix for each object and pass it to shader before drawing
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, m_xFacePositions[i]);
            _shader.setMatrix4fv("model", glm::value_ptr(model));

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
    };
    
    void renderY(Shader _shader){
        glBindVertexArray(m_yVAO);
        for (unsigned int i = 0; i < m_yFacePositions.size(); i++)
        {
            // calculate the model matrix for each object and pass it to shader before drawing
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, m_yFacePositions[i]);
            _shader.setMatrix4fv("model", glm::value_ptr(model));

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
    };
    
    void renderZ(Shader _shader){
        glBindVertexArray(m_zVAO);
        for (unsigned int i = 0; i < m_zFacePositions.size(); i++)
        {
            // calculate the model matrix for each object and pass it to shader before drawing
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, m_zFacePositions[i]);
            _shader.setMatrix4fv("model", glm::value_ptr(model));

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
    };
    
    // --------------------------------------------- ça marche, nique les rageux --------------------------------------------------------------//
    
public:
    Chunk(glm::vec3 _chunkPosition) : m_listCubes(16, std::vector<std::vector<int> >(16, std::vector<int>(16, 1))), m_chunkPosition(_chunkPosition) {
        
        m_listCubes[3][3][3] = 0;
        
        buildX();
        buildY();
        buildZ();

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
        
        //Rendering faces in each direction in an optimized way.
        renderX(_shader);
        renderY(_shader);
        renderZ(_shader);
    };
};

#endif /* Chunk_h */
