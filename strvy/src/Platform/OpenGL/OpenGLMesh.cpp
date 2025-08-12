#include "svpch.h"
#include "OpenGLMesh.h"

#include <glad/glad.h>

namespace strvy {

    OpenGLMesh::OpenGLMesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices, std::vector<Ref<Texture2D>> textures)
    {
        m_vertices = vertices;
        m_indices = indices;
        m_textures = textures;

        setupMesh();
    }

    void OpenGLMesh::draw(Ref<Shader> shader)
    {
        uint32_t diffuseNr = 1;
        uint32_t specularNr = 1;
        for (uint32_t i = 0; i < m_textures.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + i); // activate proper texture unit before binding
            // retrieve texture number (the N in diffuse_textureN)
            std::string number;
            std::string name = m_textures[i]->getTextureTypeAsString();
            if (name == "texture_diffuse")
                number = std::to_string(diffuseNr++);
            else if (name == "texture_specular")
                number = std::to_string(specularNr++);

            shader->setInt((name + number).c_str(), i);
            glBindTexture(GL_TEXTURE_2D, m_textures[i]->getRendererID());
        }
        glActiveTexture(GL_TEXTURE0);

        // draw mesh
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    void OpenGLMesh::setupMesh()
    {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), &m_vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int),
            &m_indices[0], GL_STATIC_DRAW);

        // vertex positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        // vertex normals
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
        // vertex texture coords
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

        glBindVertexArray(0);
    }

}