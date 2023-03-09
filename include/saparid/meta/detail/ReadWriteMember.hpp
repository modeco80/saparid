//
// Saparid
//
// (C) 2023 modeco80 <lily.modeco80@protonmail.ch>
//

#pragma once

#include <saparid/meta/detail/ReadMemberImpl.hpp>
#include <saparid/meta/detail/WriteMemberImpl.hpp>

// TODO: support enum classes properly (or at the very least, expand the hacks we do
//  to all the types we need to do them to)

namespace saparid::meta::detail {

	template<class TObject, class TMember, class Buffer>
	constexpr auto ReadMember(TObject& dest, Buffer& buffer) {
		return ReadMemberImpl<TObject, TMember, Buffer>::_(dest, buffer);
	}

	template<class TObject, class TMember, class Buffer>
	constexpr auto WriteMember(const TObject& src, Buffer& buffer) {
		return WriteMemberImpl<TObject, TMember, Buffer>::_(src, buffer);
	}

} // namespace saparid::meta::detail
