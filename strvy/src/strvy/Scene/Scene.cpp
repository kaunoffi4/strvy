#include "svpch.h"
#include "Scene.h"

#include "Components.h"
#include "strvy/Renderer/Renderer2D.h"
#include "strvy/Renderer/Renderer3D.h"

#include <glm/glm.hpp>

#include "Entity.h"

namespace strvy {

    Scene::Scene()
    {
        
    }

    Scene::~Scene()
    {

    }

    Entity Scene::createEntity(const std::string& name)
    {
        Entity entity = { m_registry.create(), this };
        entity.addComponent<TransformComponent>();
        auto& tag = entity.addComponent<TagComponent>();
        tag.tag = name.empty() ? "Entity" : name;

        return entity;
    }

    void Scene::destroyEntity(Entity entity)
    {
        m_registry.destroy(entity);
    }

    void Scene::onUpdateRuntime(Timestep ts)
    {
        
        // Update scripts
        {
            m_registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc)
                {
                    // TODO: move to Scene::onScenePlay
                    if (!nsc.instance)
                    {
                        nsc.instance = nsc.instantiateScript();
                        nsc.instance->m_entity = Entity{ entity, this };
                        nsc.instance->onCreate();
                    }

                    nsc.instance->onUpdate(ts);
                });
        }

        // Render 2D 
        Camera* mainCamera = nullptr;
        glm::mat4 cameraTransform;
        {
            auto view = m_registry.view<CameraComponent, TransformComponent>();
            for (auto entity : view)
            {
                auto [camera, transform] = view.get<CameraComponent, TransformComponent>(entity);

                if (camera.primary)
                {
                    mainCamera = &camera.camera;
                    cameraTransform = transform.getTransform();
                    break;
                }
            }
        }

        if (mainCamera)
        {
            Renderer2D::beginScene(*mainCamera, cameraTransform);

            auto group = m_registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);

            for(auto entity : group)
            {
                auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

                Renderer2D::drawSprite(transform.getTransform(), sprite, (int)entity);
            }

            Renderer2D::endScene();
        }
    }

    void Scene::onUpdateEditor(Timestep ts, EditorCamera& camera)
    {
        Renderer2D::beginScene(camera);

        auto group = m_registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);

        for (auto entity : group)
        {
            auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

            Renderer2D::drawSprite(transform.getTransform(), sprite, (int)entity);
        }

        Renderer2D::endScene();
    }

    void Scene::onUpdateEditor3D(Timestep ts, EditorCamera& camera, const LightBlock& lightBlock)
    {
        Renderer3D::beginScene(camera, lightBlock);

        // draw 3D objects (models)

        // test code snippet
        glm::mat4 modelCube1 = glm::mat4(1.0f);
        //Renderer3D::drawCall(modelCube1, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), 1); // change this part (drawCall function) to pass an entity ID as the third argument

        glm::mat4 modelLamp = glm::translate(glm::mat4(1.0f), lightBlock.lights[0].position);
        modelLamp = glm::inverse(modelLamp);
        modelLamp = glm::scale(modelLamp, glm::vec3(0.2f));
        //Renderer3D::drawCall(modelLamp, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), 2);




        // draw 3D primitive geometries (cubes, spheres, etc.)
        auto group = m_registry.group<TransformComponent>(entt::get<PrimitiveComponent>);
        
        struct InstanceData
        {
            glm::mat4 modelMatrix;
            glm::vec4 color;
            int entityID;
        };

        uint32_t count = group.size();
        std::vector<InstanceData> instanceData(count);

        uint32_t i = 0;

        for (auto entity : group)
        {
            auto [transform, primitive] = group.get<TransformComponent, PrimitiveComponent>(entity);

            if (i < count) // TODO: add check for different geometries (cube, sphere, etc.)
            {
                instanceData[i].modelMatrix = transform.getTransform();
                instanceData[i].color = primitive.color;
                instanceData[i].entityID = (int)entity;
                ++i;
            }

        }

        Renderer3D::drawPrimitive(count ? instanceData.data() : nullptr, sizeof(InstanceData)* count, count);

        Renderer3D::endScene();
    }

    void Scene::onViewportResize(uint32_t width, uint32_t height)
    {
        m_viewportWidth = width;
        m_viewportHeight = height;

        // Resize our non-fixedAspectRatio cameras

        auto& view = m_registry.view<CameraComponent>();
        for (auto entity : view)
        {
            auto& cameraComponent = view.get<CameraComponent>(entity);
            if (!cameraComponent.fixedAspectRatio)
            {
                cameraComponent.camera.setViewportSize(width, height);
            }
        }

    }

    Entity Scene::getPrimaryCameraEntity()
    {
        auto view = m_registry.view<CameraComponent>();

        for (auto entity : view)
        {
            const auto& camera = view.get<CameraComponent>(entity);
            if (camera.primary)
                return Entity{ entity, this };
        }
        return {};
    }

    template<typename T>
    void Scene::onComponentAdded(Entity entity, T& component) // this definition is for template types, whose implementation doesn't exist yet
    {
        static_assert(false); 
    }

    template<>
    void Scene::onComponentAdded<PrimitiveComponent>(Entity entity, PrimitiveComponent& component)
    {
        // create a primitive geometry
    }

    template<>
    void Scene::onComponentAdded<TransformComponent>(Entity entity, TransformComponent& component)
    {

    }

    template<>
    void Scene::onComponentAdded<CameraComponent>(Entity entity, CameraComponent& component)
    {
        component.camera.setViewportSize(m_viewportWidth, m_viewportHeight);
    }

    template<>
    void Scene::onComponentAdded<SpriteRendererComponent>(Entity entity, SpriteRendererComponent& component)
    {

    }

    template<>
    void Scene::onComponentAdded<TagComponent>(Entity entity, TagComponent& component)
    {

    }

    template<>
    void Scene::onComponentAdded<NativeScriptComponent>(Entity entity, NativeScriptComponent& component)
    {

    }
}