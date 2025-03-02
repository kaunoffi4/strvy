#pragma once

#include "strvy/Core/Core.h"

namespace strvy {

	enum class FramebufferTextureFormat
	{
		None = 0,

		// Color
		RGBA8,
		RED_INTEGER,

		// Depth/stencil
		DEPTH24STENCIL8,

		// Defaults
		Depth = DEPTH24STENCIL8
	};

	struct FramebufferTextureSpecification
	{
		FramebufferTextureSpecification() = default;
		FramebufferTextureSpecification(FramebufferTextureFormat format)
			: textureFormat(format) {}

		FramebufferTextureFormat textureFormat = FramebufferTextureFormat::None;
		// TODO: filtering/map
	};

	struct FramebufferAttachmentSpecification
	{
		FramebufferAttachmentSpecification() = default;
		FramebufferAttachmentSpecification(std::initializer_list<FramebufferTextureSpecification> attachments)
			: attachments(attachments) {}

		std::vector<FramebufferTextureSpecification> attachments;
	};

	struct FramebufferSpecification
	{
		uint32_t width, height;
		FramebufferAttachmentSpecification attachments;
		// Framebuffer format (color buffer, depth buffer, shadow map, etc)
		uint32_t samples = 1;

		bool swap_chain_target = false;
	};

	class Framebuffer
	{
	public:
		virtual ~Framebuffer() = default;
		virtual void bind() = 0;
		virtual void unbind() = 0;	

		virtual void resize(uint32_t width, uint32_t height) = 0;
		virtual int readPixel(uint32_t attachmentIndex, int x, int y) = 0;

		virtual void clearAttachment(uint32_t attachmentIndex, int value) = 0;

		virtual uint32_t getColorAttachmentRendererID(uint32_t index = 0) const = 0;

		virtual const FramebufferSpecification& getSpecification() const = 0;

		static Ref<Framebuffer> create(const FramebufferSpecification& spec);
	};
}