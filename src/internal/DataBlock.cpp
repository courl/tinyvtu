#include "internal/DataBlock.hpp"

#include <cstring>
#include <vector>
#include "zlib.h"

namespace tinyvtu::internal {
	Data compressData(const std::uint8_t *source, std::uint64_t size,
	                  const compression::Info &compression) {
		if (!compression.active) {
			// Uncompressed data has the following format
			// [#bytes][DATA]
			// and [#bytes] is a std::uint32_t
			Data blockData;
			blockData.resize(static_cast<std::uint32_t>(sizeof(std::uint32_t) + size));
			// copy [#bytes]
			std::memcpy(&blockData[0], &size, sizeof(std::uint32_t));
			// copy & encode [DATA]
			std::memcpy(&blockData[sizeof(std::uint32_t)], source, size);
			return blockData;
		}
		// Compressed data has the following format
		// [#blocks][#u-size][#p-size][#c-size-1]...[#c-size-#blocks][DATA]
		// each [#...] is std::uint32_t
		// [#blocks] is the number of blocks
		// [#u-size] is the size of an uncompressed block
		// [#p-size] is the size of the last partial uncompressed block
		// [#c-size-i] is the size of the i-th compressed block
		// [DATA] the data
		const std::uint32_t blockSize = compression.blockSize;
		const std::uint32_t numberOfFullBlocks = size / blockSize;
		const bool hasPartialBlock = size % blockSize != 0;
		const std::uint32_t numberOfBlocks = numberOfFullBlocks + (hasPartialBlock ? 1 : 0);
		const std::uint32_t lastPartialBlockSize = size - (numberOfFullBlocks) * blockSize;

		const std::uint32_t compressedBlockSize = std::max((numberOfFullBlocks > 0) ? compressBound(blockSize) : 0,
		                                                   compressBound(lastPartialBlockSize));
		auto compressedBlock = std::vector<std::uint8_t>(compressedBlockSize);

		Data prequel; // We write [#blocks][#u-size][#p-size][#c-size-1]...[#c-size-#blocks] in prequel
		Data rawData; // We write [DATA] in rawData and later combine the two buffers

		prequel.resize(sizeof(std::uint32_t) * (1 + 1 + 1 + numberOfBlocks));
		// copy [#blocks]
		std::memcpy(&prequel[0], &numberOfBlocks, sizeof(std::uint32_t));
		// copy [#u-size]
		std::memcpy(&prequel[sizeof(std::uint32_t)], &blockSize, sizeof(std::uint32_t));
		// copy [#p-size]
		std::memcpy(&prequel[2 * sizeof(std::uint32_t)], &lastPartialBlockSize, sizeof(std::uint32_t));
		for (auto i = 0u; i < numberOfFullBlocks; ++i) {
			uLongf destSize = compressedBlockSize;
			const auto ret = compress2(compressedBlock.data(), &destSize,
			                           &source[static_cast<std::size_t>(i) * blockSize],
			                           blockSize,
			                           compression.level);
			if (ret != Z_OK)
				throw std::runtime_error("[tinyvtk::createBlockData] zlib compression failed for full block "
				                         + std::to_string(i) + " with error code " + std::to_string(ret));

			rawData.insert(rawData.end(), compressedBlock.data(), compressedBlock.data() + destSize);
			// copy [#c-size-i]
			std::memcpy(&prequel[static_cast<std::size_t>(3 + i) * sizeof(std::uint32_t)], &destSize,
			            sizeof(std::uint32_t));
		}
		if (hasPartialBlock) {
			uLongf destSize = compressedBlockSize;
			const auto ret = compress2(compressedBlock.data(), &destSize,
			                           &source[static_cast<std::size_t>(numberOfFullBlocks * blockSize)],
			                           lastPartialBlockSize,
			                           compression.level);
			if (ret != Z_OK)
				throw std::runtime_error("[tinyvtk::createBlockData] zlib compression failed for partial block of size "
				                         + std::to_string(lastPartialBlockSize) + " with error code "
				                         + std::to_string(ret));

			rawData.insert(rawData.end(), compressedBlock.data(), compressedBlock.data() + destSize);
			// copy [#c-size-i]
			std::memcpy(&prequel[((3 + numberOfFullBlocks) * sizeof(std::uint32_t))],
			            &destSize, sizeof(std::uint32_t));
		}
		rawData.insert(rawData.begin(), prequel.begin(), prequel.end());

		return rawData;
	}
}
