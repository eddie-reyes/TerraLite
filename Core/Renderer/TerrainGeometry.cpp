#include "TerrainGeometry.h"
#include "Maths.h"
#include "Noise.h"
#include "../Utils/Globals.h"
#include <memory>

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

		for (size_t i = 0; i < m_Resolution; i++) {
			for (size_t j = 0; j < m_Resolution; j++) {

				float normalizedX = Utils::NormalizeValueRange((float)i, 0, m_Resolution, -1, 1);
				float normalizedY = Utils::NormalizeValueRange((float)j, 0, m_Resolution, -1, 1);

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

			Noise::normalizeZValues(m_vertexData.vertices);
		}
	} 

	void TerrainGeometry::CalculateNormals(){

		m_vertexData.normals.clear();
		m_vertexData.normals.reserve(m_Resolution * m_Resolution * 3);

		float sideLength = 4.0f / m_Resolution;

		//central differencing solver
		//1. Get indices of neighboring vertices (left, right, up, down)
		//2. Differentiate along X and Y axis to get the normal vector components (dx, dy)
		//3. Z component is the side length of the grid cell
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

