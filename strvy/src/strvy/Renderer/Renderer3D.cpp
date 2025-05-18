#include "svpch.h"
#include "Renderer3D.h"
#include "RenderCommand.h"


#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"
#include "UniformBuffer.h"

#include "UBOSpecifications.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


namespace strvy {

	struct Renderer3DData
	{
		static const uint32_t maxTextureSlots = 32;

		Ref<VertexArray> cubeVAO;
		Ref<VertexBuffer> cubeVBO; // can be used for both default and lighting cubes
		Ref<Shader> lightingShader;
		Ref<Texture2D> whiteTexture;

		Ref<VertexArray> lampVAO;
        Ref<Shader> lightCubeShader;

		std::array<Ref<Texture2D>, maxTextureSlots> textureSlots;
		uint32_t textureSlotIndex = 1; // 0 = whiteTexture

        // camera UBO
		struct alignas(16) CameraData
		{
			glm::mat4 viewProjection;
            glm::vec3 viewPos;
            int _pad0;
		};
		CameraData cameraBuffer;
		Ref<UniformBuffer> cameraUBO;

        // lighting UBO
        LightBlock lightBlock;
        Ref<UniformBuffer> lightUBO;


        // material UBO
        Material material;
        Ref<UniformBuffer> materialUBO;

        // temporary
        struct Instance
        {
            glm::mat4 transform;

            int entityID;
            glm::vec3 _pad0;

            glm::vec4 color;
        };

        Instance instance;
        Ref<UniformBuffer> instanceUBO; // for other not too specialized data, that cannot form a block (a single vec3 or mat4 variable, etc.)
	};

	static Renderer3DData s_data;

	void Renderer3D::init()
	{

        float vertices[] = {
            // vertex coordinates // normals
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
             0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
             0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

             0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
             0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
             0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
             0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
             0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
             0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,

            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        };

        s_data.cubeVAO = VertexArray::create();
        //s_data.lampVAO = VertexArray::create();

        s_data.cubeVBO = VertexBuffer::create(vertices, sizeof(vertices));

        s_data.cubeVBO->setLayout({
            { ShaderDataType::Float3, "a_Position" },
            { ShaderDataType::Float3, "a_Normal" }
            });

        // assign the cube's VBO for both VAO's (the first is a default cube, second - light source's cube)
        s_data.cubeVAO->addVertexBuffer(s_data.cubeVBO);
        //s_data.lampVAO->addVertexBuffer(s_data.cubeVBO);

        // initialize IBO 
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
        s_data.cubeVAO->setIndexBuffer(cubeIB);

        delete[] cubeIndices;
        //s_data.lampVAO->setIndexBuffer(cubeIB);

        s_data.lightingShader = Shader::create("assets/shaders/lightingShader.glsl");

        //s_data.lightCubeShader = Shader::create("assets/shaders/lightCubeShader.glsl");

        //s_data.textureSlots[0] = s_data.whiteTexture;
        int maxUBOSize;
        RenderCommand::getMaxUBOSize(maxUBOSize);
        std::cout << "MAX_UBO_SIZE: " << maxUBOSize << '\n';


        // initializing UBOs 
        s_data.cameraUBO = UniformBuffer::create(sizeof(Renderer3DData::CameraData), 0);
        s_data.lightUBO = UniformBuffer::create(sizeof(Renderer3DData::lightBlock), 1);
        s_data.materialUBO = UniformBuffer::create(sizeof(Renderer3DData::material), 2);
        s_data.instanceUBO = UniformBuffer::create(sizeof(Renderer3DData::instance), 3);

	}

	void Renderer3D::shutdown()
	{
	}

	void Renderer3D::beginScene(const EditorCamera& camera, const LightBlock& lightBlock)
	{
        s_data.lightingShader->bind();
        // camera UBO upload
        glm::mat4 viewProj = camera.getViewProjection();
        s_data.cameraBuffer.viewProjection = viewProj;

        glm::vec3 viewPos = camera.getPosition();
        s_data.cameraBuffer.viewPos = viewPos;

        //s_data.cameraUBO->setData((const void*)&s_data.cameraBuffer, sizeof(s_data.cameraBuffer));
        s_data.cameraUBO->setData((const void*)&s_data.cameraBuffer, sizeof(s_data.cameraBuffer));

        // lighting UBO upload
        //glm::vec3 lightDir(-0.2f, -1.0f, -0.3f);

        for (int i = 0; i < lightBlock.lightCount; ++i)
        {
            s_data.lightBlock.lights[i].position = lightBlock.lights[i].position;

            s_data.lightBlock.lights[i].ambient = lightBlock.lights[i].ambient;
            s_data.lightBlock.lights[i].diffuse = lightBlock.lights[i].diffuse;
            s_data.lightBlock.lights[i].specular = lightBlock.lights[i].specular;
        }
        s_data.lightBlock.lightCount = lightBlock.lightCount;

        s_data.lightUBO->setData((const void*)&s_data.lightBlock, sizeof(s_data.lightBlock));

        //// material upload (later transform to UBO, not separate uniforms)

        s_data.material.shininess = 32.0f;
        s_data.material.ambient = glm::vec3(1.0f, 0.5f, 0.31f);
        s_data.material.diffuse = glm::vec3(1.0f, 0.5f, 0.31f);
        s_data.material.specular = glm::vec3(0.5f, 0.5f, 0.5f);

        s_data.materialUBO->setData((const void*)&s_data.material, sizeof(s_data.material));
        // other uniform uploads


        // TODO: render a cube using lighting shader, a lamp (another cube) using lightCubeShader.glsl
        // perform a drawcall per object, instead of per batch, since it's now 3D rendering 

        // my first goal, given the current state, is to render a container (cube) using lightingShader and perfrom all the light calculations, based on light ray outcoming
        // from a light source (lamp cube). Then switch do lightCubeShader.glsl and draw that cube using white color.
        
	}

    void Renderer3D::drawCall(const glm::mat4& transform, const glm::vec4& color, int entityID) 
    {
        // TODO: find a way to switch between VAOs
        // !!! probably the solution is in the Renderer class, submit() method

        
        s_data.instance.transform = transform;
        s_data.instance.entityID = entityID;
        s_data.instance.color = color;

        s_data.lightingShader->bind();
        s_data.instanceUBO->setData((const void*)&s_data.instance, sizeof(Renderer3DData::instance));


        //s_data.materialUBO->setData((const void*)&s_data.material, sizeof(Renderer3DData::material));

        s_data.cubeVAO->bind();
        RenderCommand::drawIndexed(s_data.cubeVAO);
    }

    void Renderer3D::drawPrimitive(const glm::mat4& transform, const PrimitiveComponent& pc, int entityID)
    {
        s_data.instance.transform = transform;
        s_data.instance.entityID = entityID;
        s_data.instance.color = pc.color;

        s_data.lightingShader->bind();
        s_data.instanceUBO->setData((const void*)&s_data.instance, sizeof(s_data.instance));

        //s_data.materialUBO->setData((const void*)&s_data.material, sizeof(Renderer3DData::material));

        pc.geometry->getVAO()->bind();
        RenderCommand::drawIndexed(pc.geometry->getVAO(), 36);
    }

    void Renderer3D::endScene()
    {

    }



}