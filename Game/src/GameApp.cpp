#include "strvy.h"

#include <strvy/Core/EntryPoint.h>

#include "Platform/OpenGL/OpenGLShader.h"

#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Sandbox2D.h"



class ExampleLayer : public strvy::Layer
{
public: 
	ExampleLayer()
		: Layer("Example"), m_cameraController(1280.0f / 720.0f, true)
	{
		m_vertexArray = strvy::VertexArray::create();

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
			 0.5f, -0.5f, 0.0f,	1.0f, 0.0f, 1.0f, 1.0f,
			 0.0f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f
		};

		strvy::Ref<strvy::VertexBuffer> vertexBuffer = strvy::VertexBuffer::create(vertices, sizeof(vertices));

		// Describe
		strvy::BufferLayout layout = {
			{ strvy::ShaderDataType::Float3, "a_Position" },
			{ strvy::ShaderDataType::Float4, "a_Color" }
		};


		vertexBuffer->setLayout(layout);
		m_vertexArray->addVertexBuffer(vertexBuffer);

		uint32_t indices[3] = { 0, 1, 2 };
		strvy::Ref<strvy::IndexBuffer> indexBuffer = strvy::IndexBuffer::create(indices, sizeof(indices) / sizeof(uint32_t));
		m_vertexArray->setIndexBuffer(indexBuffer);


		m_squareVA = strvy::VertexArray::create();
		float squareVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};

		strvy::Ref<strvy::VertexBuffer> squareVB = strvy::VertexBuffer::create(squareVertices, sizeof(squareVertices));

		squareVB->setLayout({
			{ strvy::ShaderDataType::Float3, "a_Position" },
			{ strvy::ShaderDataType::Float2, "a_TexCoord" }
			});
		m_squareVA->addVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		strvy::Ref<strvy::IndexBuffer> squareIB = strvy::IndexBuffer::create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
		m_squareVA->setIndexBuffer(squareIB);

		std::string vertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}	
		)";

		std::string fragmentSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			in vec4 v_Color;

			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
				color = v_Color;
			}	
		)";

		m_shader = strvy::Shader::create("VertexPosColor", vertexSrc, fragmentSrc);

		std::string flatColorShaderVertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;

			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}	
		)";

		std::string flatColorShaderFragmentSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 color;

			uniform vec3 u_Color;

			in vec3 v_Position;

			void main()
			{
				color = vec4(u_Color, 1.0);
			}	
		)";

		m_flatColorShader = strvy::Shader::create("FlatColor", flatColorShaderVertexSrc, flatColorShaderFragmentSrc);


		auto textureShader = m_shaderLibrary.load("assets/shaders/Texture.glsl");

		m_texture = strvy::Texture2D::create("assets/textures/checkerboards.png");

		std::dynamic_pointer_cast<strvy::OpenGLShader>(textureShader)->bind();
		std::dynamic_pointer_cast<strvy::OpenGLShader>(textureShader)->uploadUniformInt("u_Texture", 0); // upload a texture with slot number 0
	}

	void onUpdate(strvy::Timestep ts) override
	{
		/*if (strvy::Input::isKeyPressed(SV_KEY_TAB))
			SV_TRACE("Tab key is pressed!");*/

		SV_TRACE("Delta time: {0}s ({1}ms)", ts.getSeconds(), ts.getMilliseconds());

		// Update
		m_cameraController.onUpdate(ts);

		// Render
		strvy::RenderCommand::setClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		strvy::RenderCommand::clear();

		strvy::Renderer::beginScene(m_cameraController.getCamera());

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));


		std::dynamic_pointer_cast<strvy::OpenGLShader>(m_flatColorShader)->bind();
		std::dynamic_pointer_cast<strvy::OpenGLShader>(m_flatColorShader)->uploadUniformFloat3("u_Color", m_squareColor);


		for (int y = 0; y < 20; ++y)
		{
			for (int x = 0; x < 20; ++x)
			{
				glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				strvy::Renderer::submit(m_flatColorShader, m_squareVA, transform);
			}
		}


		auto textureShader = m_shaderLibrary.get("Texture");

		m_texture->bind();
		strvy::Renderer::submit(textureShader, m_squareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		// Triangle 
		//strvy::Renderer::submit(m_shader, m_vertexArray, glm::mat4(1));

		strvy::Renderer::endScene();
	}

	virtual void onImGuiRender() override
	{
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Square color", glm::value_ptr(m_squareColor));
		ImGui::End();

	}

	void onEvent(strvy::Event& e) override
	{
		/*if (event.getEventType() == strvy::EventType::KeyPressed)
		{
			strvy::KeyPressedEvent& e = (strvy::KeyPressedEvent&)event;
			SV_TRACE("{0}", (char)e.getKeyCode());
		}*/

		m_cameraController.onEvent(e);
	}


private:
	strvy::ShaderLibrary m_shaderLibrary;

	strvy::Ref<strvy::Shader> m_shader;
	strvy::Ref<strvy::VertexArray> m_vertexArray;
			
	strvy::Ref<strvy::Shader> m_flatColorShader;
	strvy::Ref<strvy::VertexArray> m_squareVA;

	strvy::Ref<strvy::Texture2D> m_texture;

	strvy::OrthographicCameraController m_cameraController;

	glm::vec3 m_squareColor = { 0.2f, 0.3f, 0.8f };
};


class Game : public strvy::Application
{
public:
	Game()
	{
		//pushLayer(new ExampleLayer());
		pushLayer(new Sandbox2D());
	}

	~Game()
	{

	}
};

strvy::Application* strvy::createApplication()
{
	return new Game();
}