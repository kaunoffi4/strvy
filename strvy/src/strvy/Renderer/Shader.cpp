#include "svpch.h"
#include "Shader.h"


#include "Renderer.h"
#include "platform/OpenGL/OpenGLShader.h"

namespace strvy {

	Ref<Shader> Shader::create(const std::string& filepath)
	{
		switch (Renderer::getAPI())
		{
			case RendererAPI::API::None:		SV_CORE_ASSERT(false, " rendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:		return std::make_shared<OpenGLShader>(filepath);
		}

		SV_CORE_ASSERT(false, "Unknown rendererAPI!");
		return nullptr;
	}

	Ref<Shader> Shader::create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		switch (Renderer::getAPI())
		{
			case RendererAPI::API::None:		SV_CORE_ASSERT(false, " rendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:		return std::make_shared<OpenGLShader>(name, vertexSrc, fragmentSrc);
		}

		SV_CORE_ASSERT(false, "Unknown rendererAPI!");
		return nullptr;
	}

	void ShaderLibrary::add(const std::string& name, const Ref<Shader>& shader)
	{
		SV_CORE_ASSERT(!exists(name), "Shader already exists");
		m_shaders[name] = shader;
	}

	void ShaderLibrary::add(const Ref<Shader>& shader)
	{
		auto& name = shader->getName();
		add(name, shader);
	}

	Ref<Shader> ShaderLibrary::load(const std::string& filepath)
	{
		auto shader = Shader::create(filepath);
		add(shader);

		return shader;
	}

	Ref<Shader> ShaderLibrary::load(const std::string& name, const std::string& filepath)
	{
		auto shader = Shader::create(filepath);
		add(name, shader);

		return shader;
	}

	Ref<Shader> ShaderLibrary::get(const std::string& name)
	{
		SV_CORE_ASSERT(exists(name), "Shader not found");
		return m_shaders[name];
	}

	bool ShaderLibrary::exists(const std::string& name) const
	{
		return m_shaders.find(name) != m_shaders.end();
	}


}