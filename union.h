#pragma once

#include <array>
#include "details.h"

// UB with a but: major compilers actually support this, mostly because it's totally legal in C.
// gcc explicitly mentions this usage in its documentation: https://gcc.gnu.org/onlinedocs/gcc-13.1.0/gcc/Optimize-Options.html#index-fstrict-aliasing
// As for MSVC, some windows API types make use of this internally, see:
//	typedef union _LARGE_INTEGER {
//		struct {
//			DWORD LowPart;
//			LONG HighPart;
//			} DUMMYSTRUCTNAME;
//		struct {
//			DWORD LowPart;
//			LONG HighPart;
//			} u;
//		LONGLONG QuadPart;
//		} LARGE_INTEGER;

namespace attempts
	{
	namespace union_anonymous_struct
		{
		template <typename T, size_t size>
		struct vec_aliases;

		template<typename T> struct vec_aliases<T, 1> { union { std::array<T, 1> array; struct { T x         ; }; }; };
		template<typename T> struct vec_aliases<T, 2> { union { std::array<T, 2> array; struct { T x, y      ; }; }; };
		template<typename T> struct vec_aliases<T, 3> { union { std::array<T, 3> array; struct { T x, y, z   ; }; }; };
		template<typename T> struct vec_aliases<T, 4> { union { std::array<T, 4> array; struct { T x, y, z, w; }; }; };
		
		template <typename T, size_t size>
		struct vec : 
			vec_aliases<T, size>, 
			details::expose_array_interface_crtp
				<
				vec<T, size>, 
				[](const auto& self) -> const auto& { return self.array; },
				[](      auto& self) ->       auto& { return self.array; }
				>
			{};
		}
	}