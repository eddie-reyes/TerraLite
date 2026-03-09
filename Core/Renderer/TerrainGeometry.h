#pragma once

#include <vector>
#include <glm/gtc/matrix_transform.hpp>


namespace Renderer
{

	class TerrainGeometry {

	public:

		TerrainGeometry();
		~TerrainGeometry();

		void BuildPlane(size_t resolution);

		std::vector<float>& GetVertices() { return m_vertices; }
		std::vector<float>& GetNormals() { return m_normals; }
		std::vector<unsigned int>& GetIndices() { return m_indices; }
		size_t GetTriangleCount() const { return m_triangleCount; }

		void CalculateNormals();

	private:

		size_t m_Resolution;

		size_t m_triangleCount = 0;

		std::vector<float> m_vertices;
		std::vector<float> m_normals;
		std::vector<unsigned int> m_indices;

		bool m_OrbitEnabled = false;

		float m_ZScale = 0.1;

	};

};
