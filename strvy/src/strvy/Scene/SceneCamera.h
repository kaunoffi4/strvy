#pragma once

#include "strvy/Renderer/Camera.h"

namespace strvy {

	// this class will contain necessary data for ui for a camera, such as the type (ortho or perspective, what's its size or fov, etc.)
	// it's basicaly a representation what we will see in UI for a camera entity

	class SceneCamera : public Camera
	{
	public:
		enum class ProjectionType { Perspective = 0, Orthographic = 1 };
	public:
		SceneCamera();
		virtual ~SceneCamera() = default;

		void setPerspective(float verticalFOV, float nearClip, float farClip);
		void setOrthographic(float size, float nearClip, float farClip);

		void setViewportSize(uint32_t width, uint32_t height);

		float getPerspectiveVerticalFOV() const { return m_perspectiveFOV; }
		void setPerspectiveVerticalFOV(float verticalFOV) { m_perspectiveFOV = verticalFOV; recalculateProjection(); }
		float getPerspectiveNearClip() const { return m_perspectiveNear; }
		void setPerspectiveNearClip(float nearClip) { m_perspectiveNear = nearClip; recalculateProjection(); }
		float getPerspectiveFarClip() const { return m_perspectiveFar; }
		void setPerspectiveFarClip(float farClip) { m_perspectiveFar = farClip; recalculateProjection(); }

		float getOrthographicSize() const { return m_orthoSize; }
		void setOrthographicSize(float size) { m_orthoSize = size; recalculateProjection(); }
		float getOrthographicNearClip() const { return m_orthoNear; }
		void setOrthographicNearClip(float nearClip) { m_orthoNear = nearClip; recalculateProjection(); }
		float getOrthographicFarClip() const { return m_orthoFar; }
		void setOrthographicFarClip(float farClip) { m_orthoFar = farClip; recalculateProjection(); }

		ProjectionType getProjectionType() const { return m_projectionType; }
		void setProjectionType(ProjectionType type) { m_projectionType = type; recalculateProjection(); }
	private:
		void recalculateProjection();
	private:
		ProjectionType m_projectionType = ProjectionType::Orthographic;

		float m_perspectiveFOV = glm::radians(45.0f);
		float m_perspectiveNear = 0.01f, m_perspectiveFar = 1000.0f;
		
		
		float m_orthoSize = 10.0f;
		float m_orthoNear = -1.0f, m_orthoFar =	1.0f;


		float m_aspectRatio = 0.0f;
	};

}