#include "TerrainGeometry.h"
#include "Maths.h"
#include "Noise.h"
#include <memory>

namespace Renderer {

	static std::unique_ptr<ExposedVars> s_TerrainGeometryVariables = std::make_unique<ExposedVars>();

	TerrainGeometry::TerrainGeometry()
	{

		s_TerrainGeometryVariables->ZScale = 0.5f;
		s_TerrainGeometryVariables->NoiseScale = 1.0f;
		s_TerrainGeometryVariables->Resolution = 256;
		s_TerrainGeometryVariables->NoiseOffset = { 0.0, 0.0 };

	}

	TerrainGeometry::~TerrainGeometry()
	{

		ClearAllBuffers();

	}

	void TerrainGeometry::BuildPlane()
	{
		m_Resolution = s_TerrainGeometryVariables->Resolution;

		ClearAllBuffers();

		m_vertexData.vertices.reserve(m_Resolution * 2 * 3);
		m_vertexData.indices.reserve(m_Resolution * 6);

		for (size_t i = 0; i < m_Resolution; i++) {
			for (size_t j = 0; j < m_Resolution; j++) {

				float normalizedX = Utils::normalize_value((float)i, 0, m_Resolution, -1, 1);
				float normalizedY = Utils::normalize_value((float)j, 0, m_Resolution, -1, 1);

				m_vertexData.vertices.push_back(normalizedX);
				m_vertexData.vertices.push_back(normalizedY);
				
				m_vertexData.vertices.push_back(0.0f);

				//dont build tris for edge vertices
				if (j < m_Resolution - 1 && i < m_Resolution - 1) {

					m_vertexData.indices.push_back(i * m_Resolution + j);
					m_vertexData.indices.push_back(i * m_Resolution + (j + 1));
					m_vertexData.indices.push_back((i + 1) * m_Resolution + (j + 1));

					m_vertexData.indices.push_back((i + 1) * m_Resolution + (j + 1));
					m_vertexData.indices.push_back((i + 1) * m_Resolution + j);
					m_vertexData.indices.push_back(i * m_Resolution + j);

					m_triangleCount += 2;
				}
			}
		}
	}

	void TerrainGeometry::ApplyNoise()
	{

		size_t currentZVertexIdx = 2;

		ExposedVars& vars = GetExposedVars();

		while (currentZVertexIdx < m_vertexData.vertices.size()) {
			float x = m_vertexData.vertices[currentZVertexIdx - 2];
			float y = m_vertexData.vertices[currentZVertexIdx - 1];
			m_vertexData.vertices[currentZVertexIdx] = Utils::PerlinNoise2D((x * vars.NoiseScale) + vars.NoiseOffset.x, (y * vars.NoiseScale) + vars.NoiseOffset.y) * vars.ZScale;
			currentZVertexIdx += 3;
		}


	}

	void TerrainGeometry::CalculateNormals(){

		m_vertexData.normals.clear();
		m_vertexData.normals.reserve(m_Resolution * 2 * 3);

		float sideLength = 4.0f / m_Resolution;

		//central differencing solver
		//1. Get indices of neighboring vertices (left, right, up, down)
		//2. Differentiate along X and Y axis to get the normal vector components (dx, dy)
		//3. Z component is the side length of the grid cell * current Z scale
		//4. Normalize
		for (size_t i = 0 ; i < m_Resolution; i++) {
			for (size_t j = 0; j < m_Resolution; j++) {

				size_t index = (i * m_Resolution + j) * 3;

				float centerHeight = m_vertexData.vertices[index + 2];

				float leftHeight = (j > 0) ? m_vertexData.vertices[index - 1] : centerHeight;
				float rightHeight = (j < m_Resolution - 1) ? m_vertexData.vertices[index + 3 + 2] : centerHeight;
				float upHeight = (i > 0) ? m_vertexData.vertices[index - m_Resolution * 3 + 2] : centerHeight;
				float downHeight = (i < m_Resolution - 1) ? m_vertexData.vertices[index + m_Resolution * 3 + 2] : centerHeight;

				float dx = rightHeight - leftHeight;
				float dy = downHeight - upHeight;

				glm::vec3 normal(dx, dy, sideLength);
				normal = glm::normalize(normal);

				m_vertexData.normals.push_back(normal.x);
				m_vertexData.normals.push_back(normal.y);
				m_vertexData.normals.push_back(normal.z);

			}

		}

	}


	void TerrainGeometry::ClearAllBuffers()
	{
		m_vertexData.vertices.clear();
		m_vertexData.indices.clear();
		m_vertexData.normals.clear();
		m_triangleCount = 0;

	}

	ExposedVars& TerrainGeometry::GetExposedVars()
	{
		assert(s_TerrainGeometryVariables);
		return *s_TerrainGeometryVariables;
	}
	
}

