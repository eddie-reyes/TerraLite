#pragma once

#include <vector>

class TerrainGeometry {


public:

	TerrainGeometry();
	~TerrainGeometry();

	void BuildPlane();

	std::vector<float>& GetVertices() { return m_vertices; }

	std::vector<unsigned int>& GetIndices() { return m_indices; }

private:

	size_t m_Resolution = 512;

	std::vector<float> m_vertices;

	std::vector<unsigned int> m_indices;


};