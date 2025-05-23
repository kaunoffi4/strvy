#pragma once


#include "strvy/Renderer/Buffer.h"

namespace strvy {

	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(uint32_t size = 0);
		OpenGLVertexBuffer(float* vertices, uint32_t size);
		virtual ~OpenGLVertexBuffer();

		virtual void bind() const override;
		virtual void unbind() const override;

		virtual void invalidate() override;
		virtual void setData(const void* data, uint32_t size) override;

		virtual const BufferLayout& getLayout() const override { return m_layout; }
		virtual void setLayout(const BufferLayout& layout) override { m_layout = layout; };

	private:
		uint32_t m_rendererID;
		uint32_t m_capacity;
		BufferLayout m_layout;
	};

	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
		virtual ~OpenGLIndexBuffer();

		virtual void bind() const override;
		virtual void unbind() const override;

		virtual uint32_t getCount() const { return m_count; }

	private:
		uint32_t m_rendererID;
		uint32_t m_count;
	};

}