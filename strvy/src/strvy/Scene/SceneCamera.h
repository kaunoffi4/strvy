#pragma once

#include "strvy/Renderer/Camera.h"

namespace strvy {

	// this class will contain necessary data for ui for a camera, such as the type (ortho or perspective, what's its size or fov, etc.)
	// it's basicaly a representation what we will see in UI for a camera entity

	class SceneCamera : public Camera
	{
	public:
		SceneCamera();
		virtual ~SceneCamera() = default;

		void setOrthographic(float size, float nearClip, float farClip);
		void setViewportSize(uint32_t width, uint32_t height);

		float getOrthographicSize() const { return m_orthoSize; }
		void setOrthographicSize(float size) { m_orthoSize = size; recalculateProjection(); }

	private:
		void recalculateProjection();
	private:
		float m_orthoSize = 10.0f;
		float m_orthoNear = -1.0f, m_orthoFar =	1.0f;

		float m_aspectRatio = 0.0f;
	};

}