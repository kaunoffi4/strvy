#pragma once

#include "strvy/Renderer/UniformBuffer.h"

namespace strvy {

	class OpenGLUniformBuffer : public UniformBuffer
	{
	public:
		OpenGLUniformBuffer(uint32_t size, uint32_t binding);
		virtual ~OpenGLUniformBuffer();

		virtual void setData(const void* data, uint32_t size, uint32_t offset = 0) override;
	private:
		uint32_t m_rendererID = 0;
	};

}
