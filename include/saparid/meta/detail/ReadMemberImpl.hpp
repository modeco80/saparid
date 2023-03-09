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

	template<class TObject, class TMember, class Buffer>
	struct ReadMemberImpl;

	template<class TObject, common::FixedString name, auto Member, class Buffer>
	struct ReadMemberImpl<TObject, u8_<name, Member>, Buffer> {
		constexpr static void _(TObject& dest, Buffer& buffer) {
			(dest.*Member) = *buffer.data();
			buffer += 1;
		}
	};

	template<class TObject, common::FixedString name, auto Member, class Buffer>
	struct ReadMemberImpl<TObject, s8_<name, Member>, Buffer> {
		constexpr static void _(TObject& dest, Buffer& buffer) {
			(dest.*Member) = *buffer.data();
			buffer += 1;
		}
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

	template<class TObject, common::FixedString name, auto Member, class CharT, class Buffer>
	struct ReadMemberImpl<TObject, zstring_<name, Member, CharT>, Buffer> {
		constexpr static void _(TObject& dest, Buffer& buffer) {
			std::basic_string<CharT> string;

			while(true) {
				CharT c = ReadValueEndian<CharT, std::endian::native>(buffer);
				if(c == 0)
					break;
				string += c;
			}

			(dest.*Member) = std::move(string);
		}
	};

} // namespace saparid::meta::detail
