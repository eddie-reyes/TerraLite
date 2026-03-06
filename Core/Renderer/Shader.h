#pragma once

#include <filesystem>

namespace Renderer {

	static std::string ReadTextFile(const std::filesystem::path& path);
	uint32_t CreateGraphicsShader(const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath);

}