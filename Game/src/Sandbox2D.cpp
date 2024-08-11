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
	m_checkerboardTexture = strvy::Texture2D::create("assets/textures/checkerboards.png");
}

void Sandbox2D::onDetach()
{
}

void Sandbox2D::onUpdate(strvy::Timestep ts)
{
	// Update
	m_cameraController.onUpdate(ts);

	// Render
	strvy::RenderCommand::setClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	strvy::RenderCommand::clear();

	strvy::Renderer2D::beginScene(m_cameraController.getCamera());

	strvy::Renderer2D::drawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
	strvy::Renderer2D::drawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f });
	strvy::Renderer2D::drawQuad({ 0.0f, 0.0f, -0.1f }, { 10.0f, 10.0f }, m_checkerboardTexture);

	strvy::Renderer2D::endScene();
}

void Sandbox2D::onImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square color", glm::value_ptr(m_squareColor));
	ImGui::End();
}

void Sandbox2D::onEvent(strvy::Event& e)
{
	m_cameraController.onEvent(e);
}
