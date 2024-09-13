#include "svpch.h"
#include "strvy/Renderer/OrthographicCameraController.h"

#include "strvy/Core/Input.h"
#include "strvy/Core/KeyCodes.h"

namespace strvy {

	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation) // left, right, botton, top
		: m_aspectRatio(aspectRatio), m_camera(-m_aspectRatio * m_zoomLevel, m_aspectRatio* m_zoomLevel, -m_zoomLevel, m_zoomLevel), m_rotation(rotation)
	{

	}

	void OrthographicCameraController::onUpdate(Timestep ts)
	{
		SV_PROFILE_FUNCTION();


		if (Input::isKeyPressed(SV_KEY_W))
			m_cameraPosition.y += m_cameraTranslationSpeed * ts;
		else if (Input::isKeyPressed(SV_KEY_S))
			m_cameraPosition.y -= m_cameraTranslationSpeed * ts;

		if (Input::isKeyPressed(SV_KEY_A))
			m_cameraPosition.x -= m_cameraTranslationSpeed * ts;
		else if (Input::isKeyPressed(SV_KEY_D))
			m_cameraPosition.x += m_cameraTranslationSpeed * ts;

		if (m_rotation)
		{
			if (Input::isKeyPressed(SV_KEY_Q))
			{
				m_cameraRotation += m_cameraRotationSpeed * ts;
			}
			if (Input::isKeyPressed(SV_KEY_E))
			{ 
				m_cameraRotation -= m_cameraRotationSpeed * ts;
			}

			m_camera.setRotation(m_cameraRotation);
		}

		m_camera.setPosition(m_cameraPosition);
	}

	void OrthographicCameraController::onEvent(Event& e)
	{
		SV_PROFILE_FUNCTION();

		EventDispatcher dispatcher(e);
		dispatcher.dispatch<MouseScrolledEvent>(SV_BIND_EVENT_FN(OrthographicCameraController::onMouseScrolled));
		dispatcher.dispatch<WindowResizeEvent>(SV_BIND_EVENT_FN(OrthographicCameraController::onWindowResized));
	}

	bool OrthographicCameraController::onMouseScrolled(MouseScrolledEvent& e)
	{
		SV_PROFILE_FUNCTION();

		m_zoomLevel -= e.getYOffset() * 0.25f;
		m_zoomLevel = std::max(m_zoomLevel, 0.25f);
		m_camera.setProjection(-m_aspectRatio * m_zoomLevel, m_aspectRatio * m_zoomLevel, -m_zoomLevel, m_zoomLevel);

		return false;
	}

	bool OrthographicCameraController::onWindowResized(WindowResizeEvent& e)
	{
		SV_PROFILE_FUNCTION();

		m_aspectRatio = (float)e.getWidth() / (float)e.getHeight();
		m_camera.setProjection(-m_aspectRatio * m_zoomLevel, m_aspectRatio * m_zoomLevel, -m_zoomLevel, m_zoomLevel);

		return false;
	}
}