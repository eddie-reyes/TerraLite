#pragma once

#include "../Utils/Maths.h"
#include "../Vendor/stb_image_write.cpp"
#include "../Vendor/portable-file-dialogs.h"
#include <vector>
#include <filesystem>
#include <string>


namespace FileIO {

	void SaveHeightMapToFile(std::vector<float>& vertices, size_t resolution, int ZScale) {

		std::vector<uint8_t> heightValues(resolution);

		for (int heightIdx = 2; heightIdx < vertices.size(); heightIdx += 3) {

			float normalized = Utils::NormalizeValueRange(vertices[heightIdx], -0.5, 0.5, 0, 1);

			uint8_t grayscaleValue = static_cast<uint8_t>(std::round(normalized * 255.0f));
			heightValues.push_back(grayscaleValue);

		}
		std::string outputPath = std::format("heightmap{}x{}.png", resolution, resolution);

		auto destination = pfd::save_file("Save to disk", outputPath, { "Image Files", "*.png"} ).result();

		if (!destination.empty()) {
			stbi_write_png(outputPath.c_str(), resolution, resolution, 1, heightValues.data(), resolution * sizeof(uint8_t));
			std::cout << "[File IO] Heightmap saved to: " << destination << "\n";
		}
	}


}