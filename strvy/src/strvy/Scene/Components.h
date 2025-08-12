#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include "SceneCamera.h"
#include "strvy/Renderer/Model.h"
#include "ScriptableEntity.h"
#include "PrimitiveGeometry.h"

namespace strvy {

    struct TagComponent
    {
        std::string tag;

        TagComponent() = default;
        TagComponent(const TagComponent&) = default;
        TagComponent(const std::string& tag)
            : tag(tag) {}
    };

    struct TransformComponent
    {
        glm::vec3 translation = { 0.0f, 0.0f, 0.0f };
        glm::vec3 rotation = { 0.0f, 0.0f, 0.0f };
        glm::vec3 scale = { 1.0f, 1.0f, 1.0f };

        TransformComponent() = default;
        TransformComponent(const TransformComponent&) = default;
        TransformComponent(const glm::vec3& translation)
            : translation(translation) {}

        glm::mat4 getTransform() const
        {
            glm::mat4 lrotation = glm::toMat4(glm::quat(rotation));


            return glm::translate(glm::mat4(1.0f), translation)
                * lrotation
                * glm::scale(glm::mat4(1.0f), scale);
        }
    };


    // mesh refers to a 3D object that consists of meshes, which can be classified as one of the built-in primitive geometries (cube, sphere, etc.), or as a model object (uploaded 3D model)
    //struct ModelComponent
    //{
    //    Ref<Model> model;
    //    // might be complemented with other mesh properties 

    //    MeshComponent() = default;
    //    MeshComponent(const MeshComponent&) = default;
    //    MeshComponent(const Ref<MaterialInstance> material)
    //        : material(material) {}
    //    
    //};

    struct ModelComponent
    {
        Ref<Model> model;

        ModelComponent() = default;
        ModelComponent(const ModelComponent&) = default;
        ModelComponent(const Ref<Model>& model)
            : model(model) {}
    };

    // for primitive geometries use instanced rendering with advance to MDI further
    struct PrimitiveComponent
    {
        Ref<PrimitiveGeometry> geometry;
        glm::vec4 color{ 1.0f, 1.0f, 1.0f, 1.0f };

        PrimitiveComponent() = default;
        PrimitiveComponent(const PrimitiveComponent&) = default;
        PrimitiveComponent(const Ref<PrimitiveGeometry>& geometry)
            : geometry(geometry) {}
        PrimitiveComponent(const Ref<PrimitiveGeometry>& geometry, const glm::vec4& color)
            : geometry(geometry), color(color) {}
    };

    struct MaterialComponent
    {
        float shininess = 32.0f;
        glm::vec3 pad0;

        glm::vec3 ambient = glm::vec3(1.0f, 0.5f, 0.31f);
        float pad1;

        glm::vec3 diffuse = glm::vec3(1.0f, 0.5f, 0.31f);
        float pad2;

        glm::vec3 specular = glm::vec3(0.5f, 0.5f, 0.5f);
        float pad3;

        MaterialComponent() = default;
        MaterialComponent(const MaterialComponent&) = default;
    };

    struct SpriteRendererComponent
    {
        glm::vec4 color{ 1.0f, 1.0f, 1.0f, 1.0f };
        // Ref<MaterialInstance>  Material is basically a shader plus uniform data

        SpriteRendererComponent() = default;
        SpriteRendererComponent(const SpriteRendererComponent&) = default;
        SpriteRendererComponent(const glm::vec4& color)
            : color(color) {}
    };

    struct CameraComponent
    {
        SceneCamera camera;
        bool primary = true; // TODO: think about moving to scene 
        bool fixedAspectRatio = false;

        CameraComponent() = default;
        CameraComponent(const CameraComponent&) = default;
    };


    struct NativeScriptComponent
    {
        ScriptableEntity* instance = nullptr;

        ScriptableEntity*(*instantiateScript)(); // creating a func pointer that returns a ScriptableEntity pointer to an object
        void (*destroyScript)(NativeScriptComponent*); 

        template<typename T>
        void bind()
        {
            instantiateScript = []() { return static_cast<ScriptableEntity*>(new T()); };
            destroyScript = [](NativeScriptComponent* nsc) { delete nsc->instance; nsc->instance = nullptr; };
        }
    };

}