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
		static std::string _() { return fmt::format("u16({}) {}", (Endian == std::endian::little) ? "le" : "be", name); }
	};

	template<common::FixedString name, auto Member, std::endian Endian>
	struct MemberToStringImpl<s16_<name, Member, Endian>> {
		static std::string _() { return fmt::format("s16({}) {}", (Endian == std::endian::little) ? "le" : "be", name); }
	};

	template<common::FixedString name, auto Member, std::endian Endian>
	struct MemberToStringImpl<u32_<name, Member, Endian>> {
		static std::string _() { return fmt::format("u32({}) {}", (Endian == std::endian::little) ? "le" : "be", name); }
	};

	template<common::FixedString name, auto Member, std::endian Endian>
	struct MemberToStringImpl<s32_<name, Member, Endian>> {
		static std::string _() { return fmt::format("s32({}) {}", (Endian == std::endian::little) ? "le" : "be", name); }
	};

	template<common::FixedString name, auto Member, std::endian Endian>
	struct MemberToStringImpl<u64_<name, Member, Endian>> {
		static std::string _() { return fmt::format("u64({}) {}", (Endian == std::endian::little) ? "le" : "be", name); }
	};

	template<common::FixedString name, auto Member, std::endian Endian>
	struct MemberToStringImpl<s64_<name, Member, Endian>> {
		static std::string _() { return fmt::format("s64({}) {}", (Endian == std::endian::little) ? "le" : "be", name); }
	};

	template<common::FixedString name, auto Member, std::endian Endian>
	struct MemberToStringImpl<float_<name, Member, Endian>> {
		static std::string _() { return fmt::format("float({}) {}", (Endian == std::endian::little) ? "le" : "be", name); }
	};

	template<common::FixedString name, auto Member, std::endian Endian>
	struct MemberToStringImpl<double_<name, Member, Endian>> {
		static std::string _() { return fmt::format("double({}) {}", (Endian == std::endian::little) ? "le" : "be", name); }
	};

	template<common::FixedString name, auto Member, Metastructure Struct>
	struct MemberToStringImpl<struct_<name, Member, Struct>> {
		static std::string _() { return fmt::format("metastructure {}", name); }
	};

	template<common::FixedString name, auto Member>
	struct MemberToStringImpl<zstring_<name, Member>> {
		static std::string _() { return fmt::format("zstring {}", name); }
	};

	template<class TMember>
	auto MemberToString() {
		return MemberToStringImpl<TMember>::_();
	}

} // namespace saparid::meta::detail
