#pragma once

#include "strvy/Renderer/Shader.h"
#include "strvy/Renderer/Texture.h"

#include <glm/glm.hpp>

namespace strvy {

	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 texCoords;
	};

	class Mesh
	{
	public:

		virtual ~Mesh() = default;

		virtual void draw(Ref<Shader> shader) = 0;
		static Ref<Mesh> create(std::vector<Vertex> vertices, std::vector<uint32_t> indices, std::vector<Ref<Texture2D>> textures);
	};

}