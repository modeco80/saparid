//
// Saparid
//
// (C) 2023 modeco80 <lily.modeco80@protonmail.ch>
//

// Various metastructure object utilities.

#include <saparid/meta/detail/Schema.hpp>
#include <saparid/meta/detail/ReadWriteMember.hpp>
#include <saparid/meta/detail/MemberToString.hpp>
#include <kumi/tuple.hpp>
#include <fmt/core.h>

namespace saparid::meta::detail {

	/** Go through all members of the schema for a given object. **/
	template<class T, class OnMember>
	constexpr void ForEachMember(OnMember&& onMember) {
		kumi::for_each_index([&](auto index, const auto& member) {
			static_cast<void>(index);
			onMember(member); 
		}, Schema<T>());
	}

	/** Query how many members are inside of a schema. **/
	template<class T>
	consteval auto MemberCount() {
		return kumi::size<decltype(Schema<T>())>::value;
	}

	/** Generate a simple string representation of an object and the contents of its data members. Useful for debugging.
		The format of this string is simple. */
	template<class T>
	auto StringifyObject(const T& object) {
		std::string str = "{ ";
		auto lastCommaIndex = MemberCount<T>() - 1;
		std::size_t index {};

		ForEachMember<T>([&](auto member) { 
			using M = decltype(member);
			auto memberName = MemberToString<M>();
			const auto& value = (object.*M::MemPtr);

			auto transformValue = [&value]() {
				using V = std::remove_cvref_t<decltype(value)>;
				if constexpr(std::is_same_v<V, std::string>)
					return fmt::format("\"{}\"", value);
				else
					if constexpr(Metastructure<V>)
						return StringifyObject(value);
					else
						return fmt::format("{}", value);
			};

			// Do some semi-fancy comma formatting.
			if(index++ < lastCommaIndex)
				str += fmt::format("{} = {}, ", memberName, transformValue());
			else
				str += fmt::format("{} = {}", memberName, transformValue());
		});

		str += " }";
		return str;
	}

	/** Read an object from a user provided Buffer object. **/
	template<class T, class Buffer>
	constexpr void ReadObject(T& dest, Buffer& src) {
		ForEachMember<T>([&](auto member) { ReadMember<T, decltype(member)>(dest, src); });
	}

	/** Write an object to a user provided Buffer object. **/
	template<class T, class Buffer>
	constexpr void WriteObject(const T& src, Buffer& dest) {
		ForEachMember<T>([&](auto member) { WriteMember<T, decltype(member)>(src, dest); });
	}

}
