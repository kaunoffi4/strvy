#pragma once


#include "strvy/Renderer/Framebuffer.h"

namespace strvy {
	
	class OpenGLFramebuffer : public Framebuffer
	{
	public:
		OpenGLFramebuffer(const FramebufferSpecification& spec);
		virtual ~OpenGLFramebuffer();

		void invalidate();

		virtual void bind() override;
		virtual void unbind() override;

		virtual void resize(uint32_t width, uint32_t height) override;
		virtual int readPixel(uint32_t attachmentIndex, int x, int y) override;

		virtual uint32_t getColorAttachmentRendererID(uint32_t index = 0) const override 
		{ 
			SV_CORE_ASSERT(index < m_colorAttachments.size(), "Exceeding the size of a container");
			return m_colorAttachments[index]; 
		}

		virtual const FramebufferSpecification& getSpecification() const override { return m_specification; }
	private:
		uint32_t m_rendererID = 0;
		FramebufferSpecification m_specification;

		std::vector<FramebufferTextureSpecification> m_colorAttachmentSpecifications;
		FramebufferTextureSpecification m_depthAttachmentSpecification = FramebufferTextureFormat::None;

		std::vector<uint32_t> m_colorAttachments;
		uint32_t m_depthAttachment = 0;
	};
}