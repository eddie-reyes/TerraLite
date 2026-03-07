#pragma once

#include <vector>
#include <glm/gtc/matrix_transform.hpp>

namespace Renderer
{

	class TerrainGeometry {

	public:

		TerrainGeometry();
		~TerrainGeometry();

		void BuildPlane();

		std::vector<float>& GetVertices() { return m_vertices; }

		std::vector<unsigned int>& GetIndices() { return m_indices; }

		size_t GetTriangleCount() const { return m_triangleCount; }

		void OrbitAroundCenter(glm::mat4& View, glm::mat4& Model, float mouseDeltaX, float mouseDeltaY);

		void ToggleOrbitEnabled() { m_OrbitEnabled = !m_OrbitEnabled; }

		bool GetOrbitEnabled() const { return m_OrbitEnabled; } 

	private:

		size_t m_Resolution = 512;

		size_t m_triangleCount = 0;

		std::vector<float> m_vertices;

		std::vector<unsigned int> m_indices;

		bool m_OrbitEnabled = false;

	};

};
