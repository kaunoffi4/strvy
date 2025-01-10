#pragma once

#include <glm/glm.hpp>

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
        glm::mat4 transform{ 1.0f };

        TransformComponent() = default;
        TransformComponent(const TransformComponent&) = default;
        TransformComponent(const glm::mat4& transform)
            : transform(transform) {}

        operator const glm::mat4& () const { return transform; }
        operator glm::mat4& () { return transform; }
    };

    struct SpriteRendererComponent
    {
        glm::vec4 color{ 1.0f, 1.0f, 1.0f, 1.0f };

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