#pragma once


//it can be included only by client application, not by engine itself (for use by engine applications)

#include "strvy/Core/Application.h"
#include "strvy/Core/Log.h"
#include "strvy/Core/Layer.h"

#include "strvy/Core/Timestep.h"

#include "strvy/Core/Input.h"
#include "strvy/Core/KeyCodes.h"
#include "strvy/Core/MouseButtonCodes.h"
#include "strvy/Renderer/OrthographicCameraController.h"

#include "strvy/ImGui/ImGuiLayer.h"

#include "strvy/Scene/Scene.h"
#include "strvy/Scene/Entity.h"
#include "strvy/Scene/ScriptableEntity.h"
#include "strvy/Scene/Components.h"

// ----Renderer--------------------
#include "strvy/Renderer/Renderer.h"
#include "strvy/Renderer/Renderer2D.h"
#include "strvy/Renderer/RenderCommand.h"

#include "strvy/Renderer/Buffer.h"
#include "strvy/Renderer/Shader.h"
#include "strvy/Renderer/Framebuffer.h"
#include "strvy/Renderer/Texture.h"
#include "strvy/Renderer/SubTexture2D.h"
#include "strvy/Renderer/VertexArray.h"

#include "strvy/Renderer/OrthographicCamera.h"
// --------------------------------


