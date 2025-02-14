#include "svpch.h";
#include "SceneCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace strvy {

	SceneCamera::SceneCamera()
	{
		recalculateProjection();
	}

	void SceneCamera::setPerspective(float verticalFOV, float nearClip, float farClip)
	{
		m_projectionType = ProjectionType::Perspective;
		m_perspectiveFOV = verticalFOV;
		m_perspectiveNear = nearClip;
		m_perspectiveFar = farClip;

		recalculateProjection();
	}

	void SceneCamera::setOrthographic(float size, float nearClip, float farClip)
	{
		m_projectionType = ProjectionType::Orthographic;
		m_orthoSize = size;
		m_orthoNear = nearClip;
		m_orthoFar = farClip;

		recalculateProjection();
	}

	void SceneCamera::setViewportSize(uint32_t width, uint32_t height)
	{
		m_aspectRatio = (float)width / (float)height;

		recalculateProjection();
	}

	void SceneCamera::recalculateProjection()
	{
		if (m_projectionType == ProjectionType::Perspective)
		{
			m_projection = glm::perspective(m_perspectiveFOV, m_aspectRatio, m_perspectiveNear, m_perspectiveFar);
		}
		else
		{
			float orthoLeft = -m_orthoSize * m_aspectRatio * 0.5f;
			float orthoRight = m_orthoSize * m_aspectRatio * 0.5f;
			float orthoBottom = -m_orthoSize * 0.5f;
			float orthoTop = m_orthoSize * 0.5f;

			m_projection = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop, m_orthoNear, m_orthoFar);
		}
	}

}