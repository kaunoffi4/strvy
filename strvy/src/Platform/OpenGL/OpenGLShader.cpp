#include "svpch.h"
#include "OpenGLShader.h"
#include <glad/glad.h>

#include <glm/gtc/type_ptr.hpp>

namespace strvy {

	static GLenum ShaderTypeFromString(const std::string& type)
	{
		if (type == "vertex")	
			return GL_VERTEX_SHADER;
		if (type == "fragment" || type == "pixel")	
			return GL_FRAGMENT_SHADER;

		SV_CORE_ASSERT(false, "Uknown shader type!");
		return 0;
	}

	OpenGLShader::OpenGLShader(const std::string& filepath)
	{
		SV_PROFILE_FUNCTION();

		std::string source = readFile(filepath);
		auto shaderSources = preProcess(source);
		compile(shaderSources);

		// Extract name from filepath
		auto lastSlash = filepath.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastDot = filepath.rfind('.'); // the same as find_last_of()
		auto count = lastDot == std::string::npos ? filepath.size() - lastSlash : lastDot - lastSlash;
		m_name = filepath.substr(lastSlash, count);
		// "u_ViewProjection"
		m_VPLocation = glGetUniformLocation(m_rendererID, "u_ViewProjection");
	}


	OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
		: m_name(name)
	{
		SV_PROFILE_FUNCTION();

		std::unordered_map<GLenum, std::string> sources;
		sources[GL_VERTEX_SHADER] = vertexSrc;
		sources[GL_FRAGMENT_SHADER] = fragmentSrc;
		compile(sources);
	}

	
	OpenGLShader::~OpenGLShader()
	{
		SV_PROFILE_FUNCTION();

		glDeleteProgram(m_rendererID);
	}

