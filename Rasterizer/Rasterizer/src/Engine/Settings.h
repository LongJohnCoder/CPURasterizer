#pragma once

#include <cstdint>

constexpr int Width = 1024;
constexpr int Height = 720;

namespace Engine
{
	struct Settings final
	{
		int MSSA{};
		int lightModelId{};
		int sceneId{};
		uint32_t trianglesCount{};
		uint32_t spheresCount{};
		float fps{};
		bool useShadows;
	};
}
