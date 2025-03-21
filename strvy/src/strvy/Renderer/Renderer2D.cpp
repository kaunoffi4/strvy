#include "svpch.h"
#include "Renderer2D.h"


#include "VertexArray.h"
#include "Shader.h"
#include "UniformBuffer.h"
#include "RenderCommand.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
 


namespace strvy {

	struct quadVertex
	{
		glm::vec3 position;
		glm::vec4 color;
		glm::vec2 texCoord;
		float texIndex;
		float tilingFactor;

		// Editor only
		int entityID;
	};

	struct Renderer2DData
	{
		static const uint32_t maxQuads = 10000;
		static const uint32_t maxVertices = maxQuads * 4;
		static const uint32_t maxIndices = maxQuads * 6;
		static const uint32_t maxTextureSlots = 32; // TODO: RenderCaps

		Ref<VertexArray> quadVertexArray;
		Ref<VertexBuffer> quadVertexBuffer;
		Ref<Shader> textureShader;
		Ref<Texture2D> whiteTexture;

		uint32_t quadIndexCount = 0;
		quadVertex* quadVertexBufferBase = nullptr;
		quadVertex* quadVertexBufferPtr = nullptr;

		std::array<Ref<Texture2D>, maxTextureSlots> textureSlots;
		uint32_t textureSlotIndex = 1; // 0 = whiteTexture

		glm::vec4 quadVertexPositions[4];

		Renderer2D::Statistics stats;

		struct CameraData
		{
			glm::mat4 viewProjection;
		};
		CameraData cameraBuffer;
		Ref<UniformBuffer> cameraUniformBuffer;
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
			{ ShaderDataType::Float2, "a_TexCoord" },
			{ ShaderDataType::Float, "a_TexIndex" },
			{ ShaderDataType::Float, "a_TilingFactor" },
			{ ShaderDataType::Int, "a_EntityID" }
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

		s_data.textureSlots[0] = s_data.whiteTexture;

		s_data.quadVertexPositions[0] = { -0.5f, -0.5, 0.0f, 1.0f };
		s_data.quadVertexPositions[1] = {  0.5f, -0.5, 0.0f, 1.0f };
		s_data.quadVertexPositions[2] = {  0.5f,  0.5, 0.0f, 1.0f };
		s_data.quadVertexPositions[3] = { -0.5f,  0.5, 0.0f, 1.0f };

		s_data.cameraUniformBuffer = UniformBuffer::create(sizeof(Renderer2DData::CameraData), 0);
	}

	void Renderer2D::shutdown()
	{
		SV_PROFILE_FUNCTION();
		delete[] s_data.quadVertexBufferBase;
	}

	void Renderer2D::beginScene(const Camera& camera, const glm::mat4& transform) // In a case of success, change for all of the overrides
	{
		SV_PROFILE_FUNCTION();

		glm::mat4 viewProj = camera.getProjection() * glm::inverse(transform);

		s_data.cameraBuffer.viewProjection = viewProj;
		s_data.cameraUniformBuffer->setData(glm::value_ptr(viewProj), sizeof(glm::mat4));

		s_data.quadIndexCount = 0;
		s_data.quadVertexBufferPtr = s_data.quadVertexBufferBase;

		s_data.textureSlotIndex = 1;
	}

	void Renderer2D::beginScene(const EditorCamera& camera)
	{
		SV_PROFILE_FUNCTION();

		glm::mat4 viewProj = camera.getViewProjection();

		s_data.cameraBuffer.viewProjection = viewProj;
		s_data.cameraUniformBuffer->setData(glm::value_ptr(viewProj), sizeof(glm::mat4));

		s_data.quadIndexCount = 0;
		s_data.quadVertexBufferPtr = s_data.quadVertexBufferBase;

		s_data.textureSlotIndex = 1;
	}

	void Renderer2D::beginScene(const OrthographicCamera& camera)
	{
		SV_PROFILE_FUNCTION();

		//s_data.textureShader->bind();
		//s_data.textureShader->setMat4("u_ViewProjection", camera.getVPMatrix());

		s_data.quadIndexCount = 0;
		s_data.quadVertexBufferPtr = s_data.quadVertexBufferBase;

		s_data.textureSlotIndex = 1;
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
		for (uint32_t i = 0; i < s_data.textureSlotIndex; ++i)
		{
			s_data.textureSlots[i]->bind(i);
		}

		s_data.textureShader->bind();
		RenderCommand::drawIndexed(s_data.quadVertexArray, s_data.quadIndexCount);
		s_data.stats.drawCalls++;
	}

