#pragma once

#include <vector>
#include <functional>
#include <glm/gtc/matrix_transform.hpp>


namespace Renderer
{

	struct ExposedVars {
		float ZScale;
		float PerlinNoiseScale;
		glm::vec2 PerlinNoiseOffset;
		float PerlinMix;
		float DiamondSquareMix;
		float VoronoiMix;
		bool PerturbEnabled;


		unsigned int Resolution;

	};

	struct GeometryVertexData {
		std::vector<float> vertices;
		std::vector<float> normals;
		std::vector<unsigned int> indices;
	};

	class TerrainGeometry {

	public:

		TerrainGeometry();
		~TerrainGeometry();

		void BuildPlane();
		void ApplyNoise();

		GeometryVertexData& GetVertexData() { return m_vertexData; }
		size_t GetTriangleCount() const { return m_triangleCount; }

		void CalculateNormals();

		static ExposedVars& GetExposedVars();

		void ClearAllBuffers();

	private:

		size_t m_Resolution = 128;

		size_t m_triangleCount = 0;

		GeometryVertexData m_vertexData;

	};

};
