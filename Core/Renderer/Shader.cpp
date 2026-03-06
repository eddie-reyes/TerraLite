#include "Shader.h"

#include <iostream>
#include <fstream>
#include <assert.h>

#include <glad/gl.h>

namespace Renderer
{
	static std::string ReadTextFile(const std::filesystem::path& path)
	{
		std::ifstream file(path);

		if (!file.is_open())
		{
			std::cerr << "Failed to open file: " << path.string() << std::endl;
			assert(false);
			return {};
		}

		std::ostringstream contentStream;
		contentStream << file.rdbuf();
		return contentStream.str();
	}


	uint32_t CreateGraphicsShader(const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath)
	{
		std::string vertexShaderSource = ReadTextFile(vertexPath);
		std::string fragmentShaderSource = ReadTextFile(fragmentPath);

		// Vertex shader

		GLuint vertexShaderHandle = glCreateShader(GL_VERTEX_SHADER);

		const GLchar* source = (const GLchar*)vertexShaderSource.c_str();
		glShaderSource(vertexShaderHandle, 1, &source, 0);

		glCompileShader(vertexShaderHandle);

		GLint isCompiled = 0;
		glGetShaderiv(vertexShaderHandle, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(vertexShaderHandle, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(vertexShaderHandle, maxLength, &maxLength, &infoLog[0]);

			std::cerr << infoLog.data() << std::endl;

			glDeleteShader(vertexShaderHandle);
			return -1;
		}

		// Fragment shader

		GLuint fragmentShaderHandle = glCreateShader(GL_FRAGMENT_SHADER);

		source = (const GLchar*)fragmentShaderSource.c_str();
		glShaderSource(fragmentShaderHandle, 1, &source, 0);

		glCompileShader(fragmentShaderHandle);

		isCompiled = 0;
		glGetShaderiv(fragmentShaderHandle, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(fragmentShaderHandle, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(fragmentShaderHandle, maxLength, &maxLength, &infoLog[0]);

			std::cerr << infoLog.data() << std::endl;

			glDeleteShader(fragmentShaderHandle);
			return -1;
		}

		// Program linking

		GLuint program = glCreateProgram();
		glAttachShader(program, vertexShaderHandle);
		glAttachShader(program, fragmentShaderHandle);
		glLinkProgram(program);

		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			std::cerr << infoLog.data() << std::endl;

			glDeleteProgram(program);
			glDeleteShader(vertexShaderHandle);
			glDeleteShader(fragmentShaderHandle);

			return -1;
		}

		glDetachShader(program, vertexShaderHandle);
		glDetachShader(program, fragmentShaderHandle);
		return program;
	}
}