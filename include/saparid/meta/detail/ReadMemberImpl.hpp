//
// Saparid
//
// (C) 2023 modeco80 <lily.modeco80@protonmail.ch>
//

#pragma once

#include <saparid/meta/detail/BufferHelpers.hpp>
#include <saparid/meta/detail/Tags.hpp>
#include <string>

namespace saparid::meta::detail {

	template<class T, class Buffer>
	constexpr void ReadObject(T& dest, Buffer& src);

	template<class TObject, class TMember, class Buffer>
	struct ReadMemberImpl;

	template<class TObject, common::FixedString name, auto Member, class Buffer>
	struct ReadMemberImpl<TObject, u8_<name, Member>, Buffer> {
		constexpr static void _(TObject& dest, Buffer& buffer) { (dest.*Member) = ReadValueEndian<u8, std::endian::native>(buffer); }
	};

	template<class TObject, common::FixedString name, auto Member, class Buffer>
	struct ReadMemberImpl<TObject, s8_<name, Member>, Buffer> {
		constexpr static void _(TObject& dest, Buffer& buffer) { (dest.*Member) = ReadValueEndian<s8, std::endian::native>(buffer); }
	};

	template<class TObject, common::FixedString name, auto Member, std::endian Endian, class Buffer>
	struct ReadMemberImpl<TObject, u16_<name, Member, Endian>, Buffer> {
		constexpr static void _(TObject& dest, Buffer& buffer) { (dest.*Member) = ReadValueEndian<u16, Endian>(buffer); }
	};

	template<class TObject, common::FixedString name, auto Member, std::endian Endian, class Buffer>
	struct ReadMemberImpl<TObject, s16_<name, Member, Endian>, Buffer> {
		constexpr static void _(TObject& dest, Buffer& buffer) { (dest.*Member) = ReadValueEndian<s16, Endian>(buffer); }
	};

	template<class TObject, common::FixedString name, auto Member, std::endian Endian, class Buffer>
	struct ReadMemberImpl<TObject, u32_<name, Member, Endian>, Buffer> {
		constexpr static void _(TObject& dest, Buffer& buffer) { (dest.*Member) = ReadValueEndian<u32, Endian>(buffer); }
	};

	template<class TObject, common::FixedString name, auto Member, std::endian Endian, class Buffer>
	struct ReadMemberImpl<TObject, s32_<name, Member, Endian>, Buffer> {
		constexpr static void _(TObject& dest, Buffer& buffer) { (dest.*Member) = ReadValueEndian<s32, Endian>(buffer); }
	};

	template<class TObject, common::FixedString name, auto Member, std::endian Endian, class Buffer>
	struct ReadMemberImpl<TObject, u64_<name, Member, Endian>, Buffer> {
		constexpr static void _(TObject& dest, Buffer& buffer) { (dest.*Member) = ReadValueEndian<u64, Endian>(buffer); }
	};

	template<class TObject, common::FixedString name, auto Member, std::endian Endian, class Buffer>
	struct ReadMemberImpl<TObject, s64_<name, Member, Endian>, Buffer> {
		constexpr static void _(TObject& dest, Buffer& buffer) { (dest.*Member) = ReadValueEndian<s64, Endian>(buffer); }
	};

	template<class TObject, common::FixedString name, auto Member, std::endian Endian, class Buffer>
	struct ReadMemberImpl<TObject, float_<name, Member, Endian>, Buffer> {
		constexpr static void _(TObject& dest, Buffer& buffer) { (dest.*Member) = ReadValueEndian<float, Endian>(buffer); }
	};

	template<class TObject, common::FixedString name, auto Member, std::endian Endian, class Buffer>
	struct ReadMemberImpl<TObject, double_<name, Member, Endian>, Buffer> {
		constexpr static void _(TObject& dest, Buffer& buffer) { (dest.*Member) = ReadValueEndian<double, Endian>(buffer); }
	};

	template<class TObject, common::FixedString name, auto Member, Metastructure Struct, class Buffer>
	struct ReadMemberImpl<TObject, struct_<name, Member, Struct>, Buffer> {
		constexpr static void _(TObject& src, Buffer& buffer) {
			static_assert(!std::is_same_v<TObject, Struct>, "Cyclic read. Please remove this element from your metastructure definition");
			ReadObject<Struct>((src.*Member), buffer);
		}
	};

	template<class TObject, common::FixedString name, auto Member, class Buffer>
	struct ReadMemberImpl<TObject, zstring_<name, Member>, Buffer> {
		constexpr static void _(TObject& dest, Buffer& buffer) {
			// MAYBE:
			//
			// We could create a copy of the buffer here, "read" the string from that copy,
			// and then we construct a string_view inside the class with how many characters we read.
			// Once we've done that, we can then advance the actual buffer with how many bytes we read from that copy.
			//
			// Advantages of this:
			// - No Allocator requirements/extra memory allocations
			// - Metastructure types can stay POD, meaning they can be cheap to copy and move around
			//	 (though honestly it's probably not a good idea to copy them)
			// Disadvantages:
			// - The user has to *ensure* the underlying data buffer (Not the Buffer object) the metastructure was read from
			//	 lasts longer than the read metastructure itself. (This is tricky, and a fair bit unwieldly without wrappers
			//	 	which manage the lifetime of the buffer behind the scenes.)
			// - In the case the member IS a string, we would have to duplicate the code and essentially have this version
			//	 used in explicitly those cases. In this case, I think maintainability (however subjective that is, considering
			//	 that I've managed to shoehorn a template library into this project) trumps performance.

			std::string string;
			string.reserve(8);

			while(true) {
				char c = ReadValueEndian<char, std::endian::native>(buffer);
				if(c == 0)
					break;
				string.push_back(c);
			}

			// Move assign into the member, to avoid yet more allocation pressure.
			(dest.*Member) = std::move(string);
		}
	};

} // namespace saparid::meta::detail
