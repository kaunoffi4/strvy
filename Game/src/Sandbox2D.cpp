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

	strvy::FramebufferSpecification fb_spec;
	fb_spec.width = 1280;
	fb_spec.height = 720;
	m_framebuffer = strvy::Framebuffer::create(fb_spec);


	//m_spriteSheet = strvy::Texture2D::create("assets/textures/sheet.png");

	//m_textureStairs = strvy::SubTexture2D::createFromCoords(m_spriteSheet, { 7, 6 }, { 128,128 });

	m_particle.ColorBegin = { 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
	m_particle.ColorEnd = { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f };
	m_particle.SizeBegin = 0.5f, m_particle.SizeVariation = 0.3f, m_particle.SizeEnd = 0.0f;
	m_particle.LifeTime = 1.0f;
	m_particle.Velocity = { 0.0f, 0.0f };
	m_particle.VelocityVariation = { 3.0f, 1.0f };
	m_particle.Position = { 0.0f, 0.0f };
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

	strvy::Renderer2D::resetStats();
	m_framebuffer->bind();
	strvy::RenderCommand::setClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	strvy::RenderCommand::clear();

	{
		static float rotation = 0.0f;
		rotation += ts * 20.0f;

		SV_PROFILE_SCOPE("Renderer2D::drawQuads");

		strvy::Renderer2D::beginScene(m_cameraController.getCamera());
		strvy::Renderer2D::drawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
		strvy::Renderer2D::drawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, m_squareColor);
		strvy::Renderer2D::drawQuad({ 0.0f, 0.0f, -0.1f }, { 20.0f, 20.0f }, m_checkerboardTexture, 10.0f, glm::vec4(1.0f, 0.9f, 0.9f, 1.0f));
		strvy::Renderer2D::drawRotatedQuad({ -2.0f, 0.0f,  0.0f }, { 1.0f, 1.0f }, glm::radians(rotation), m_checkerboardTexture, 20.0f, glm::vec4(1.0f, 0.9f, 0.9f, 1.0f));
		strvy::Renderer2D::endScene();

		strvy::Renderer2D::beginScene(m_cameraController.getCamera());

		for (float y = -5.0f; y < 5.0f; y += 0.5f)
		{
			for (float x = -5.0f; x < 5.0f; x += 0.5f)
			{
				glm::vec4 color = { (y + 5.0f) / 10.0f, 0.4f, (x + 5.0f) / 10.0f, 1.0f };
				strvy::Renderer2D::drawQuad({ x, y }, { 0.45f, 0.45f }, color);
			}
		}

		strvy::Renderer2D::endScene();
		m_framebuffer->unbind();
	}

	// Particle
	if (strvy::Input::isMouseButtonPressed(SV_MOUSE_BUTTON_LEFT))
	{
		auto [x, y] = strvy::Input::getMousePosition();
		auto width = strvy::Application::get().getWindow().getWidth();
		auto height = strvy::Application::get().getWindow().getHeight();

		auto bounds = m_cameraController.getBounds();
		auto pos = m_cameraController.getCamera().getPosition();
		x = (x / width) * bounds.getWidth() - bounds.getWidth() * 0.5f;
		y = bounds.getHeight() * 0.5f - (y / height) * bounds.getHeight();
		m_particle.Position = { x + pos.x, y + pos.y };
		for (int i = 0; i < 5; i++)
			m_particleSystem.Emit(m_particle);
	}

	m_particleSystem.OnUpdate(ts);
	m_particleSystem.OnRender(m_cameraController.getCamera());
}

void Sandbox2D::onImGuiRender()
{
	SV_PROFILE_FUNCTION();
	
	static bool docking_enabled = true;
	if (docking_enabled)
	{

		static bool dockspace_open = true;
		static bool opt_fullscreen_persistant = true;
		static ImGuiDockNodeFlags opt_flags = ImGuiDockNodeFlags_None;
		bool opt_fullscreen = opt_fullscreen_persistant;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		// When using ImGuiDockNodeFlags_PassthruDockspace, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (opt_flags & ImGuiDockNodeFlags_PassthruDockspace)
			window_flags |= ImGuiWindowFlags_NoBackground;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &dockspace_open, window_flags);
		ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// Dockspace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockspace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), opt_flags);
		}

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				// Disabling fullscreen would allow the window to be moved to the front of other windows, 
				// which we can't undo at the moment without finer window depth/z control.
				//ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);

				if (ImGui::MenuItem("Exit")) strvy::Application::get().close();
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		ImGui::End();

		ImGui::Begin("Settings");

		auto stats = strvy::Renderer2D::getStats();
		ImGui::Text("Renderer2D stats:");
		ImGui::Text("Draw calls: %d", stats.drawCalls);
		ImGui::Text("Quads: %d", stats.quadCount);
		ImGui::Text("Vertices: %d", stats.getTotalVertexCount());
		ImGui::Text("Indices: %d", stats.getTotalIndexCount());

		ImGui::ColorEdit4("Square color", glm::value_ptr(m_squareColor));

		uint32_t textureID = m_framebuffer->getColorAttachmentRendererID();
		ImGui::Image((void*)textureID, ImVec2{ 320.0f, 180.0f });

		ImGui::End();
	}
	else
	{
	}
}

void Sandbox2D::onEvent(strvy::Event& e)
{
	m_cameraController.onEvent(e);
}
