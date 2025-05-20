#pragma once

#include <glm/glm.hpp>


namespace strvy {

    struct alignas(16) CameraData
    {
        glm::mat4 viewProjection;
        glm::vec3 viewPos;
        int _pad0;
    };

    struct alignas(16) GLSLLight
    {
        glm::vec3 position;
        int _pad0;

        //glm::vec3 direction;
        //int _pad0; // probably they don't need

        glm::vec3 ambient;
        int _pad1;
        glm::vec3 diffuse;
        int _pad2;
        glm::vec3 specular;
        int _pad3;
        // other lighting parameters
    };

    struct alignas(16) LightBlock
    {
        uint32_t lightCount;
        glm::vec3 _pad0; // padding variable to fit into 16 bytes per variable layout (std140)
        GLSLLight lights[10]; // maximum 100 in forward rendering 

    };

    struct alignas(16) Material
    {
        float shininess;
        glm::vec3 _pad3;

        glm::vec3 ambient;
        float _pad0;

        glm::vec3 diffuse;
        float _pad1;

        glm::vec3 specular;
        float _pad2;
    };

    struct alignas(16) MaterialBlock
    {
        Material materials[5];
    };
	
    struct alignas(16) InstanceBlock 
    {
        glm::mat4 transform;

        int entityID;
        glm::vec3 _pad0;

        glm::vec4 color;
    };

}