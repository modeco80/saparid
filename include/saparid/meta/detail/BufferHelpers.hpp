//
// Saparid
//
// (C) 2023 modeco80 <lily.modeco80@protonmail.ch>
//

#pragma once

#include <bit>
#include <cstring>
#include <saparid/common/CommonTypes.hpp>

namespace saparid::common::meta::detail {

	// adjustor trait to pun certain types so that std::bitswap doesn't scream
	template<class T>
	struct BestBitType {
		using type = T;
	};

	template<>
	struct BestBitType<float> {
		using type = u32;
	};

	template<>
	struct BestBitType<double> {
		using type = u64;
	};

	template<class T>
	constexpr auto ToBestBits(T& t) {
		return *std::bit_cast<typename BestBitType<T>::type*>(&t);
	}

	template<class T, std::endian Endian, class Buffer>
	constexpr static T ReadValueEndian(Buffer& buffer) {
		T t;

		// read data & advance the buffer
		std::memcpy(std::bit_cast<u8*>(&t), buffer.data(), sizeof(T));
		buffer += sizeof(T);

		if constexpr(Endian != std::endian::native && sizeof(T) > 1)
			return std::byteswap(ToBestBits(t));

		return t;
	}

	template<class T, std::endian Endian, class Buffer>
	constexpr static void WriteValueEndian(Buffer& buffer, const T value) {
		auto out = value;

		if constexpr(Endian != std::endian::native && sizeof(T) > 1)
			out = std::byteswap(ToBestBits(out));

		// read data
		std::memcpy(buffer.data(), &out, sizeof(T));
		buffer += sizeof(T);
	}

}