	void Renderer2D::flushAndReset()
	{
		endScene();

		s_data.quadIndexCount = 0;
		s_data.quadVertexBufferPtr = s_data.quadVertexBufferBase;

		s_data.textureSlotIndex = 1;
	}

	void Renderer2D::drawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		drawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer2D::drawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		SV_PROFILE_FUNCTION();

		if (s_data.quadIndexCount >= Renderer2DData::maxIndices)
			flushAndReset();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		drawQuad(transform, color);


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

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		drawQuad(transform, texture, tilingFactor);

#if OLD_PATH
		s_data.textureShader->setFloat4("u_Color", tintColor);
		s_data.textureShader->setFloat("u_tilingFactor", tilingFactor);
		texture->bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_data.textureShader->setMat4("u_Transform", transform);

		s_data.quadVertexArray->bind();
		RenderCommand::drawIndexed(s_data.quadVertexArray);
#endif
	}

	void Renderer2D::drawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<SubTexture2D>& subtexture, float tilingFactor, const glm::vec4& tintColor)
	{
		drawQuad({ position.x, position.y, 0.0f }, size, subtexture, tilingFactor, tintColor);
	}

	void Renderer2D::drawQuad(const glm::mat4& transform, const glm::vec4& color, int entityID)
	{
		SV_PROFILE_FUNCTION();

		if (s_data.quadIndexCount >= Renderer2DData::maxIndices)
			flushAndReset();

		constexpr size_t quadVertexCount = 4;
		constexpr glm::vec2 textureCoords[] = { {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f} };

		const float textureIndex = 0.0f; // white texture
		const float tilingFactor = 1.0f;

		for (size_t i = 0; i < quadVertexCount; ++i)
		{
			s_data.quadVertexBufferPtr->position = transform * s_data.quadVertexPositions[i];
			s_data.quadVertexBufferPtr->color = color;
			s_data.quadVertexBufferPtr->texCoord = textureCoords[i];
			s_data.quadVertexBufferPtr->texIndex = textureIndex;
			s_data.quadVertexBufferPtr->tilingFactor = tilingFactor;
			s_data.quadVertexBufferPtr->entityID = entityID;
			s_data.quadVertexBufferPtr++;
		}

		s_data.quadIndexCount += 6;
		s_data.stats.quadCount++;

	}

	void Renderer2D::drawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor, int entityID)
	{
		SV_PROFILE_FUNCTION();


		constexpr size_t quadVertexCount = 4;
		constexpr glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
		constexpr glm::vec2 textureCoords[] = { {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f} };

		if (s_data.quadIndexCount >= Renderer2DData::maxIndices)
			flushAndReset();

		float textureIndex = 0.0f;
		for (uint32_t i = 1; i < s_data.textureSlotIndex; ++i)
		{
			if (*s_data.textureSlots[i].get() == *texture.get()) // get() gives a pointer, then we wanna dereference this pointer to compare two texturesID
			{													 // we do this manipulations since if we just were to compare them, we would compare the shared_pointers, not the data they're referring to 
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			if (s_data.textureSlotIndex >= Renderer2DData::maxTextureSlots)
				flushAndReset();

			textureIndex = (float)s_data.textureSlotIndex;
			s_data.textureSlots[s_data.textureSlotIndex++] = texture;
		}

		for (size_t i = 0; i < quadVertexCount; ++i)
		{
			s_data.quadVertexBufferPtr->position = transform * s_data.quadVertexPositions[i];
			s_data.quadVertexBufferPtr->color = color;
			s_data.quadVertexBufferPtr->texCoord = textureCoords[i];
			s_data.quadVertexBufferPtr->texIndex = textureIndex;
			s_data.quadVertexBufferPtr->tilingFactor = tilingFactor;
			s_data.quadVertexBufferPtr->entityID = entityID;
			s_data.quadVertexBufferPtr++;
		}

		s_data.quadIndexCount += 6;
		s_data.stats.quadCount++;
	}

	void Renderer2D::drawSprite(const glm::mat4& transform, SpriteRendererComponent& src, int entityID)
	{
		drawQuad(transform, src.color, entityID);
	}

	void Renderer2D::drawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture2D>& subtexture, float tilingFactor, const glm::vec4& tintColor)
	{
		SV_PROFILE_FUNCTION();


		constexpr size_t quadVertexCount = 4;
		constexpr glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
		const glm::vec2* textureCoords = subtexture->getTexCoords();
		const Ref<Texture2D> texture = subtexture->getTexture();

		if (s_data.quadIndexCount >= Renderer2DData::maxIndices)
			flushAndReset();

		float textureIndex = 0.0f;
		for (uint32_t i = 1; i < s_data.textureSlotIndex; ++i)
		{
			if (*s_data.textureSlots[i].get() == *texture.get()) // get() gives a pointer, then we wanna dereference this pointer to compare two texturesID
			{													 // we do this manipulations since if we just were to compare them, we would compare the shared_pointers, not the data they're referring to 
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			if (s_data.textureSlotIndex >= Renderer2DData::maxTextureSlots)
				flushAndReset();

			textureIndex = (float)s_data.textureSlotIndex;
			s_data.textureSlots[s_data.textureSlotIndex++] = texture;
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		for (size_t i = 0; i < quadVertexCount; ++i)
		{
			s_data.quadVertexBufferPtr->position = transform * s_data.quadVertexPositions[i];
			s_data.quadVertexBufferPtr->color = color;
			s_data.quadVertexBufferPtr->texCoord = textureCoords[i];
			s_data.quadVertexBufferPtr->texIndex = textureIndex;
			s_data.quadVertexBufferPtr->tilingFactor = tilingFactor;
			s_data.quadVertexBufferPtr++;
		}

		s_data.quadIndexCount += 6;
		s_data.stats.quadCount++;
	}

	void Renderer2D::drawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		drawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, color);
	}
	void Renderer2D::drawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		SV_PROFILE_FUNCTION();

		if (s_data.quadIndexCount >= Renderer2DData::maxIndices)
			flushAndReset();

		const float texIndex = 0.0f; // white texture
		const float tilingFactor = 1.0f;

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		s_data.quadVertexBufferPtr->position = transform * s_data.quadVertexPositions[0];
		s_data.quadVertexBufferPtr->color = color;
		s_data.quadVertexBufferPtr->texCoord = { 0.0f, 0.0f };
		s_data.quadVertexBufferPtr->texIndex = texIndex;
		s_data.quadVertexBufferPtr->tilingFactor = tilingFactor;
		s_data.quadVertexBufferPtr++;

		s_data.quadVertexBufferPtr->position = transform * s_data.quadVertexPositions[1];
		s_data.quadVertexBufferPtr->color = color;
		s_data.quadVertexBufferPtr->texCoord = { 1.0f, 0.0f };
		s_data.quadVertexBufferPtr->texIndex = texIndex;
		s_data.quadVertexBufferPtr->tilingFactor = tilingFactor;
		s_data.quadVertexBufferPtr++;

		s_data.quadVertexBufferPtr->position = transform * s_data.quadVertexPositions[2];
		s_data.quadVertexBufferPtr->color = color;
		s_data.quadVertexBufferPtr->texCoord = { 1.0f, 1.0f };
		s_data.quadVertexBufferPtr->texIndex = texIndex;
		s_data.quadVertexBufferPtr->tilingFactor = tilingFactor;
		s_data.quadVertexBufferPtr++;

		s_data.quadVertexBufferPtr->position = transform * s_data.quadVertexPositions[3];
		s_data.quadVertexBufferPtr->color = color;
		s_data.quadVertexBufferPtr->texCoord = { 0.0f, 1.0f };
		s_data.quadVertexBufferPtr->texIndex = texIndex;
		s_data.quadVertexBufferPtr->tilingFactor = tilingFactor;
		s_data.quadVertexBufferPtr++;

		s_data.quadIndexCount += 6;
		s_data.stats.quadCount++;
	}
	void Renderer2D::drawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		drawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, texture, tilingFactor, tintColor);
	}
	void Renderer2D::drawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		SV_PROFILE_FUNCTION();

		if (s_data.quadIndexCount >= Renderer2DData::maxIndices)
			flushAndReset();

		constexpr glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

		float textureIndex = 0.0f;
		for (uint32_t i = 1; i < s_data.textureSlotIndex; ++i)
		{
			if (*s_data.textureSlots[i].get() == *texture.get()) // get() gives a pointer, then we wanna dereference this pointer to compare two texturesID
			{													 // we do this manipulations since if we just were to compare them, we would compare the shared_pointers, not the data they're referring to 
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			textureIndex = (float)s_data.textureSlotIndex;
			s_data.textureSlots[s_data.textureSlotIndex++] = texture;
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		s_data.quadVertexBufferPtr->position = transform * s_data.quadVertexPositions[0];
		s_data.quadVertexBufferPtr->color = color;
		s_data.quadVertexBufferPtr->texCoord = { 0.0f, 0.0f };
		s_data.quadVertexBufferPtr->texIndex = textureIndex;
		s_data.quadVertexBufferPtr->tilingFactor = tilingFactor;
		s_data.quadVertexBufferPtr++;

		s_data.quadVertexBufferPtr->position = transform * s_data.quadVertexPositions[1];
		s_data.quadVertexBufferPtr->color = color;
		s_data.quadVertexBufferPtr->texCoord = { 1.0f, 0.0f };
		s_data.quadVertexBufferPtr->texIndex = textureIndex;
		s_data.quadVertexBufferPtr->tilingFactor = tilingFactor;
		s_data.quadVertexBufferPtr++;

		s_data.quadVertexBufferPtr->position = transform * s_data.quadVertexPositions[2];
		s_data.quadVertexBufferPtr->color = color;
		s_data.quadVertexBufferPtr->texCoord = { 1.0f, 1.0f };
		s_data.quadVertexBufferPtr->texIndex = textureIndex;
		s_data.quadVertexBufferPtr->tilingFactor = tilingFactor;
		s_data.quadVertexBufferPtr++;

		s_data.quadVertexBufferPtr->position = transform * s_data.quadVertexPositions[3];
		s_data.quadVertexBufferPtr->color = color;
		s_data.quadVertexBufferPtr->texCoord = { 0.0f, 1.0f };
		s_data.quadVertexBufferPtr->texIndex = textureIndex;
		s_data.quadVertexBufferPtr->tilingFactor = tilingFactor;
		s_data.quadVertexBufferPtr++;

		s_data.quadIndexCount += 6;
		s_data.stats.quadCount++;
	}

	void Renderer2D::drawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<SubTexture2D>& subtexture, float tilingFactor, const glm::vec4& tintColor)
	{
		drawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, subtexture, tilingFactor, tintColor);
	}
	void Renderer2D::drawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<SubTexture2D>& subtexture, float tilingFactor, const glm::vec4& tintColor)
	{
		SV_PROFILE_FUNCTION();

		if (s_data.quadIndexCount >= Renderer2DData::maxIndices)
			flushAndReset();

		constexpr glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
		const glm::vec2* textureCoords = subtexture->getTexCoords();
		const Ref<Texture2D> texture = subtexture->getTexture();

		float textureIndex = 0.0f;
		for (uint32_t i = 1; i < s_data.textureSlotIndex; ++i)
		{
			if (*s_data.textureSlots[i].get() == *texture.get()) // get() gives a pointer, then we wanna dereference this pointer to compare two texturesID
			{													 // we do this manipulations since if we just were to compare them, we would compare the shared_pointers, not the data they're referring to 
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			textureIndex = (float)s_data.textureSlotIndex;
			s_data.textureSlots[s_data.textureSlotIndex++] = texture;
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		s_data.quadVertexBufferPtr->position = transform * s_data.quadVertexPositions[0];
		s_data.quadVertexBufferPtr->color = color;
		s_data.quadVertexBufferPtr->texCoord = { 0.0f, 0.0f };
		s_data.quadVertexBufferPtr->texIndex = textureIndex;
		s_data.quadVertexBufferPtr->tilingFactor = tilingFactor;
		s_data.quadVertexBufferPtr++;

		s_data.quadVertexBufferPtr->position = transform * s_data.quadVertexPositions[1];
		s_data.quadVertexBufferPtr->color = color;
		s_data.quadVertexBufferPtr->texCoord = { 1.0f, 0.0f };
		s_data.quadVertexBufferPtr->texIndex = textureIndex;
		s_data.quadVertexBufferPtr->tilingFactor = tilingFactor;
		s_data.quadVertexBufferPtr++;

		s_data.quadVertexBufferPtr->position = transform * s_data.quadVertexPositions[2];
		s_data.quadVertexBufferPtr->color = color;
		s_data.quadVertexBufferPtr->texCoord = { 1.0f, 1.0f };
		s_data.quadVertexBufferPtr->texIndex = textureIndex;
		s_data.quadVertexBufferPtr->tilingFactor = tilingFactor;
		s_data.quadVertexBufferPtr++;

		s_data.quadVertexBufferPtr->position = transform * s_data.quadVertexPositions[3];
		s_data.quadVertexBufferPtr->color = color;
		s_data.quadVertexBufferPtr->texCoord = { 0.0f, 1.0f };
		s_data.quadVertexBufferPtr->texIndex = textureIndex;
		s_data.quadVertexBufferPtr->tilingFactor = tilingFactor;
		s_data.quadVertexBufferPtr++;

		s_data.quadIndexCount += 6;
		s_data.stats.quadCount++;
	}

	void Renderer2D::resetStats()
	{
		memset(&s_data.stats, 0, sizeof(Statistics));
	}

	Renderer2D::Statistics Renderer2D::getStats()
	{
		return s_data.stats;
	}
}