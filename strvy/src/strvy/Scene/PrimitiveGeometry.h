#pragma once

#include "strvy/Renderer/VertexArray.h"

namespace strvy {

	class PrimitiveGeometry
	{
	public:
		virtual ~PrimitiveGeometry() = default;

		virtual Ref<VertexArray> getVAO() const = 0;
	};


	// for the initial state I get along with basic rendering 
	class PrimitiveCube : public PrimitiveGeometry
	{
	public:
		PrimitiveCube();
		~PrimitiveCube();

		void init();

		virtual Ref<VertexArray> getVAO() const override { return PrimitiveCube::m_VAO; }

	private:
		static Ref<VertexArray>	 m_VAO; // I'm gonna use the same VAO and VBO for all cube instances
		static Ref<VertexBuffer> m_VBO;

		static uint32_t m_count;
	};

}