#pragma once

#include <vector>
#include <functional>
#include <glm/gtc/matrix_transform.hpp>


namespace Renderer
{

	struct ExposedVars {
		float ZScale;
		float NoiseScale;
		unsigned int Resolution;

	};

	class TerrainGeometry {

	public:

		TerrainGeometry();
		~TerrainGeometry();

		void BuildPlane();
		void ApplyNoise();

		std::vector<float>& GetVertices() { return m_vertices; }
		std::vector<float>& GetNormals() { return m_normals; }
		std::vector<unsigned int>& GetIndices() { return m_indices; }
		size_t GetTriangleCount() const { return m_triangleCount; }

		void CalculateNormals();

		static ExposedVars& GetExposedVars();

		void ClearAllBuffers();

	private:

		size_t m_Resolution = 128;

		size_t m_triangleCount = 0;

		std::vector<float> m_vertices;
		std::vector<float> m_normals;
		std::vector<unsigned int> m_indices;

		bool m_OrbitEnabled = false;

	};

};