	std::string OpenGLShader::readFile(const std::string& filepath)
	{
		SV_PROFILE_FUNCTION();

		std::string result;
		std::ifstream in(filepath, std::ios::in | std::ios::binary); // input file stream / we tell this is the input file in 2nd parameter and that we want it to be read as binary in 3rd
		if (in)
		{
			in.seekg(0, std::ios::end); // this will go at the very end of the file
			result.resize(in.tellg());  // param is meaning where that actual ptr is
										// since the pointer to the file is at the end it resulting in the size of the file
			in.seekg(0, std::ios::beg); // undo the previous operation
			in.read(&result[0], result.size());
			in.close();					// close the stream
		}
		else
		{
			SV_CORE_ERROR("Could not open file '{0}'", filepath);
		}

		return result;
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::preProcess(const std::string& source)
	{
		SV_PROFILE_FUNCTION();

		std::unordered_map<GLenum, std::string> shaderSources;

		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0);
		while (pos != std::string::npos)
		{
			size_t eol = source.find_first_of("\r\n", pos); // end of line = eol
			SV_CORE_ASSERT(eol != std::string::npos, "Syntax error");
			size_t begin = pos + typeTokenLength + 1;
			std::string type = source.substr(begin, eol - begin);
			SV_CORE_ASSERT(type == "vertex" || type == "fragment" || type == "pixel", "Invalid shader type specification");

			size_t nextLinePos = source.find_first_not_of("\r\n", eol);
			pos = source.find(typeToken, nextLinePos);
			shaderSources[ShaderTypeFromString(type)] = source.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos));
		}

		return shaderSources;
	}

	void OpenGLShader::compile(const std::unordered_map<GLenum, std::string>& shaderSources)
	{
		SV_PROFILE_FUNCTION();

		GLuint program = glCreateProgram();
		SV_CORE_ASSERT(shaderSources.size() <= 2, "Maximum count of shaders is 2 at the moment");
		std::array<GLenum, 2> glShaderIDs;  
		int glShaderIDIndex = 0;
		for (const auto& [type, source] : shaderSources)
		{
			GLuint shader = glCreateShader(type);

			const GLchar* sourceCStr = source.c_str();
			glShaderSource(shader, 1, &sourceCStr, 0);

			// Compile the vertex shader
			glCompileShader(shader);

			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				// The maxLength includes the NULL character
				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				// We don't need the shader anymore.
				glDeleteShader(shader);


				SV_CORE_ERROR("{0}", infoLog.data());
				SV_CORE_ASSERT(false, "Shader compilation failure!");
				break;
			}

			glAttachShader(program, shader);
			glShaderIDs[glShaderIDIndex++] = shader;
		}

		// Link our program
		glLinkProgram(program);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(program);

			for (auto id : glShaderIDs)
				glDeleteShader(id);


			SV_CORE_ERROR("{0}", infoLog.data());
			SV_CORE_ASSERT(false, "Shader link failure!");
			return;
		}

		// Always detach shaders after a successful link.
		for (auto id : glShaderIDs)
			glDetachShader(program, id);

		m_rendererID = program;
	}

	void OpenGLShader::bind() const
	{
		SV_PROFILE_FUNCTION();

		glUseProgram(m_rendererID);
	}

	void OpenGLShader::unbind() const
	{
		SV_PROFILE_FUNCTION();

		glUseProgram(0);
	}

	void OpenGLShader::setInt(const std::string& name, int value)
	{
		SV_PROFILE_FUNCTION();

		uploadUniformInt(name, value);
	}

	void OpenGLShader::setIntArray(const std::string& name, int* values, uint32_t count)
	{
		uploadUniformIntArray(name, values, count);
	}

	void OpenGLShader::setFloat(const std::string& name, float value)
	{
		SV_PROFILE_FUNCTION();

		uploadUniformFloat(name, value);
	}

	void OpenGLShader::setFloat3(const std::string& name, const glm::vec3& value)
	{
		SV_PROFILE_FUNCTION();

		uploadUniformFloat3(name, value);
	}

	void OpenGLShader::setFloat4(const std::string& name, const glm::vec4& value)
	{
		SV_PROFILE_FUNCTION();

		uploadUniformFloat4(name, value);
	}

	void OpenGLShader::setMat4(const std::string& name, const glm::mat4& value)
	{
		SV_PROFILE_FUNCTION();

		uploadUniformMat4(name, value);
	}

	void OpenGLShader::uploadUniformInt(const std::string& name, int value)
	{
		GLint location = glGetUniformLocation(m_rendererID, name.c_str());
		glUniform1i(location, value);
	}

	void OpenGLShader::uploadUniformIntArray(const std::string& name, int* values, uint32_t count)
	{
		GLint location = glGetUniformLocation(m_rendererID, name.c_str());
		glUniform1iv(location, count, values);
	}

	void OpenGLShader::uploadUniformFloat(const std::string& name, float value)
	{
		GLint location = glGetUniformLocation(m_rendererID, name.c_str());
		glUniform1f(location, value);
	}

	void OpenGLShader::uploadUniformFloat2(const std::string& name, const glm::vec2& value)
	{
		GLint location = glGetUniformLocation(m_rendererID, name.c_str());
		glUniform2f(location, value.x, value.y);
	}

	void OpenGLShader::uploadUniformFloat3(const std::string& name, const glm::vec3& value)
	{
		GLint location = glGetUniformLocation(m_rendererID, name.c_str());
		glUniform3f(location, value.x, value.y, value.z);
	}

	void OpenGLShader::uploadUniformFloat4(const std::string& name, const glm::vec4& value)
	{
		GLint location = glGetUniformLocation(m_rendererID, name.c_str());
		glUniform4f(location, value.x, value.y, value.z, value.w);
	}

	void OpenGLShader::uploadUniformMat3(const std::string& name, const glm::mat3& matrix)
	{
		GLint location = glGetUniformLocation(m_rendererID, name.c_str());
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::uploadUniformMat4(const std::string& name, const glm::mat4& matrix)
	{
		SV_PROFILE_SCOPE("uploadUniformMat4");
		GLint location;
		if (name == "u_ViewProjection")
		{
			location = m_VPLocation;
		}
		else
			location = glGetUniformLocation(m_rendererID, name.c_str());

		glUniformMatrix4fv(m_VPLocation, 1, GL_FALSE, glm::value_ptr(matrix));
	}

}