#pragma once

#include "strvy.h"
#include "Panels/SceneHierarchyPanel.h"

namespace strvy {

	class EditorLayer : public strvy::Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;

		virtual void onAttach() override;
		virtual void onDetach() override;

		void onUpdate(strvy::Timestep ts) override;
		virtual void onImGuiRender(Timestep ts) override;
		void onEvent(strvy::Event& e) override;
	private:
		OrthographicCameraController m_cameraController;

		// Temp
		Ref<VertexArray> m_squareVA;
		Ref<Shader> m_flatColorShader;
		Ref<Framebuffer> m_framebuffer;

		Ref<Scene> m_activeScene;
		Entity m_squareEntity;
		Entity m_cameraEntity;
		Entity m_secondCamera;

		bool m_primaryCamera = true;

		bool m_viewportFocused = false, m_viewportHovered = false;
		glm::vec2 m_viewportSize = { 0.0f, 0.0f };

		Ref<Texture2D> m_checkerboardTexture;

		glm::vec4 m_squareColor = { 0.2f, 0.3f, 0.8f, 1.0f };

		// Panels
		SceneHierarchyPanel m_sceneHierarchyPanel;
	};
}