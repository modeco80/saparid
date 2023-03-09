//
// Saparid
//
// (C) 2021-2023 modeco80 <lily.modeco80@protonmail.ch>
//

#include <cstdio>

#include <saparid/common/CommonTypes.hpp>

#include <saparid/proto/WireTypes.hpp>

using namespace saparid;

struct other {
	u16 hi;
};

struct test {
	u8 hi;
	u16 test;
	std::string str;
	other o;
};

namespace saparid::meta::detail {
	template<>
	constexpr auto Schema<::other>() {
		using ::other;
		return kumi::make_tuple(
			u16_<"hi", &other::hi>{}
		);
	}

	template<>
	constexpr auto Schema<::test>() {
		using ::test;
		return kumi::make_tuple(
			u8_<"hi", &test::hi> {},
			u16_<"test", &test::test, std::endian::big> {},
			zstring_<"str", &test::str> {},
			struct_<"o", &test::o, ::other>{}
		);
	}



} // namespace saparid::meta::detail

int main() {

	// set up a tiny little test buffer
	u8 buffer[64] {};
	common::SpanBuffer span(&buffer[0], sizeof(buffer));

	// write a test object into that buffer, and reset it
	// back to the start of memory
	meta::WriteObject(test { 0xFE, 0xCADE, "hello worldasdfghjkl", { 32 } }, span);
	span.reset();

	// Print the contents of memory
	for(auto c : span) {
		fmt::print("{:02x} ", c);
	}

	fmt::print("\n");

	test read{};
	meta::ReadObject(read, span);

	fmt::print("magic word: {}\n", meta::StringifyObject(read));
	return 0;
}
