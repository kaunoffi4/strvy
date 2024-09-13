#include "svpch.h"
#include "OrthographicCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace strvy {

	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
		: m_projectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)), m_viewMatrix(1.0f)
	{
		SV_PROFILE_FUNCTION();

		m_VPMatrix = m_projectionMatrix * m_viewMatrix;
	}

	void OrthographicCamera::setProjection(float left, float right, float bottom, float top)
	{
		SV_PROFILE_FUNCTION();

		m_projectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
		m_VPMatrix = m_projectionMatrix * m_viewMatrix;
	}

	void OrthographicCamera::recalculateViewMatrix()
	{
		SV_PROFILE_FUNCTION();

		glm::mat4 transform = glm::rotate(glm::mat4(1.0f), glm::radians(m_rotation), glm::vec3(0, 0, 1)) 
			* glm::translate(glm::mat4(1.0f), m_position);													// camera's local space

		
		m_viewMatrix = glm::inverse(transform);
		m_VPMatrix = m_projectionMatrix * m_viewMatrix;
	}
}