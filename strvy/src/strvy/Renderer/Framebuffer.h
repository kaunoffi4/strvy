#pragma once

#include "strvy/Core/Core.h"

namespace strvy {

	struct FramebufferSpecification
	{
		uint32_t width, height;
		// Framebuffer format (color buffer, depth buffer, shadow map, etc)
		uint32_t samples = 1;

		bool swap_chain_target = false;
	};

	class Framebuffer
	{
	public:
		virtual void bind() = 0;
		virtual void unbind() = 0;	

		virtual uint32_t getColorAttachmentRendererID() const = 0;

		virtual const FramebufferSpecification& getSpecification() const = 0;

		static Ref<Framebuffer> create(const FramebufferSpecification& spec);
	};
}