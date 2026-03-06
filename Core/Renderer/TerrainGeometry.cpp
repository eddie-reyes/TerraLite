#include "TerrainGeometry.h"


TerrainGeometry::TerrainGeometry()
{

	m_vertices.reserve(m_Resolution * 4 * 3);
	m_indices.reserve(m_Resolution * 6);

}

TerrainGeometry::~TerrainGeometry()
{
}

void TerrainGeometry::BuildPlane()
{

	for (size_t i = 0; i < m_Resolution; i++) {
		for (size_t j = 0; j < m_Resolution; j++)
		{

			m_vertices.push_back((float)i);
			m_vertices.push_back((float)j);
			m_vertices.push_back(0.0f);

			m_indices.push_back(i * m_Resolution + j);
			m_indices.push_back(i * m_Resolution + (j + 1));
			m_indices.push_back((i + 1) * m_Resolution + (j + 1));

			m_indices.push_back((i + 1) * m_Resolution + (j + 1));
			m_indices.push_back(i * m_Resolution + (j + 1));
			m_indices.push_back(i * m_Resolution + j);

		}
	}

}
