#include "svpch.h"
#include "Renderer2D.h"


#include "VertexArray.h"
#include "Shader.h"
#include "RenderCommand.h"

#include <glm/gtc/matrix_transform.hpp>
 


namespace strvy {

	struct quadVertex
	{
		glm::vec3 position;
		glm::vec4 color;
		glm::vec2 texCoord;
		// TODO: texID
	};

	struct Renderer2DData
	{
		const uint32_t maxQuads = 10000;
		const uint32_t maxVertices = maxQuads * 4;
		const uint32_t maxIndices = maxQuads * 6;

		Ref<VertexArray> quadVertexArray;
		Ref<VertexBuffer> quadVertexBuffer;
		Ref<Shader> textureShader;
		Ref<Texture2D> whiteTexture;

		uint32_t quadIndexCount = 0;
		quadVertex* quadVertexBufferBase = nullptr;
		quadVertex* quadVertexBufferPtr = nullptr;

	};

	static Renderer2DData s_data;

	void Renderer2D::init()
	{
		SV_PROFILE_FUNCTION();


		s_data.quadVertexArray = VertexArray::create();

		s_data.quadVertexBuffer = VertexBuffer::create(s_data.maxVertices * sizeof(quadVertex));

		s_data.quadVertexBuffer->setLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color" },
			{ ShaderDataType::Float2, "a_TexCoord" }
			});
		s_data.quadVertexArray->addVertexBuffer(s_data.quadVertexBuffer);

		s_data.quadVertexBufferBase = new quadVertex[s_data.maxVertices];

		uint32_t* quadIndices = new uint32_t[s_data.maxIndices];

		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_data.maxIndices; i += 6)
		{
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			offset += 4;
		}

		Ref<IndexBuffer> quadIB = IndexBuffer::create(quadIndices, s_data.maxIndices);
		s_data.quadVertexArray->setIndexBuffer(quadIB);

		delete[] quadIndices;


		s_data.whiteTexture = Texture2D::create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_data.whiteTexture->setData(&whiteTextureData, sizeof(uint32_t));

		s_data.textureShader = Shader::create("assets/shaders/Texture.glsl");
		s_data.textureShader->bind();
		s_data.textureShader->setInt("u_Texture", 0);
	}

	void Renderer2D::shutdown()
	{
		SV_PROFILE_FUNCTION();
	}

	void Renderer2D::beginScene(const OrthographicCamera& camera)
	{
		SV_PROFILE_FUNCTION();

		s_data.textureShader->bind();
		s_data.textureShader->setMat4("u_ViewProjection", camera.getVPMatrix());

		s_data.quadIndexCount = 0;
		s_data.quadVertexBufferPtr = s_data.quadVertexBufferBase;
	}

	void Renderer2D::endScene()
	{
		SV_PROFILE_FUNCTION();

		uint32_t dataSize = (uint8_t*)s_data.quadVertexBufferPtr - (uint8_t*)s_data.quadVertexBufferBase;
		s_data.quadVertexBuffer->setData(s_data.quadVertexBufferBase, dataSize);

		flush();
	}

	void Renderer2D::flush()
	{
		RenderCommand::drawIndexed(s_data.quadVertexArray, s_data.quadIndexCount); // < ----
	}

	void Renderer2D::drawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		drawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer2D::drawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		SV_PROFILE_FUNCTION();

		s_data.quadVertexBufferPtr->position = position;
		s_data.quadVertexBufferPtr->color = color;
		s_data.quadVertexBufferPtr->texCoord = { 0.0f, 0.0f };
		s_data.quadVertexBufferPtr++;

		s_data.quadVertexBufferPtr->position = { position.x + size.x, position.y, 0.0f };
		s_data.quadVertexBufferPtr->color = color;
		s_data.quadVertexBufferPtr->texCoord = { 0.0f, 0.0f };
		s_data.quadVertexBufferPtr++;

		s_data.quadVertexBufferPtr->position = { position.x + size.x, position.y + size.y, 0.0f };
		s_data.quadVertexBufferPtr->color = color;
		s_data.quadVertexBufferPtr->texCoord = { 0.0f, 0.0f };
		s_data.quadVertexBufferPtr++;

		s_data.quadVertexBufferPtr->position = { position.x, position.y + size.y, 0.0f };;
		s_data.quadVertexBufferPtr->color = color;
		s_data.quadVertexBufferPtr->texCoord = { 0.0f, 0.0f };
		s_data.quadVertexBufferPtr++;

		s_data.quadIndexCount += 6;

		/*s_data.textureShader->setFloat("u_tilingFactor", 1.0f);
		s_data.whiteTexture->bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_data.textureShader->setMat4("u_Transform", transform);

		s_data.quadVertexArray->bind();
		RenderCommand::drawIndexed(s_data.quadVertexArray);*/
	}
	void Renderer2D::drawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		drawQuad({ position.x, position.y, 0.0f }, size, texture, tilingFactor, tintColor);
	}
	void Renderer2D::drawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		SV_PROFILE_FUNCTION();

		s_data.textureShader->setFloat4("u_Color", tintColor);
		s_data.textureShader->setFloat("u_tilingFactor", tilingFactor);
		texture->bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_data.textureShader->setMat4("u_Transform", transform);

		s_data.quadVertexArray->bind();
		RenderCommand::drawIndexed(s_data.quadVertexArray);
	}

	void Renderer2D::drawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		drawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, color);
	}
	void Renderer2D::drawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		SV_PROFILE_FUNCTION();

		s_data.textureShader->setFloat4("u_Color", color);
		s_data.textureShader->setFloat("u_tilingFactor", 1.0f);
		s_data.whiteTexture->bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_data.textureShader->setMat4("u_Transform", transform);

		s_data.quadVertexArray->bind();
		RenderCommand::drawIndexed(s_data.quadVertexArray);
	}
	void Renderer2D::drawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		drawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, texture, tilingFactor, tintColor);
	}
	void Renderer2D::drawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		SV_PROFILE_FUNCTION();

		s_data.textureShader->setFloat4("u_Color", tintColor);
		s_data.textureShader->setFloat("u_tilingFactor", tilingFactor);
		texture->bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) 
			* glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_data.textureShader->setMat4("u_Transform", transform);

		s_data.quadVertexArray->bind();
		RenderCommand::drawIndexed(s_data.quadVertexArray);
	}
}