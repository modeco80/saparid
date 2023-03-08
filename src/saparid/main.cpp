//
// Saparid
//
// (C) 2021-2023 modeco80 <lily.modeco80@protonmail.ch>
//

#include <cstdio>

#include <saparid/common/CommonTypes.hpp>

#include <saparid/meta/meta.hpp>

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
			u16_<"test", &test::test> {}
		);
	}
} // namespace saparid::meta::detail

int main() {
	std::printf("the magic word is: %s\n", meta::StringifyObject(test { .hi = 0xFE, .test = 0xCADE }).c_str());

	std::printf("Hello world!\n");
	return 0;
}
