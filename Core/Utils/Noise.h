#include <glm/gtc/noise.hpp>
#include "../Renderer/TerrainGeometry.h"

/*
All noise interpolation techniques and the corresponding implementations are inspired by:
"Realtime Procedural Terrain Generation: Realtime Synthesis of Eroded Fractal Terrain for Use in Computer Games" 
    Author: Jacob Olsen, xenorg@imada.sdu.dk 
    Department of Mathematics And Computer Science (IMADA)
    University of Southern Denmark
*/

namespace Noise {

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

    inline void normalizeZValues(std::vector<float>& vertices) {

        auto& exposedVars = Renderer::TerrainGeometry::GetExposedVars();

        float minH = vertices[2];
        float maxH = vertices[2];
        
        //find mix/max of all heights
        for (size_t currentZVertexIdx = 2; currentZVertexIdx < vertices.size(); currentZVertexIdx += 3) {

            minH = std::min(minH, vertices[currentZVertexIdx]);
            maxH = std::max(maxH, vertices[currentZVertexIdx]);

        }

        //edge case if somehow the heightmap is a plane (or is almost a plane)
        float range = maxH - minH;
        if (range <= 1e-8f)
            return;

        //normalize
        for (size_t currentZVertexIdx = 2; currentZVertexIdx < vertices.size(); currentZVertexIdx += 3) {

            vertices[currentZVertexIdx] = Utils::NormalizeValueRange(vertices[currentZVertexIdx], minH, maxH, -1, 1) * exposedVars.ZScale;
            
        }

    }

    std::vector<float> ExtractZValuesFromVertices(std::vector<float>& vertices)
    {
        std::vector<float> values;
        values.reserve(vertices.size() / 3);

        for (int zIndex = 2; zIndex < vertices.size(); zIndex += 3) {

            values.push_back(vertices[zIndex]);
            vertices[zIndex] = 0;

        }

        return values;
    }

    //////////////////
    //DIAMOND SQUARE// -- approximates 1/f noise
    //////////////////
	inline std::vector<float> GenerateSmoothedDiamondSquare(std::vector<float>& vertices, size_t resolution) {

        auto& exposedVars = Renderer::TerrainGeometry::GetExposedVars();

        HeightMap hm(resolution, vertices);

        //initialize corners to random value -- NOTE: We use a uniform distribution instead of a gaussian distribution (not ideal). We opt for the former because its faster
        hm.Set(0, 0, Utils::randomFloatUniform(-1.0, 1.0));
        hm.Set(resolution - 1, 0, Utils::randomFloatUniform(-1.0, 1.0));
        hm.Set(0, resolution - 1, Utils::randomFloatUniform(-1.0, 1.0));
        hm.Set(resolution - 1, resolution - 1, Utils::randomFloatUniform(-1.0, 1.0));

        int step = resolution;
        float amplitude = exposedVars.DiamondSquareAmplitude;    
        float persistence = exposedVars.DiamondSquarePersistance;

        //iterative version
        while (step >= 1)
        {
            int half = step / 2;
            if (half == 0) half = 1;

            // Diamond step
            for (int y = half; y < resolution; y += step)
            {
                for (int x = half; x < resolution; x += step)
                {
                    float a = hm.At(x - half, y - half); //bottom left
                    float b = hm.At(x + half, y - half); //bottom right
                    float c = hm.At(x - half, y + half); //upper left
                    float d = hm.At(x + half, y + half); //upper right

                    float avg = 0.25f * (a + b + c + d);

                    // Smoothed midpoint displacement:
                    // reduce displacement in low areas by scaling with avg
                    float offset = Utils::randomFloatUniform(-amplitude, amplitude) * avg;

                    hm.Set(x, y, avg + offset);

                }
            }

            // Square step
            for (int y = 0; y < resolution; y += half)
            {
                int startX = ((y / half) % 2 == 0) ? half : 0;

                for (int x = startX; x < resolution; x += step)
                {
                    float a = hm.At(x - half, y); //left
                    float b = hm.At(x + half, y); //right
                    float c = hm.At(x, y - half); //up
                    float d = hm.At(x, y + half); //down

                    float avg = 0.25f * (a + b + c + d);
                    float offset = Utils::randomFloatUniform(-amplitude, amplitude) * avg;

                    hm.Set(x, y, avg + offset);
                }
            }

            amplitude *= persistence;
            step /= 2;
            if (step == 0) break;
        }

        normalizeZValues(vertices);
        return ExtractZValuesFromVertices(vertices);
	}

    ///////////
    //VORONOI// -- breaks monotony of homogenous noise by creating feature points
    ///////////
    inline std::vector<glm::vec2> GenerateFeaturePoints(int resolution, int cellsPerAxis, int pointsPerCell)
    {
        std::vector<glm::vec2> pts;
        float regionSize = float(resolution) / float(cellsPerAxis);

        for (int gy = 0; gy < cellsPerAxis; ++gy)
        {
            for (int gx = 0; gx < cellsPerAxis; ++gx)
            {
                for (int k = 0; k < pointsPerCell; ++k)
                {
                    float px = (gx + Utils::randomFloatUniform(0.0, 1.0)) * regionSize;
                    float py = (gy + Utils::randomFloatUniform(0.0, 1.0)) * regionSize;
                    pts.push_back({ px, py });
                }
            }
        }

        return pts;
    }

