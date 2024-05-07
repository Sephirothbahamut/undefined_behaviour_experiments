#pragma once

#include <array>
#include "details.h"

// UB because reinterpret_cast is evil even when the usage seems intuitive. Don't trust reintrepret_cast.

namespace attempts
	{
	namespace reinterpret_to_array
		{
		template <typename T, size_t size>
		struct vec_aliases;

		template <typename T>
		struct vec_aliases<T, 1> { T x; };
		template <typename T>
		struct vec_aliases<T, 2> { T x, y; };
		template <typename T>
		struct vec_aliases<T, 3> { T x, y, z; };
		template <typename T>
		struct vec_aliases<T, 4> { T x, y, z, w; };

		template <typename T, size_t size>
		struct vec :
			vec_aliases<T, size>,
			details::expose_array_interface_crtp
				<
				vec<T, size>,
				[](const auto& self) -> const auto& { return reinterpret_cast<const std::array<T, size>&>(self); },
				[](      auto& self) ->       auto& { return reinterpret_cast<      std::array<T, size>&>(self); }
				>
			{};
		}
	}