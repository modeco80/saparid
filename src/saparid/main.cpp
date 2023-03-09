//
// Saparid
//
// (C) 2021-2023 modeco80 <lily.modeco80@protonmail.ch>
//

#include <cstdio>

#include <saparid/common/CommonTypes.hpp>

#include <saparid/proto/WireTypes.hpp>

using namespace saparid;

struct test {
	u8 hi;
	u16 test;
};

namespace saparid::meta::detail {
	template<>
	constexpr auto Schema<::test>() {
		using ::test;
		return kumi::make_tuple(
			u8_<"hi", &test::hi> {}, 
			u16_<"test", &test::test, std::endian::big> {}
		);
	}
} // namespace saparid::meta::detail

int main() {

	u8 buffer[16] {};
	common::SpanBuffer span(&buffer[0], sizeof(buffer));


	meta::WriteObject(test { 0xFE, 0xCADE }, span);

	span.reset();

	for(auto c : span) {
		std::printf("%02x ", c);
	}

	std::printf("\n");

	test read;
	meta::ReadObject(read, span);

	std::printf("the magic word is: %s\n", meta::StringifyObject(read).c_str());

	std::printf("Hello world!\n");
	return 0;
}
