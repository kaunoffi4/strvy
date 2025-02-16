#include "EditorLayer.h"

#include "imgui/imgui.h"

#include "Platform/OpenGL/OpenGLShader.h"

#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp>

#include "strvy/Scene/SceneSerializer.h"

namespace strvy {


	EditorLayer::EditorLayer()
		: Layer("Sandbox2D"), m_cameraController(1280.0f / 720.0f, true)
	{
	}

	void EditorLayer::onAttach()
	{
		SV_PROFILE_FUNCTION();

		m_checkerboardTexture = Texture2D::create("assets/textures/checkerboards.png");

		FramebufferSpecification fb_spec;
		fb_spec.width = 1280;
		fb_spec.height = 720;
		m_framebuffer = Framebuffer::create(fb_spec);


		m_activeScene = createRef<Scene>();
		
#if 0
		// Entity
		auto square = m_activeScene->createEntity("Green Square");

		square.addComponent<SpriteRendererComponent>(glm::vec4{0.0f, 1.0f, 0.0f, 1.0f});

		m_squareEntity = square;

		m_cameraEntity = m_activeScene->createEntity("Camera A");
		m_cameraEntity.addComponent<CameraComponent>();

		m_secondCamera = m_activeScene->createEntity("Camera B");
		auto& cc = m_secondCamera.addComponent<CameraComponent>();
		cc.primary = false;
		//m_spriteSheet = Texture2D::create("assets/textures/sheet.png");
		//m_textureStairs = SubTexture2D::createFromCoords(m_spriteSheet, { 7, 6 }, { 128,128 });


		class CameraController : public ScriptableEntity
		{
		public:
			void onCreate() 
			{
				auto& translation = getComponent<TransformComponent>().translation;
				translation.x = rand() % 10 - 5.0f;
			}

			void onDestroy() 
			{
				std::cout << "onDestroy"<< std::endl;
			}

			void onUpdate(Timestep ts) 
			{
				auto& translation = getComponent<TransformComponent>().translation;
				float speed = 5.0f;

				if (Input::isKeyPressed(SV_KEY_A))
					translation.x -= speed * ts;
				if (Input::isKeyPressed(SV_KEY_D))
					translation.x += speed * ts;
				if (Input::isKeyPressed(SV_KEY_W))
					translation.y += speed * ts;
				if (Input::isKeyPressed(SV_KEY_S))
					translation.y -= speed * ts;
			}

		};

		m_secondCamera.addComponent<NativeScriptComponent>().bind<CameraController>();
		m_cameraEntity.addComponent<NativeScriptComponent>().bind<CameraController>();
#endif

		m_sceneHierarchyPanel.setContext(m_activeScene);
	}

	void EditorLayer::onDetach()
	{
		SV_PROFILE_FUNCTION();

	}

	void EditorLayer::onUpdate(Timestep ts)
	{
		SV_PROFILE_FUNCTION();

		// Resize
		if (FramebufferSpecification spec = m_framebuffer->getSpecification();
			m_viewportSize.x > 0.0f && m_viewportSize.y > 0.0f && // zero sized framebuffer is invalid
			(spec.width != m_viewportSize.x || spec.height != m_viewportSize.y))
		{
			m_framebuffer->resize((uint32_t)m_viewportSize.x, (uint32_t)m_viewportSize.y);
			m_cameraController.onResize(m_viewportSize.x, m_viewportSize.y);

			m_activeScene->onViewportResize((uint32_t)m_viewportSize.x, (uint32_t)m_viewportSize.y);
		}


		// Update
		if(m_viewportFocused)
			m_cameraController.onUpdate(ts);

		// Render

		Renderer2D::resetStats();
		m_framebuffer->bind();
		RenderCommand::setClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		RenderCommand::clear();

		
		// Update scene
		m_activeScene->onUpdate(ts);

		m_framebuffer->unbind();
	}

	void EditorLayer::onImGuiRender(Timestep ts)
	{
		SV_PROFILE_FUNCTION();

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
		if (opt_flags) // before: if (opt_flags & ImGuiDockNodeFlags_PassthruDockspace)
			window_flags |= ImGuiWindowFlags_NoBackground;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &dockspace_open, window_flags);
		ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// Dockspace
		ImGuiIO& io = ImGui::GetIO();
		ImGuiStyle& style = ImGui::GetStyle();
		float minWinSizeX = style.WindowMinSize.x;
		style.WindowMinSize.x = 370.0f;

		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockspace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), opt_flags);
		}
		style.WindowMinSize.x = minWinSizeX;


		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				// Disabling fullscreen would allow the window to be moved to the front of other windows, 
				// which we can't undo at the moment without finer window depth/z control.
				//ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);

				if (ImGui::MenuItem("Serialize"))
				{
					SceneSerializer serializer(m_activeScene);
					serializer.serialize("assets/scenes/Example.strvy");
				}

				if (ImGui::MenuItem("Deserialize"))
				{
					SceneSerializer serializer(m_activeScene);
					serializer.deserialize("assets/scenes/Example.strvy");
				}

				if (ImGui::MenuItem("Exit")) Application::get().close();
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		ImGui::End();

		m_sceneHierarchyPanel.onImGuiRender();

		ImGui::Begin("render stats");

		auto stats = Renderer2D::getStats();
		ImGui::Text("Renderer2D stats:");
		ImGui::Text("FPS: %.1f", (1000.0f / ts.getMilliseconds()));
		ImGui::Text("Draw calls: %d", stats.drawCalls);
		ImGui::Text("Quads: %d", stats.quadCount);
		ImGui::Text("Vertices: %d", stats.getTotalVertexCount());
		ImGui::Text("Indices: %d", stats.getTotalIndexCount());

		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Viewport");

		m_viewportFocused = ImGui::IsWindowFocused();
		m_viewportHovered = ImGui::IsWindowHovered();
		Application::get().getImGuiLayer()->blockEvents(!m_viewportFocused || !m_viewportHovered);

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail(); // check what this does
		m_viewportSize = { viewportPanelSize.x, viewportPanelSize.y };

		SV_WARN("Viewport Size: {0}, {1}", viewportPanelSize.x, viewportPanelSize.y);
		uint32_t textureID = m_framebuffer->getColorAttachmentRendererID();
		ImGui::Image(textureID, ImVec2{ m_viewportSize.x, m_viewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		ImGui::End();
		ImGui::PopStyleVar();

	}

	void EditorLayer::onEvent(Event& e)
	{
		m_cameraController.onEvent(e);
	}
}
