#include "svpch.h"
#include "PrimitiveGeometry.h"

namespace strvy {

    Ref<VertexArray> PrimitiveCube::m_VAO;
    Ref<VertexBuffer> PrimitiveCube::m_VBO;
    uint32_t PrimitiveCube::m_count = 0;


	PrimitiveCube::PrimitiveCube()
	{
        m_count++;
	}

    PrimitiveCube::~PrimitiveCube()
    {
        m_count--;
        if (m_count == 0)
        {
            m_VAO.reset();
            m_VBO.reset();
        }
    }

    void PrimitiveCube::init()
    {
            float vertices[] = {
                // vertex coordinates // normals
                -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
                 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
                 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
                -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

                 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
                -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
                -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
                 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

                -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
                -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
                -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
                -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,

                 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
                 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
                 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
                 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

                -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
                 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
                 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
                -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,

                 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
                -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
                -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
                 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
            };

            m_VAO = VertexArray::create();
            m_VBO = VertexBuffer::create(vertices, sizeof(vertices));


            m_VBO->setLayout({
                { ShaderDataType::Float3, "a_Position" },
                { ShaderDataType::Float3, "a_Normal" }
                });

            m_VAO->addVertexBuffer(m_VBO);


            uint32_t* cubeIndices = new uint32_t[36];

            uint32_t offset = 0;
            for (uint32_t i = 0; i < 36; i += 6)
            {
                cubeIndices[i + 0] = offset + 0;
                cubeIndices[i + 1] = offset + 1;
                cubeIndices[i + 2] = offset + 2;

                cubeIndices[i + 3] = offset + 0;
                cubeIndices[i + 4] = offset + 3;
                cubeIndices[i + 5] = offset + 2;

                offset += 4;
            }

            Ref<IndexBuffer> cubeIB = IndexBuffer::create(cubeIndices, 36);
            m_VAO->setIndexBuffer(cubeIB);

            delete[] cubeIndices;
    }

}  