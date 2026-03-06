#pragma once

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "TerrainGeometry.h"

namespace Renderer {

	class Renderer
	{
	public:

		Renderer();
		~Renderer();

		void Init();
		void Draw(float dt);


	private:

		TerrainGeometry m_TerrainGeometry;

	};

};
