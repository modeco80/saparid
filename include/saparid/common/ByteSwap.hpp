//
// saparid
//
// (C) 2021-2023 modeco80 <lily.modeco80@protonmail.ch>
//

#pragma once

#include <bit>
#include <cstdint>
#include <cstddef>
#include <cassert>

#include <saparid/common/Asio.hpp>
#include <boost/asio/buffer.hpp>

namespace saparid::common {

	template<class T>
	inline void ByteSwapBuffer(T* src, std::size_t length) {
		assert(length % sizeof(std::uint16_t));

		auto* buffer = std::bit_cast<std::uint16_t*>(src);
		auto chunkSize = length / sizeof(std::uint16_t);

		for(auto i = 0; i < chunkSize; ++i)
			buffer[i] = std::byteswap(buffer[i]);
	}

	template<class T>
	constexpr void NetworkSwapBuffer(T* src, std::size_t length) {
		if constexpr(std::endian::native == std::endian::little)
			ByteSwapBuffer(src, length);
	}
	
	template<class BufferLike>
	inline void ByteSwapBuffer(BufferLike buffer) {
		ByteSwapBuffer(static_cast<std::uint8_t*>(buffer.data()), buffer.size());
	}

	template<class BufferLike>
	constexpr void NetworkSwapBuffer(BufferLike buffer) {
		if constexpr(std::endian::native == std::endian::little)
			ByteSwapBuffer(buffer);
	}

	/**
	 * Swap a value from host endian to network endian, or vise versa.
	 */
	template <class T>
	constexpr T NetworkSwapValue(T value) {
		if constexpr(std::endian::native == std::endian::little)
			return std::byteswap(value);
		else
			return value;
	}

	/**
	 * An automatically swapped network-byte-order value.
	 */
	template <class T>
	struct NetworkOrdered {
		constexpr NetworkOrdered() = default;
		
		constexpr NetworkOrdered(const T& hostValue)
			: networkValue(NetworkSwapValue(hostValue)) {
		}

		// Implicit conversion byte swaps, function call operator doesn't
		// the call operator is intended to be used when serializing.

		constexpr operator T() const { return NetworkSwapValue(networkValue); }


		constexpr T operator() const { return networkValue; } 

	   private:
		T networkValue {};
	};

} // namespace saparid::common
