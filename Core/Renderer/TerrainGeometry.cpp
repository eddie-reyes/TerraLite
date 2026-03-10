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

	}

	TerrainGeometry::~TerrainGeometry()
	{

		ClearAllBuffers();

	}

	void TerrainGeometry::BuildPlane()
	{
		m_Resolution = s_TerrainGeometryVariables->Resolution;

		ClearAllBuffers();

		m_vertices.reserve(m_Resolution * 2 * 3);
		m_indices.reserve(m_Resolution * 6);

		for (size_t i = 0; i < m_Resolution; i++) {
			for (size_t j = 0; j < m_Resolution; j++) {

				float normalizedX = Utils::normalize_value((float)i, 0, m_Resolution, -1, 1);
				float normalizedY = Utils::normalize_value((float)j, 0, m_Resolution, -1, 1);

				m_vertices.push_back(normalizedX);
				m_vertices.push_back(normalizedY);
				
				m_vertices.push_back(0.0f);

				//dont build tris for edge vertices
				if (j < m_Resolution - 1 && i < m_Resolution - 1) {

					m_indices.push_back(i * m_Resolution + j);
					m_indices.push_back(i * m_Resolution + (j + 1));
					m_indices.push_back((i + 1) * m_Resolution + (j + 1));

					m_indices.push_back((i + 1) * m_Resolution + (j + 1));
					m_indices.push_back((i + 1) * m_Resolution + j);
					m_indices.push_back(i * m_Resolution + j);

					m_triangleCount += 2;
				}
			}
		}
	}

	void TerrainGeometry::ApplyNoise()
	{

		size_t currentZVertexIdx = 2;

		while (currentZVertexIdx < m_vertices.size()) {
			float x = m_vertices[currentZVertexIdx - 2];
			float y = m_vertices[currentZVertexIdx - 1];
			m_vertices[currentZVertexIdx] = Utils::PerlinNoise2D(x * s_TerrainGeometryVariables->NoiseScale, y * s_TerrainGeometryVariables->NoiseScale) * s_TerrainGeometryVariables->ZScale;
			currentZVertexIdx += 3;
		}


	}

	void TerrainGeometry::CalculateNormals(){

		m_normals.clear();
		m_normals.reserve(m_Resolution * 2 * 3);

		float sideLength = 4.0f / m_Resolution;

		//central differencing solver
		//1. Get indices of neighboring vertices (left, right, up, down)
		//2. Differentiate along X and Y axis to get the normal vector components (dx, dy)
		//3. Z component is the side length of the grid cell * current Z scale
		//4. Normalize
		for (size_t i = 0 ; i < m_Resolution; i++) {
			for (size_t j = 0; j < m_Resolution; j++) {

				size_t index = (i * m_Resolution + j) * 3;

				float centerHeight = m_vertices[index + 2];

				float leftHeight = (j > 0) ? m_vertices[index - 1] : centerHeight;
				float rightHeight = (j < m_Resolution - 1) ? m_vertices[index + 3 + 2] : centerHeight;
				float upHeight = (i > 0) ? m_vertices[index - m_Resolution * 3 + 2] : centerHeight;
				float downHeight = (i < m_Resolution - 1) ? m_vertices[index + m_Resolution * 3 + 2] : centerHeight;

				float dx = rightHeight - leftHeight;
				float dy = downHeight - upHeight;

				glm::vec3 normal(dx, dy, sideLength);
				normal = glm::normalize(normal);

				m_normals.push_back(normal.x);
				m_normals.push_back(normal.y);
				m_normals.push_back(normal.z);

			}

		}

	}


	void TerrainGeometry::ClearAllBuffers()
	{
		m_vertices.clear();
		m_indices.clear();
		m_normals.clear();
		m_triangleCount = 0;

	}

	ExposedVars& TerrainGeometry::GetExposedVars()
	{
		assert(s_TerrainGeometryVariables);
		return *s_TerrainGeometryVariables;
	}
	
}

