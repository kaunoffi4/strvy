#pragma once

#include "strvy/Renderer/Mesh.h"

namespace strvy {

	class OpenGLMesh : public Mesh
	{
	public:
		// mesh data
		std::vector<Vertex>		m_vertices;
		std::vector<Ref<Texture2D>>	m_textures;
		std::vector<uint32_t>	m_indices;


		OpenGLMesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices, std::vector<Ref<Texture2D>> textures);
		void draw(Ref<Shader> shader) override;
	private:
		// render data
		uint32_t VAO, VBO, EBO;

		void setupMesh();
	};

}