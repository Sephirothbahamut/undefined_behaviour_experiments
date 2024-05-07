#pragma once

#include <array>
#include <iostream>
#include "details.h"

// Seemed fancy but actually has plenty of downsides.
// UB because only the first [[no_unique_address]] element of a class is guaranteed to share the same address as the root class, allowing for the reinterpret cast usage to be well defined.
// All reinterpret cast operations to the owner type on the other [[no_unique_address]] elements are UB.
// Even if it wasn't UB it'd be inconveniente since you'd need to manually rewrite and expose all of T's interface in the alias implementation. Quite inconvenient to say the least.
// I only implemented operator++ and operator<< for the sake of the example.

// This version works only on Gcc and Clang, not on MSVC.
// Won't work on MSVC even if you specify __declspec(empty_bases), only x will be correct.
// Note that this "not working in MSVC" is exactly what you should expect by willingly using undefined behaviour.
// x being correct shows the expected behaviour in the only usage that is actually supported by the standard.

namespace attempts
	{
	namespace no_unique_address
		{
		template <typename OWNER_T, typename VALUE_TYPE, size_t INDEX>
		struct array_element_alias
			{
			using owner_t = OWNER_T;

			constexpr const owner_t& owner() const noexcept { return reinterpret_cast<const owner_t&>(*this); }
			constexpr       owner_t& owner()       noexcept { return reinterpret_cast<      owner_t&>(*this); }

			using value_type = VALUE_TYPE;
			inline static constexpr size_t index{INDEX};

			constexpr const value_type& value() const noexcept { return owner().operator[](index); }
			constexpr       value_type& value()       noexcept { return owner().operator[](index); }

			constexpr operator const value_type&() const noexcept { return value(); }
			constexpr operator       value_type&()       noexcept { return value(); }

			constexpr const value_type* operator&() const noexcept { return std::addressof(value()); }
			constexpr       value_type* operator&()       noexcept { return std::addressof(value()); }

			constexpr value_type& operator=(const auto& new_value) noexcept { value() = new_value; return *this; }

			constexpr value_type operator++(int) noexcept { return value()++; }
			constexpr value_type operator++() noexcept { return ++value(); }
			};

		template <typename OWNER_T, typename VALUE_TYPE, size_t INDEX>
		std::ostream& operator<<(std::ostream& os, const array_element_alias<OWNER_T, VALUE_TYPE, INDEX>& element) noexcept
			{
			os << VALUE_TYPE{element};
			return os;
			}
		
		namespace clean
			{
			template <typename DERIVED_T, typename VALUE_TYPE, size_t SIZE>
			struct vec_aliases;

			template <typename DERIVED_T, typename VALUE_TYPE>
			struct vec_aliases<DERIVED_T, VALUE_TYPE, 1>
				{
				[[no_unique_address]] array_element_alias<DERIVED_T, VALUE_TYPE, 0> x;
				};
			template <typename DERIVED_T, typename VALUE_TYPE>
			struct vec_aliases<DERIVED_T, VALUE_TYPE, 2> : vec_aliases<DERIVED_T, VALUE_TYPE, 1>
				{
				[[no_unique_address]] array_element_alias<DERIVED_T, VALUE_TYPE, 1> y;
				};
			template <typename DERIVED_T, typename VALUE_TYPE>
			struct vec_aliases<DERIVED_T, VALUE_TYPE, 3> : vec_aliases<DERIVED_T, VALUE_TYPE, 2>
				{
				[[no_unique_address]] array_element_alias<DERIVED_T, VALUE_TYPE, 2> z;
				};
			template <typename DERIVED_T, typename VALUE_TYPE>
			struct vec_aliases<DERIVED_T, VALUE_TYPE, 4> : vec_aliases<DERIVED_T, VALUE_TYPE, 3>
				{
				[[no_unique_address]] array_element_alias<DERIVED_T, VALUE_TYPE, 3> w;
				};

			template <typename T, size_t size>
			struct vec : vec_aliases<vec<T, size>, T, size>, std::array<T, size>
				{
				template <typename ...Args>
				vec(Args&&... args) : std::array<T, size>{std::forward<Args>(args)...} {}
				};

			// This implementation does not satisfy is_standard_layout. The next one does.
			// static_assert(std::is_standard_layout_v<vec<float, 1>>);
			// static_assert(std::is_standard_layout_v<vec<float, 2>>);
			// static_assert(std::is_standard_layout_v<vec<float, 3>>);
			// static_assert(std::is_standard_layout_v<vec<float, 4>>);
			}
		
		// Way more verbose, and still UB, but at least succeeds the is_standard_layout constraint.
		namespace standard_layout
			{
			template <typename T, size_t size>
			struct vec;
			
			template <typename T>
			struct vec<T, 1> : details::expose_array_interface_crtp
				<
				vec<T, 1>, 
				[](const auto& self) -> const auto& { return self.array; },
				[](      auto& self) ->       auto& { return self.array; }
				>
				{
				[[no_unique_address]] array_element_alias<vec<T, 1>, T, 0> x;
				std::array<T, 1> array;
				
				template <typename ...Args>
				vec(Args&&... args) : array{std::forward<Args>(args)...} {}
				};
			template <typename T>
			struct vec<T, 2> : details::expose_array_interface_crtp
				<
				vec<T, 2>, 
				[](const auto& self) -> const auto& { return self.array; },
				[](      auto& self) ->       auto& { return self.array; }
				>
				{
				[[no_unique_address]] array_element_alias<vec<T, 2>, T, 0> x;
				[[no_unique_address]] array_element_alias<vec<T, 2>, T, 1> y;
				std::array<T, 2> array;
				
				template <typename ...Args>
				vec(Args&&... args) : array{std::forward<Args>(args)...} {}
				};
			template <typename T>
			struct vec<T, 3> : details::expose_array_interface_crtp
				<
				vec<T, 3>, 
				[](const auto& self) -> const auto& { return self.array; },
				[](      auto& self) ->       auto& { return self.array; }
				>
				{
				[[no_unique_address]] array_element_alias<vec<T, 3>, T, 0> x;
				[[no_unique_address]] array_element_alias<vec<T, 3>, T, 1> y;
				[[no_unique_address]] array_element_alias<vec<T, 3>, T, 2> z;
				std::array<T, 3> array;
				
				template <typename ...Args>
				vec(Args&&... args) : array{std::forward<Args>(args)...} {}
				};
			template <typename T>
			struct vec<T, 4> : details::expose_array_interface_crtp
				<
				vec<T, 4>, 
				[](const auto& self) -> const auto& { return self.array; },
				[](      auto& self) ->       auto& { return self.array; }
				>
				{
				[[no_unique_address]] array_element_alias<vec<T, 4>, T, 0> x;
				[[no_unique_address]] array_element_alias<vec<T, 4>, T, 1> y;
				[[no_unique_address]] array_element_alias<vec<T, 4>, T, 2> z;
				[[no_unique_address]] array_element_alias<vec<T, 4>, T, 3> w;
				std::array<T, 4> array;
				
				template <typename ...Args>
				vec(Args&&... args) : array{std::forward<Args>(args)...} {}
				};
		
			static_assert(std::is_standard_layout_v<vec<float, 1>>);
			static_assert(std::is_standard_layout_v<vec<float, 2>>);
			static_assert(std::is_standard_layout_v<vec<float, 3>>);
			static_assert(std::is_standard_layout_v<vec<float, 4>>);
			}
		}
	}