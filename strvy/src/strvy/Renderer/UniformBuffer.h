#pragma once

#include <strvy/Core/Core.h>

namespace strvy {

	class UniformBuffer
	{
	public:
		virtual ~UniformBuffer() {}
		virtual void setData(const void* data, uint32_t size, uint32_t offset = 0) = 0;

		static Ref<UniformBuffer> create(uint32_t size, uint32_t binding);
	};

}