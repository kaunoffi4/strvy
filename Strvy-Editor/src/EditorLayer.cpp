#include "EditorLayer.h"

#include "imgui/imgui.h"

#include "Platform/OpenGL/OpenGLShader.h"

#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp>

#include "strvy/Scene/SceneSerializer.h"

#include "strvy/Utils/PlatformUtils.h"

#include "ImGuizmo.h"

#include "strvy/Math/Math.h"

namespace strvy {


	EditorLayer::EditorLayer()
		: Layer("Sandbox2D"), m_cameraController(1280.0f / 720.0f, true)
	{
	}

	void EditorLayer::onAttach()
	{
		SV_PROFILE_FUNCTION();

		m_checkerboardTexture = Texture2D::create("assets/textures/checkerboards.png");

		FramebufferSpecification fbSpec;
		fbSpec.attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::Depth };
		fbSpec.width = 1280;
		fbSpec.height = 720;
		m_framebuffer = Framebuffer::create(fbSpec);

		m_activeScene = createRef<Scene>();

		m_editorCamera = EditorCamera(30.0f, 1.778f, 0.1f, 1000.0f);
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
			m_editorCamera.setViewportSize(m_viewportSize.x, m_viewportSize.y);

			m_activeScene->onViewportResize((uint32_t)m_viewportSize.x, (uint32_t)m_viewportSize.y);
		}


		// Update
		if (m_viewportFocused)
		{
			m_cameraController.onUpdate(ts);
			m_editorCamera.onUpdate(ts);
		}


		// Render

		Renderer2D::resetStats();
		m_framebuffer->bind();
		RenderCommand::setClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		RenderCommand::clear();

		
		// Update scene
		m_activeScene->onUpdateEditor(ts, m_editorCamera);

		auto[mx, my] = ImGui::GetMousePos();
		mx -= m_viewportBounds[0].x;
		my -= m_viewportBounds[0].y;
		glm::vec2 viewportSize = m_viewportBounds[1] - m_viewportBounds[0];

		my = viewportSize.y - my;
		int mouseX = (int)mx;
		int mouseY = (int)my;

		if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)viewportSize.x && mouseY < (int)viewportSize.y)
		{
			int pixelData = m_framebuffer->readPixel(1, mouseX, mouseY);
			SV_CORE_WARN("Pixel data = {0}", pixelData);
		}


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

				if (ImGui::MenuItem("New", "Ctrl+N"))
					newScene();

				if (ImGui::MenuItem("Open...", "Ctrl+O"))
					openScene();

				if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
					saveSceneAs();

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
		auto viewportOffset = ImGui::GetCursorPos(); // Includes tab bar

		m_viewportFocused = ImGui::IsWindowFocused();
		m_viewportHovered = ImGui::IsWindowHovered();
		Application::get().getImGuiLayer()->blockEvents(!m_viewportFocused && !m_viewportHovered);

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail(); // check what this does
		m_viewportSize = { viewportPanelSize.x, viewportPanelSize.y };

		//SV_WARN("Viewport Size: {0}, {1}", viewportPanelSize.x, viewportPanelSize.y);
		uint32_t textureID = m_framebuffer->getColorAttachmentRendererID();
		ImGui::Image(textureID, ImVec2{ m_viewportSize.x, m_viewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		auto windowSize = ImGui::GetWindowSize();
		ImVec2 minBound = ImGui::GetWindowPos();
		minBound.x += viewportOffset.x;
		minBound.y += viewportOffset.y;

		ImVec2 maxBound = { minBound.x + windowSize.x, minBound.y + windowSize.y };
		m_viewportBounds[0] = { minBound.x, minBound.y };
		m_viewportBounds[1] = { maxBound.x, maxBound.y };


		// Gizmos
		Entity selectedEntity = m_sceneHierarchyPanel.getSelectedEntity();
		if (selectedEntity && m_gizmoType != -1)
		{
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();

			float windowWidth = (float)ImGui::GetWindowWidth();
			float windowHeight = (float)ImGui::GetWindowHeight();
			ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

			// Camera
			 			 
			// Runtime camera from entity
			//auto cameraEntity = m_activeScene->getPrimaryCameraEntity();
			//const auto& camera = cameraEntity.getComponent<CameraComponent>().camera;
			//const glm::mat4& cameraProjection = camera.getProjection();
			//glm::mat4 cameraView = glm::inverse(cameraEntity.getComponent<TransformComponent>().getTransform());

			// Editor camera 
			const glm::mat4& cameraProjection = m_editorCamera.getProjection();
			glm::mat4 cameraView = m_editorCamera.getViewMatrix();

			// Entity transform
			auto& tc = selectedEntity.getComponent<TransformComponent>();
			glm::mat4 transform = tc.getTransform();

			// Snapping 
			bool snap = Input::isKeyPressed(Key::LeftControl);
			float snapValue = 0.5f; // snap to 0.5m for translation/scale
			// snap to 45 degrees for rotation
			if (m_gizmoType == ImGuizmo::OPERATION::ROTATE)
				snapValue = 45.0f;

			float snapValues[3] = { snapValue, snapValue, snapValue };

			ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection), 
				(ImGuizmo::OPERATION)m_gizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform),
				nullptr, snap ? snapValues : nullptr);

			if (ImGuizmo::IsUsing)
			{

				glm::vec3 translation, rotation, scale;
				Math::decomposeTransform(transform, translation, rotation, scale);

				glm::vec3 deltaRotation = rotation - tc.rotation;
				tc.translation = translation;
				tc.rotation += deltaRotation;
				tc.scale = scale;
			}


		}


		ImGui::End();
		ImGui::PopStyleVar();

	}

	void EditorLayer::onEvent(Event& e)
	{
		m_cameraController.onEvent(e);
		m_editorCamera.onEvent(e);

		EventDispatcher dispatcher(e);
		dispatcher.dispatch<KeyPressedEvent>(SV_BIND_EVENT_FN(EditorLayer::onKeyPressed));
	}

	bool EditorLayer::onKeyPressed(KeyPressedEvent& e)
	{
		// Shortcuts
		if (e.getRepeatCount() > 0)
			return false;

		bool control = Input::isKeyPressed(Key::LeftControl) || Input::isKeyPressed(Key::RightControl);
		bool shift = Input::isKeyPressed(Key::LeftShift) || Input::isKeyPressed(Key::RightShift);

		switch (e.getKeyCode())
		{
			case Key::N:
			{
				if (control)
					newScene();
				break;
			}
			case Key::O:
			{
				if (control)
					openScene();

				break;
			}
			case Key::S:
			{
				if (control && shift)
					saveSceneAs();

				break;
			}

			// Gizmos
			case Key::Q:
				m_gizmoType = -1;
				break;
			case Key::W:
				m_gizmoType = ImGuizmo::OPERATION::TRANSLATE;
				break;
			case Key::E:
				m_gizmoType = ImGuizmo::OPERATION::ROTATE;
				break;
			case Key::R:
				m_gizmoType = ImGuizmo::OPERATION::SCALE;
				break;

		}

		return false;
	}

	void EditorLayer::newScene()
	{
		m_activeScene = createRef<Scene>();
		m_activeScene->onViewportResize((uint32_t)m_viewportSize.x, (uint32_t)m_viewportSize.y);
		m_sceneHierarchyPanel.setContext(m_activeScene);
	}

	void EditorLayer::openScene()
	{
		std::string filepath = FileDialogs::openFile("Strvy Scene (*.strvy)\0*.strvy\0");
		if (!filepath.empty())
		{
			m_activeScene = createRef<Scene>();
			m_activeScene->onViewportResize((uint32_t)m_viewportSize.x, (uint32_t)m_viewportSize.y);
			m_sceneHierarchyPanel.setContext(m_activeScene);

			SceneSerializer serializer(m_activeScene);
			serializer.deserialize(filepath);
		}
	}

	void EditorLayer::saveSceneAs()
	{
		std::string filepath = FileDialogs::saveFile("Strvy Scene (*.strvy)\0*.strvy\0");

		if (!filepath.empty())
		{
			SceneSerializer serializer(m_activeScene);
			serializer.serialize(filepath);
		}
	}
}
