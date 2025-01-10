#pragma once

#include "strvy/Renderer/OrthographicCamera.h"
#include "strvy/Core/Timestep.h"

#include "strvy/Events/ApplicationEvent.h"
#include "strvy/Events/MouseEvent.h"

namespace strvy {

	struct OrthographicCameraBounds
	{
		float left, right;
		float bottom, top;

		float getWidth() { return right - left; }
		float getHeight() { return top - bottom; }
	};

	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(float aspectRatio, bool rotation = false);

		void onUpdate(Timestep ts);
		void onEvent(Event& e);

		void onResize(float width, float height);

		OrthographicCamera& getCamera() { return m_camera; }
		const OrthographicCamera& getCamera() const { return m_camera; }

		float getZoomLevel() const { return m_zoomLevel; }
		void setZoomLevel(float level) { m_zoomLevel = level; }

		const OrthographicCameraBounds& getBounds() const { return m_bounds; }
	private:
		bool onMouseScrolled(MouseScrolledEvent& e);
		bool onWindowResized(WindowResizeEvent& e);
	private:
		float m_aspectRatio;
		float m_zoomLevel = 1.0f;
		OrthographicCameraBounds m_bounds;
		OrthographicCamera m_camera; // the order of the members matters

		bool m_rotation;
		float m_cameraRotation = 0.0f;
		glm::vec3 m_cameraPosition = { 0.0f, 0.0f, 0.0f };

		float m_cameraTranslationSpeed = 5.0f, m_cameraRotationSpeed = 150.0f;
	};
}