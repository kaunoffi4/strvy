#pragma once

#include "strvy/Renderer/VertexArray.h"

namespace strvy {

	class PrimitiveGeometry
	{
	public:
		virtual ~PrimitiveGeometry() = default;

		virtual Ref<VertexArray> getVAO() const = 0;
		virtual uint32_t getCount() const = 0;
	};

	// for the initial state I get along with basic rendering 
	class PrimitiveCube : public PrimitiveGeometry
	{
	public:
		PrimitiveCube();

		virtual Ref<VertexArray> getVAO() const override { return m_VAO; }

		virtual uint32_t getCount() const override { return m_count; }
	private:
		Ref<VertexArray> m_VAO;
		Ref<VertexBuffer> m_VBO;
		uint32_t m_count;

	};

}