#pragma once

#include "../Utils/Maths.h"
#include "../Vendor/stb_image.cpp"
#include "../Vendor/stb_image_write.cpp"
#include "../Vendor/portable-file-dialogs.h"
#include <vector>
#include <string>


namespace FileIO {

	inline void SaveHeightMapToFile(std::vector<float>& vertices, size_t resolution) {

		std::vector<uint8_t> heightValues(resolution);

		for (int heightIdx = 2; heightIdx < vertices.size(); heightIdx += 3) {

			float normalized = Utils::NormalizeValueRange(vertices[heightIdx], -0.5, 0.5, 0, 1);

			uint8_t grayscaleValue = static_cast<uint8_t>(std::round(normalized * 255.0f));
			heightValues.push_back(grayscaleValue);

		}
		std::string outputPath = std::format("heightmap_{}.png", resolution);

		auto destination = pfd::save_file("Save to disk", outputPath, { "Image Files", "*.png"} ).result();

		if (!destination.empty()) {
			stbi_write_png(destination.c_str(), resolution, resolution, 1, heightValues.data(), resolution * sizeof(uint8_t)); 
			std::cout << "[File IO] Heightmap saved to: " << destination << "\n";
		}
	}

	inline bool OpenHeightMap(std::vector<float>& vertices) {

		auto selection = pfd::open_file("Select a file", ".", { "Image Files", "*.png" }).result();

		if (selection.empty()) {

			std::cout << "[File IO] No File Selected\n";
			return false;
		}

		int width, height, channels;

		unsigned char* pngData = stbi_load(selection[0].c_str(), &width, &height, &channels, 0);

		if (!(width == height && channels == 1)) {

			std::cout << "[File IO] Invalid file type\n";
			return false;

		}

		std::vector<float> ZValues;
		ZValues.reserve(width * height);

		// Since we forced 1 channel, each pixel is one byte
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				int index = y * width + x;
				unsigned char grayscaleValue = pngData[index];
				float normalized = Utils::NormalizeValueRange(static_cast<float>(grayscaleValue), 0.0, 255.0, -0.5, 0.5);
				ZValues.push_back(normalized);
				
			}
		}

		int ZIndex = 0;
		for (int heightIdx = 2; heightIdx < vertices.size(); heightIdx += 3) {

			vertices[heightIdx] = ZValues[ZIndex];
			ZIndex++;

		}

		
		return true;

	}

}