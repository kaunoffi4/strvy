#pragma once

#include <glm/glm.hpp>

namespace strvy {

	class Camera
	{
	public:
		Camera() = default;
		Camera(const glm::mat4& projection)
			: m_projection(projection) {}

		virtual ~Camera() = default;

		const glm::mat4 getProjection() const { return m_projection; }

	protected:
		glm::mat4 m_projection = glm::mat4(1.0f);
	};

}