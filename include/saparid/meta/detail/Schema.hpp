//
// Saparid
//
// (C) 2023 modeco80 <lily.modeco80@protonmail.ch>
//

#pragma once

namespace saparid::meta::detail {

	template<class T>
	constexpr auto Schema() = delete; // define a specialization of this function for your type if you're seeing this in a diagnostic..

	// clang-format off

	template<class T>
	concept Metastructure = requires(T t) {
		{ Schema<T>() }; // Schema queries should compile, otherwise this isn't a metastructure
	};

	// clang-format on

} // namespace saparid::meta::detail
