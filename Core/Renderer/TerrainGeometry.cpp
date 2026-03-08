#include "TerrainGeometry.h"
#include "Maths.h"
#include "Noise.h"

namespace Renderer {

	TerrainGeometry::TerrainGeometry()
	{

		m_vertices.reserve(m_Resolution * 4 * 3);
		m_indices.reserve(m_Resolution * 6);

	}

	TerrainGeometry::~TerrainGeometry()
	{

		m_vertices.clear();
		m_indices.clear();

	}

	void TerrainGeometry::BuildPlane()
	{

		for (size_t i = 0; i < m_Resolution; i++) {
			for (size_t j = 0; j < m_Resolution; j++)
			{

				float normalizedX = Utils::normalize_value((float)i, 0, m_Resolution, -1, 1);
				float normalizedY = Utils::normalize_value((float)j, 0, m_Resolution, -1, 1);

				m_vertices.push_back(normalizedX);
				m_vertices.push_back(normalizedY);
				
				m_vertices.push_back(Utils::PerlinNoise2D(normalizedX, normalizedY));

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
	
}

