#pragma once

#include <saparid/common/CommonTypes.hpp>

namespace saparid::common {

	/** A simple implementation of the metastructure library's Buffer concept
		backed by an arbitrary range of bytes. **/
	struct SpanBuffer {
		using SizeType = std::size_t;

		constexpr SpanBuffer() = default;

		constexpr SpanBuffer(u8* buf, SizeType size)
			: begin_(buf),
			  end_(buf + size),
			  cursor_(buf) {}

		// container interface
		constexpr u8* begin() { return cursor_; }
		constexpr u8* end() { return end_; }
		constexpr const u8* begin() const { return cursor_; }
		constexpr const u8* end() const { return end_; }
		constexpr u8* data() { return cursor_; }
		constexpr const u8* data() const { return cursor_; }

		constexpr void reset() { cursor_ = begin_; }

		constexpr SizeType size() const { return (end_ - cursor_); }

		constexpr SizeType BytesAdvanced() { return (cursor_ - begin_); }

		constexpr SpanBuffer& operator+=(SizeType size) {
			cursor_ += size < this->size() ? size : this->size();
			return *this;
		}

		friend constexpr SpanBuffer operator+(const SpanBuffer& buffer, SizeType n) {
			SpanBuffer buf { buffer }; // copy (cheap)
			return buf += n;
		}

	   private:
		u8* begin_ {};
		u8* end_ {};
		u8* cursor_ {};
	};

} // namespace saparid::common
