#pragma once

#include <vector>

namespace Renderer
{

	struct ExposedVars {


		float ZScale;
		unsigned int Resolution;

		float DiamondSquareAmplitude;
		float DiamondSquarePersistance;
		float DiamondSquareMix;
		bool DiamondSquareEnabled;

		int VoronoiCellsPerAxis;
		int VoronoiFeaturePointsPerCell;
		float VoronoiMix;
		bool VoronoiEnabled;

		float PerturbMaxDisplacement;
		bool PerturbEnabled;

		int ErosionIterations;
		float ErosionTalus;
		float ErosionStrength;
		bool ErosionEnabled;

		std::vector<float>* TerrainVertices;

	};

	struct GeometryVertexData {
		std::vector<float> vertices;
		std::vector<float> normals;
		std::vector<unsigned int> indices;
	};

	struct HeightMap
	{
		int size;
		std::vector<float>& data;

		HeightMap(int n, std::vector<float>& vertices) : size(n), data(vertices) {};

		float& At(int x, int y) const
		{
			x = (x % size + size) % size;
			y = (y % size + size) % size;
			//assuming +Z is height component
			return data[(y * size + x) * 3 + 2];
		}

		void Set(int x, int y, float val) {

			x = (x % size + size) % size;
			y = (y % size + size) % size;
			data[(y * size + x) * 3 + 2] = val;
		}

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
