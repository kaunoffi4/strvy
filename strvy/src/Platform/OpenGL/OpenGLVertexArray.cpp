#include "svpch.h"
#include "OpenGLVertexArray.h"

#include <glad/glad.h>

namespace strvy {

	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
			case strvy::ShaderDataType::Float:			return GL_FLOAT;
			case strvy::ShaderDataType::Float2:			return GL_FLOAT;
			case strvy::ShaderDataType::Float3:			return GL_FLOAT;
			case strvy::ShaderDataType::Float4:			return GL_FLOAT;
			case strvy::ShaderDataType::Mat3:			return GL_FLOAT;
			case strvy::ShaderDataType::Mat4:			return GL_FLOAT;
			case strvy::ShaderDataType::Int:			return GL_INT;
			case strvy::ShaderDataType::Int2:			return GL_INT;
			case strvy::ShaderDataType::Int3:			return GL_INT;
			case strvy::ShaderDataType::Int4:			return GL_INT;
			case strvy::ShaderDataType::Bool:			return GL_BOOL;
		}

		SV_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		SV_PROFILE_FUNCTION();

		glCreateVertexArrays(1, &m_rendererID);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		SV_PROFILE_FUNCTION();

		glDeleteVertexArrays(1, &m_rendererID);
	}

	void OpenGLVertexArray::bind() const
	{
		SV_PROFILE_FUNCTION();

		glBindVertexArray(m_rendererID);
	}

	void OpenGLVertexArray::unbind() const
	{
		SV_PROFILE_FUNCTION();

		glBindVertexArray(0);
	}

	void OpenGLVertexArray::addVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
	{
		SV_PROFILE_FUNCTION();

		SV_CORE_ASSERT(vertexBuffer->getLayout().getElements().size(), "VertexBuffer has no layout!");


		glBindVertexArray(m_rendererID);
		vertexBuffer->bind();

		uint32_t index = 0;
		const auto& layout = vertexBuffer->getLayout();
		for (const auto& element : layout)
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index,
				element.getComponentCount(),
				ShaderDataTypeToOpenGLBaseType(element.type),
				element.normalized ? GL_TRUE : GL_FALSE,
				layout.getStride(),
				(const void*)element.offset);
			index++;
		}

		m_vertexBuffers.push_back(vertexBuffer);
	}

	void OpenGLVertexArray::setIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
	{
		glBindVertexArray(m_rendererID);
		indexBuffer->bind();

		m_indexBuffer = indexBuffer;
	}
}