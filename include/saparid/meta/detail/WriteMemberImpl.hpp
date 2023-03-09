//
// Saparid
//
// (C) 2023 modeco80 <lily.modeco80@protonmail.ch>
//

#pragma once

#include <saparid/meta/detail/BufferHelpers.hpp>
#include <saparid/meta/detail/Tags.hpp>

namespace saparid::meta::detail {

	template<class T, class Buffer>
	constexpr void WriteObject(const T& src, Buffer& dest);

	template<class TObject, class TMember, class Buffer>
	struct WriteMemberImpl;

	template<class TObject, common::FixedString name, auto Member, class Buffer>
	struct WriteMemberImpl<TObject, u8_<name, Member>, Buffer> {
		constexpr static void _(const TObject& src, Buffer& buffer) { WriteValueEndian<s8, std::endian::native>(buffer, (src.*Member)); }
	};

	template<class TObject, common::FixedString name, auto Member, class Buffer>
	struct WriteMemberImpl<TObject, s8_<name, Member>, Buffer> {
		constexpr static void _(const TObject& src, Buffer& buffer) { WriteValueEndian<s8, std::endian::native>(buffer, (src.*Member)); }
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

	template<class TObject, common::FixedString name, auto Member, Metastructure Struct, class Buffer>
	struct WriteMemberImpl<TObject, struct_<name, Member, Struct>, Buffer> {
		constexpr static void _(const TObject& src, Buffer& buffer) {
			static_assert(!std::is_same_v<TObject, Struct>, "Cyclic write. Please remove this element from your metastructure definition");
			WriteObject<Struct>((src.*Member), buffer);
		}
	};

	template<class TObject, common::FixedString name, auto Member, class Buffer>
	struct WriteMemberImpl<TObject, zstring_<name, Member>, Buffer> {
		constexpr static void _(const TObject& dest, Buffer& buffer) {
			for(const auto c : (dest.*Member))
				WriteValueEndian<char, std::endian::native>(buffer, c);

			WriteValueEndian<char, std::endian::native>(buffer, 0);
		}
	};

} // namespace saparid::meta::detail
