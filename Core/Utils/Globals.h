#include "../Renderer/TerrainGeometry.h"

namespace Globals
{
	inline const Renderer::ExposedVars DEFAULT_NOISE_SETTINGS = { 0.5, 512, 0.5, 0.5, 0.666, true, 4, 1, 0.333, true, 0.25, true, 50, 4.0, 0.25, false, nullptr};

	inline const float SIDBAR_OFFSET = 300.0f;

	inline const char * SupportedResolutions[] = { "64x64", "128x128", "256x256", "512x512", "1024x1024" };

}