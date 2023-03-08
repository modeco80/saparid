//
// Saparid
//
// (C) 2023 modeco80 <lily.modeco80@protonmail.ch>
//

#pragma once

#include <saparid/common/CommonTypes.hpp>
#include <saparid/common/FixedString.hpp>

namespace saparid::meta::detail {

	template<common::FixedString name, auto Member>
	struct _TagBase {
		constexpr static auto Name = name;
		constexpr static auto MemPtr = Member;
	};

	template<common::FixedString name, auto Member>
	struct u8_ : public _TagBase<name, Member> {};

	template<common::FixedString name, auto Member>
	struct s8_ : public _TagBase<name, Member> {};

	template<common::FixedString name, auto Member, std::endian Endian = std::endian::little>
	struct u16_ : public _TagBase<name, Member> {};

	template<common::FixedString name, auto Member, std::endian Endian = std::endian::little>
	struct s16_ : public _TagBase<name, Member> {};

	template<common::FixedString name, auto Member, std::endian Endian = std::endian::little>
	struct u32_ : public _TagBase<name, Member> {};

	template<common::FixedString name, auto Member, std::endian Endian = std::endian::little>
	struct s32_ : public _TagBase<name, Member> {};

	template<common::FixedString name, auto Member, std::endian Endian = std::endian::little>
	struct u64_ : public _TagBase<name, Member> {};

	template<common::FixedString name, auto Member, std::endian Endian = std::endian::little>
	struct s64_ : public _TagBase<name, Member> {};

	template<common::FixedString name, auto Member, std::endian Endian = std::endian::little>
	struct float_ : public _TagBase<name, Member> {};

	template<common::FixedString name, auto Member, std::endian Endian = std::endian::little>
	struct double_ : public _TagBase<name, Member> {};

	
    template<common::FixedString name, auto Member, class CharT = char>
	struct astring_ : public _TagBase<name, Member> {};


	// TODO:
	// 
	// discard_<Size> discards Size bytes
	// 
	// array_?
	// 
	// astringlen_<name, Member, CharT>
	// 
	// pstring_<name, Member, CharT, LengthT=u32, LengthEndian> - a string which encoides its length by prefixing it
	// pstringlen_<name, Member, CharT, LengthT, LengthEndian> - same as astringlen, but for prefix strings
	//
	// User provided tags? This SHOULD just work, provided the user
	// adds respective implementations ReadMemberImpl and WriteMemberImpl 
	// for their tag, but besides looking at the current code there 
	// really isn't much documentation on how to do that
}
