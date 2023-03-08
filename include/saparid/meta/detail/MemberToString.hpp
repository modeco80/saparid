//
// Saparid
//
// (C) 2023 modeco80 <lily.modeco80@protonmail.ch>
//

#pragma once

#include <saparid/meta/detail/Tags.hpp>
#include <fmt/core.h>

namespace saparid::meta::detail {

	template<class TMember>
	struct MemberToStringImpl {
		static std::string _() { return "<ERROR: MemberToStringImpl is not specialized..?>"; }
	};

	template<common::FixedString name, auto Member>
	struct MemberToStringImpl<u8_<name, Member>> {
		static std::string _() { return fmt::format("u8 {}", name); }
	};

	template<common::FixedString name, auto Member>
	struct MemberToStringImpl<s8_<name, Member>> {
		static std::string _() { return fmt::format("s8 {}", name); }
	};

	template<common::FixedString name, auto Member, std::endian Endian>
	struct MemberToStringImpl<u16_<name, Member, Endian>> {
		static std::string _() { return fmt::format("u16_{} {}", (Endian == std::endian::little) ? "le" : "be", name); }
	};

	template<common::FixedString name, auto Member, std::endian Endian>
	struct MemberToStringImpl<s16_<name, Member, Endian>> {
		static std::string _() { return fmt::format("s16_{} {}", (Endian == std::endian::little) ? "le" : "be", name); }
	};

	template<common::FixedString name, auto Member, std::endian Endian>
	struct MemberToStringImpl<u32_<name, Member, Endian>> {
		static std::string _() { return fmt::format("u32_{} {}", (Endian == std::endian::little) ? "le" : "be", name); }
	};

	template<common::FixedString name, auto Member, std::endian Endian>
	struct MemberToStringImpl<s32_<name, Member, Endian>> {
		static std::string _() { return fmt::format("s32_{} {}", (Endian == std::endian::little) ? "le" : "be", name); }
	};

	template<common::FixedString name, auto Member, std::endian Endian>
	struct MemberToStringImpl<u64_<name, Member, Endian>> {
		static std::string _() { return fmt::format("u64_{} {}", (Endian == std::endian::little) ? "le" : "be", name); }
	};

	template<common::FixedString name, auto Member, std::endian Endian>
	struct MemberToStringImpl<s64_<name, Member, Endian>> {
		static std::string _() { return fmt::format("s64_{} {}", (Endian == std::endian::little) ? "le" : "be", name); }
	};

	template<common::FixedString name, auto Member, std::endian Endian>
	struct MemberToStringImpl<float_<name, Member, Endian>> {
		static std::string _() { return fmt::format("float_{} {}", (Endian == std::endian::little) ? "le" : "be", name); }
	};

	template<common::FixedString name, auto Member, std::endian Endian>
	struct MemberToStringImpl<double_<name, Member, Endian>> {
		static std::string _() { return fmt::format("double_{} {}", (Endian == std::endian::little) ? "le" : "be", name); }
	};

	template<common::FixedString name, auto Member, class CharT>
	struct MemberToStringImpl<astring_<name, Member, CharT>> {
		static std::string _() { return fmt::format("astring {}", name); }
	};

	template<class TMember>
	auto MemberToString() {
		return MemberToStringImpl<TMember>::_();
	}

} // namespace saparid::meta::detail
