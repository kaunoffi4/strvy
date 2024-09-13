#include "Sandbox2D.h"

#include "imgui/imgui.h"

#include "Platform/OpenGL/OpenGLShader.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_cameraController(1280.0f / 720.0f, true)
{
}

void Sandbox2D::onAttach()
{
	SV_PROFILE_FUNCTION();

	m_checkerboardTexture = strvy::Texture2D::create("assets/textures/checkerboards.png");
}

void Sandbox2D::onDetach()
{
	SV_PROFILE_FUNCTION();

}

void Sandbox2D::onUpdate(strvy::Timestep ts)
{
	SV_PROFILE_FUNCTION();

	// Update
	m_cameraController.onUpdate(ts);

	// Render
	strvy::RenderCommand::setClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	strvy::RenderCommand::clear();

	{
		static float rotation = 0.0f;
		rotation += ts * 20.0f;

		SV_PROFILE_SCOPE("Renderer2D::drawQuads");

		strvy::Renderer2D::beginScene(m_cameraController.getCamera());
		strvy::Renderer2D::drawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
		strvy::Renderer2D::drawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, m_squareColor);
		strvy::Renderer2D::drawQuad({ 0.0f, 0.0f, -0.1f }, { 10.0f, 10.0f }, m_checkerboardTexture, 10.0f, glm::vec4(1.0f, 0.9f, 0.9f, 1.0f));
		strvy::Renderer2D::drawRotatedQuad({ -2.0f, 0.0f,  0.0f }, { 1.0f, 1.0f }, rotation, m_checkerboardTexture, 20.0f, glm::vec4(1.0f, 0.9f, 0.9f, 1.0f));
		strvy::Renderer2D::endScene();
	}
}

void Sandbox2D::onImGuiRender()
{
	SV_PROFILE_FUNCTION();

	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square color", glm::value_ptr(m_squareColor));

	/*for (auto& result : m_profileResults)
	{
		char label[50];
		strcpy(label, "  %.3fms");
		strcat(label, result.name);
		ImGui::Text(label, result.time);
	}
	m_profileResults.clear();*/

	ImGui::End();
}

void Sandbox2D::onEvent(strvy::Event& e)
{
	m_cameraController.onEvent(e);
}
