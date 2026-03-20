#include "TerrainGeometry.h"
#include "Maths.h"
#include "Noise.h"
#include "../Utils/Globals.h"

namespace Renderer {

	static std::unique_ptr<ExposedVars> s_TerrainGeometryVariables = std::make_unique<ExposedVars>();

	TerrainGeometry::TerrainGeometry()
	{

		s_TerrainGeometryVariables = std::make_unique<ExposedVars>(Globals::DEFAULT_NOISE_SETTINGS);
		
	}

	TerrainGeometry::~TerrainGeometry()
	{

		ClearAllBuffers();

	}

	void TerrainGeometry::BuildPlane()
	{
		m_Resolution = s_TerrainGeometryVariables->Resolution;

		ClearAllBuffers();

		m_vertexData.vertices.reserve(m_Resolution * m_Resolution * 3);
		m_vertexData.indices.reserve(m_Resolution * m_Resolution * 6);

		for (size_t y = 0; y < m_Resolution; y++) {
			for (size_t x = 0; x < m_Resolution; x++) {

				float normalizedX = Utils::NormalizeValueRange((float)y, 0, m_Resolution, -1, 1);
				float normalizedY = Utils::NormalizeValueRange((float)x, 0, m_Resolution, -1, 1);

				m_vertexData.vertices.push_back(normalizedX);
				m_vertexData.vertices.push_back(normalizedY);
				
				m_vertexData.vertices.push_back(0.0f);

				//dont build tris for edge vertices
				if (x < m_Resolution - 1 && y < m_Resolution - 1) {

					m_vertexData.indices.push_back(y * m_Resolution + x);
					m_vertexData.indices.push_back(y * m_Resolution + (x + 1));
					m_vertexData.indices.push_back((y + 1) * m_Resolution + (x + 1));

					m_vertexData.indices.push_back((y + 1) * m_Resolution + (x + 1));
					m_vertexData.indices.push_back((y + 1) * m_Resolution + x);
					m_vertexData.indices.push_back(y * m_Resolution + x);

					m_triangleCount += 2;
				}
			}
		}

		s_TerrainGeometryVariables->TerrainVertices = &m_vertexData.vertices;
	}

	void TerrainGeometry::ApplyNoise()
	{

		auto& exposedVars = GetExposedVars();

		//Step 1. Generate 1/f noise and voronoi noise
		std::vector<float> DiamondSquareValues = Noise::GenerateSmoothedDiamondSquare(m_vertexData.vertices, m_Resolution);
		std::vector<float> VoronoiValues = Noise::GenerateVoronoiRidges(m_vertexData.vertices, m_Resolution);

		int noiseIdx = 0;

		//mix noise
		for (int zIndex = 2; zIndex < m_vertexData.vertices.size(); zIndex += 3) {

			if (exposedVars.DiamondSquareEnabled) m_vertexData.vertices[zIndex] += DiamondSquareValues[noiseIdx] * exposedVars.DiamondSquareMix;
			if (exposedVars.VoronoiEnabled) m_vertexData.vertices[zIndex] += VoronoiValues[noiseIdx] * exposedVars.VoronoiMix;
			noiseIdx++;

		}

		//Step 2. Apply pertubation by sampling 1/f noise. This breaks the seams caused by the voronoi noise.
		if (exposedVars.PerturbEnabled) Noise::Perturb(m_vertexData.vertices, m_Resolution);

		//Step 3. Apply fast erosion method using rotated Von Neumann neighborhoods
		if (exposedVars.ErosionEnabled) {

			for (int i = 0; i < exposedVars.ErosionIterations; ++i)
				Noise::FastErode(m_vertexData.vertices, (exposedVars.ErosionTalus / m_Resolution) * exposedVars.ZScale, m_Resolution);

			NormalizeZValues(m_vertexData.vertices);
		}
	} 

	void TerrainGeometry::CalculateNormals(){

		m_vertexData.normals.clear();
		m_vertexData.normals.reserve(m_Resolution * m_Resolution * 3);

		float sideLength = 4.0f / m_Resolution;

		HeightMap hm(m_Resolution, m_vertexData.vertices);

		//central differencing solver
		//1. Get indices of neighboring vertices (left, right, up, down)
		//2. Differentiate along X and Y axis to get the normal vector components (dx, dy)
		//3. Z component is the side length of the grid cell
		//4. Normalize

		for (size_t y = 0 ; y < m_Resolution; y++) {
			for (size_t x = 0; x < m_Resolution; x++) {

				float centerHeight = hm.At(x, y);

				float leftHeight = (x > 0) ? hm.At(x - 1, y) : centerHeight;
				float rightHeight = (x < m_Resolution - 1) ? hm.At(x + 1, y) : centerHeight;
				float upHeight = (y > 0) ? hm.At(x, y - 1) : centerHeight;
				float downHeight = (y < m_Resolution - 1) ? hm.At(x, y + 1) : centerHeight;

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

	void TerrainGeometry::NormalizeZValues(std::vector<float>& vertices) {

		auto& exposedVars = Renderer::TerrainGeometry::GetExposedVars();

		float minH = vertices[2];
		float maxH = vertices[2];

		//find min/max of all heights
		for (size_t currentZVertexIdx = 2; currentZVertexIdx < vertices.size(); currentZVertexIdx += 3) {

			minH = std::min(minH, vertices[currentZVertexIdx]);
			maxH = std::max(maxH, vertices[currentZVertexIdx]);

		}

		//edge case if somehow the Renderer::HeightMap is a plane (or is almost a plane)
		float range = maxH - minH;
		if (range <= 1e-8f)
			return;

		//normalize
		for (size_t currentZVertexIdx = 2; currentZVertexIdx < vertices.size(); currentZVertexIdx += 3) {

			vertices[currentZVertexIdx] = Utils::NormalizeValueRange(vertices[currentZVertexIdx], minH, maxH, -0.5, 0.5) * exposedVars.ZScale;

		}

	}

	std::vector<float> TerrainGeometry::ExtractZValuesFromVertices(std::vector<float>& vertices)
	{
		std::vector<float> values;
		values.reserve(vertices.size() / 3);

		for (int zIndex = 2; zIndex < vertices.size(); zIndex += 3) {

			values.push_back(vertices[zIndex]);
			vertices[zIndex] = 0;

		}

		return values;
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

