//
// Saparid
//
// (C) 2023 modeco80 <lily.modeco80@protonmail.ch>
//

#pragma once

#include <saparid/meta/detail/BufferHelpers.hpp>
#include <saparid/meta/detail/Tags.hpp>

namespace saparid::meta::detail {

	template<class TObject, class TMember, class Buffer>
	struct WriteMemberImpl;

	template<class TObject, common::FixedString name, auto Member, class Buffer>
	struct WriteMemberImpl<TObject, u8_<name, Member>, Buffer> {
		constexpr static void _(const TObject& src, Buffer& buffer) {
			*buffer.data() = (src.*Member);
			buffer += 1;
		}
	};

	template<class TObject, common::FixedString name, auto Member, class Buffer>
	struct WriteMemberImpl<TObject, s8_<name, Member>, Buffer> {
		constexpr static void _(const TObject& src, Buffer& buffer) {
			*buffer.data() = (src.*Member);
			buffer += 1;
		}
	};

	template<class TObject, common::FixedString name, auto Member, std::endian Endian, class Buffer>
	struct WriteMemberImpl<TObject, u16_<name, Member, Endian>, Buffer> {
		constexpr static void _(const TObject& src, Buffer& buffer) { WriteValueEndian<u16, Endian>(buffer, (src.*Member)); }
	};

	template<class TObject, common::FixedString name, auto Member, std::endian Endian, class Buffer>
	struct WriteMemberImpl<TObject, s16_<name, Member, Endian>, Buffer> {
		constexpr static void _(const TObject& src, Buffer& buffer) { WriteValueEndian<s16, Endian>(buffer, (src.*Member)); }
	};

	template<class TObject, common::FixedString name, auto Member, std::endian Endian, class Buffer>
	struct WriteMemberImpl<TObject, u32_<name, Member, Endian>, Buffer> {
		constexpr static void _(const TObject& src, Buffer& buffer) { WriteValueEndian<u32, Endian>(buffer, (src.*Member)); }
	};

	template<class TObject, common::FixedString name, auto Member, std::endian Endian, class Buffer>
	struct WriteMemberImpl<TObject, s32_<name, Member, Endian>, Buffer> {
		constexpr static void _(const TObject& src, Buffer& buffer) { WriteValueEndian<s32, Endian>(buffer, (src.*Member)); }
	};

	template<class TObject, common::FixedString name, auto Member, std::endian Endian, class Buffer>
	struct WriteMemberImpl<TObject, u64_<name, Member, Endian>, Buffer> {
		constexpr static void _(const TObject& src, Buffer& buffer) { WriteValueEndian<u64, Endian>(buffer, (src.*Member)); }
	};

	template<class TObject, common::FixedString name, auto Member, std::endian Endian, class Buffer>
	struct WriteMemberImpl<TObject, s64_<name, Member, Endian>, Buffer> {
		constexpr static void _(const TObject& src, Buffer& buffer) { WriteValueEndian<s64, Endian>(buffer, (src.*Member)); }
	};

	template<class TObject, common::FixedString name, auto Member, std::endian Endian, class Buffer>
	struct WriteMemberImpl<TObject, float_<name, Member, Endian>, Buffer> {
		constexpr static void _(const TObject& src, Buffer& buffer) { WriteValueEndian<float, Endian>(buffer, (src.*Member)); }
	};

	template<class TObject, common::FixedString name, auto Member, std::endian Endian, class Buffer>
	struct WriteMemberImpl<TObject, double_<name, Member, Endian>, Buffer> {
		constexpr static void _(const TObject& src, Buffer& buffer) { WriteValueEndian<double, Endian>(buffer, (src.*Member)); }
	};

	template<class TObject, common::FixedString name, auto Member, class CharT, class Buffer>
	struct WriteMemberImpl<TObject, zstring_<name, Member, CharT>, Buffer> {
		constexpr static void _(const TObject& dest, Buffer& buffer) {
			for(const auto c : (dest.*Member))
				WriteValueEndian<CharT, std::endian::native>(buffer, c);

			WriteValueEndian<CharT, std::endian::native>(buffer, (CharT)0);
		}
	};

} // namespace saparid::meta::detail