    inline std::vector<float> GenerateVoronoiRidges(std::vector<float>& vertices, size_t resolution)
    {
        auto& exposedVars = Renderer::TerrainGeometry::GetExposedVars();

        HeightMap hm(resolution, vertices);

        int cellsPerAxis = exposedVars.VoronoiCellsPerAxis;
        int pointsPerCell = exposedVars.VoronoiFeaturePointsPerCell;
        std::vector<glm::vec2> features = GenerateFeaturePoints(resolution, cellsPerAxis, pointsPerCell);

        for (int y = 0; y < resolution; ++y)
        {
            for (int x = 0; x < resolution; ++x)
            {
                float d1 = std::numeric_limits<float>::max();
                float d2 = std::numeric_limits<float>::max();

                for (const glm::vec2& p : features)
                {
                    float d = Utils::DistApprox((float)x, (float)y, p.x, p.y, resolution);

                    //find smallest d1 and d2 for all feature points in region
                    if (d < d1)
                    {
                        d2 = d1;
                        d1 = d;
                    }
                    else if (d < d2)
                    {
                        d2 = d;
                    }
                }

                //c1 = 1, c2 = -1
                hm.Set(x, y, d1 - d2);
            }
        }

        normalizeZValues(vertices); 
        return ExtractZValuesFromVertices(vertices);
    }

    //////////
    //PERLIN// -- simulates smooth continuous surfaces
    //////////
    inline std::vector<float> GeneratePerlinNoise(std::vector<float>& vertices, size_t resolution) {

        HeightMap hm(resolution, vertices);

        for(int y = 0; y < resolution; ++y)
        {
            for (int x = 0; x < resolution; ++x)
            {
                hm.Set(x, y, glm::perlin(glm::vec2((float)x * 0.01, (float)y * 0.01)));
            }
        }

        normalizeZValues(vertices);
        return ExtractZValuesFromVertices(vertices);

    }

    ////////////////
    //PERTURBATION// -- removes seams caused by voronoi by displacing original noise
    ////////////////
    inline float BilinearInterp(HeightMap& source, float fx, float fy) {

        int x0 = (int)std::floor(fx);
        int y0 = (int)std::floor(fy);
        int x1 = x0 + 1;
        int y1 = y0 + 1;

        //fractional part of x, y
        float alongX = fx - (float)x0;
        float alongY = fy - (float)y0;

        //get adjacentcies
        float bottomLeft = source.At(x0, y0);
        float bottomRight = source.At(x1, y0);
        float topLeft = source.At(x0, y1);
        float topRight = source.At(x1, y1);

        float bottom = Utils::Lerp(bottomLeft, bottomRight, alongX);
        float top = Utils::Lerp(topLeft, topRight, alongX);
        return Utils::Lerp(bottom, top, alongY);
    }

    inline void Perturb(std::vector<float>& vertices, size_t resolution) {

        auto& exposedVars = Renderer::TerrainGeometry::GetExposedVars();

        //save copy of original noise to sample from
        std::vector<float> sourceVertices = vertices;

        HeightMap source(resolution, sourceVertices);
        HeightMap dest(resolution, vertices);

        //noise plane
        std::vector<float> dispPlane(vertices.size(), 0.0f);

        std::vector<float> noiseX = GenerateSmoothedDiamondSquare(dispPlane, resolution);
        std::vector<float> noiseY = GenerateSmoothedDiamondSquare(dispPlane, resolution);

        float filteringMagnitude = exposedVars.PerturbMaxDisplacement;
        float maxDisplacement = filteringMagnitude * (float)resolution;
        float invZScale = 1 / exposedVars.ZScale;

        int noiseIdx = 0;
        for (int y = 0; y < resolution; ++y)
        {
            for (int x = 0; x < resolution; ++x)
            {

                float dx = noiseX[noiseIdx] * invZScale * maxDisplacement;
                float dy = noiseY[noiseIdx] * invZScale * maxDisplacement;

                float sx = (float)x + dx;
                float sy = (float)y + dy;

                dest.Set(x, y, BilinearInterp(source, sx, sy));
                noiseIdx++;
            }
        }

        normalizeZValues(vertices);

    }

    ///////////
    //EROSION// -- samples Von Neumann adjacencies to erode terrain
    ///////////
    inline void FastErode(std::vector<float>& vertices, float talus, size_t resolution)
    {

        HeightMap hm(resolution, vertices);

        // rotated Von Neumann neighborhood:
        // lower-left, lower-right, upper-left, upper-right
        const int dx[4] = { -1, 1, -1, 1 };
        const int dy[4] = { -1, -1, 1, 1 };

        for (int y = 0; y < resolution; ++y)
        {
            for (int x = 0; x < resolution; ++x)
            {
                float h = hm.At(x, y);

                float dmax = 0.0f;
                int lowestIndex = -1;

                // Find lowest neighbor through max height drop
                for (int i = 0; i < 4; ++i)
                {
                    int nx = x + dx[i];
                    int ny = y + dy[i];

                    float hn = hm.At(nx, ny);
                    float d = h - hn;

                    if (d > dmax)
                    {
                        dmax = d;
                        lowestIndex = i;
                    }
                }

                // New proposed rule from paper:
                // erode only if local drop is small enough
                if (dmax > 0.0f && dmax <= talus)
                {
                    //c = 0.5
                    float delta = 0.25f * dmax;

                    int nx = x + dx[lowestIndex];
                    int ny = y + dy[lowestIndex];

                    float h = hm.At(x, y);
                    float hn = hm.At(nx, ny);

                    hm.Set(x, y, h - delta);
                    hm.Set(nx, ny, hn + delta);
                }
            }
        }
    }

}