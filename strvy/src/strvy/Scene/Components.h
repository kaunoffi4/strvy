#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include "SceneCamera.h"
#include "ScriptableEntity.h"

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