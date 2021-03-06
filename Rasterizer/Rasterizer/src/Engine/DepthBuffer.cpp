#include "DepthBuffer.h"

#include "Concurrency.h"

#include "../SIMD/AVX.h"

namespace Engine
{
	DepthBuffer::DepthBuffer(int width, int height):
		tileDimX((width + TILE_SIZE - 1) >> TILE),
		tileDimY((height + TILE_SIZE - 1) >> TILE)
	{
		buffer.resize(tileDimY * tileDimX);
		Clear();
	}

	void DepthBuffer::Clear()
	{
		Concurrency::ForEach(buffer.begin(), buffer.end(), [this](TileDepth3D& tile3D)
		{
			for (int i = 0; i < 4; ++i)
				for (auto& tile2D : tile3D[i])
					for (auto& tile1D : tile2D)
						tile1D = INFINITY;
		});
	}

	AVXBool DepthBuffer::ZTest(const AVXFloat& depth, int x, int y, uint32_t sampleId, const AVXBool& mask)
	{
		const int tileX = x >> TILE;
		const int tileY = y >> TILE;

		TileDepth3D& tile3D = buffer[tileY * tileDimX + tileX];

		const int intraTileX = x & (TILE_SIZE - 1);
		const int intraTileY = y & (TILE_SIZE - 1);

		AVXFloat& currentDepth = tile3D[sampleId][intraTileX >> 1][(TILE_SIZE - 1 - intraTileY) >> 1];

		AVXBool ret = depth <= currentDepth;
		currentDepth = AVX::Select(ret & mask, depth, currentDepth);

		return ret;
	}
}
