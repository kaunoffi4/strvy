#pragma once

#include "strvy.h"

#include "ParticleSystem.h"

class Sandbox2D : public strvy::Layer
{
public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;

	virtual void onAttach() override;
	virtual void onDetach() override;

	void onUpdate(strvy::Timestep ts) override;
	virtual void onImGuiRender(strvy::Timestep ts) override;
	void onEvent(strvy::Event& e) override;
private:
	strvy::OrthographicCameraController m_cameraController;

	strvy::Ref<strvy::Texture2D> m_checkerboardTexture;
	strvy::Ref<strvy::Texture2D> m_spriteSheet;
	strvy::Ref<strvy::SubTexture2D> m_textureStairs;

	// Temp
	strvy::Ref<strvy::VertexArray> m_squareVA;
	strvy::Ref<strvy::Shader> m_flatColorShader;

	glm::vec4 m_squareColor = { 0.2f, 0.3f, 0.8f, 1.0f };

	ParticleSystem m_particleSystem;
	ParticleProps m_particle;
};