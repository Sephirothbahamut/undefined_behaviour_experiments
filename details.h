#pragma once

// Just an utility class to expose array's interface in the implementations that don't inherit from std::array but have one as a member variable instead.

namespace attempts
	{
	namespace details
		{
		// Used to expose array interface in versions that don't use inheritance from std::array
		template <typename derived_T, auto get_array_const, auto get_array>
		struct expose_array_interface_crtp
			{
			private:
				using derived_t = derived_T;
				constexpr const derived_t& derived() const noexcept { return static_cast<const derived_t&>(*this); }
				constexpr       derived_t& derived()       noexcept { return static_cast<      derived_t&>(*this); }
				constexpr const auto& arr() const noexcept { return get_array_const(derived()); }
				constexpr       auto& arr()       noexcept { return get_array      (derived()); }
			
			public:
				constexpr const auto& operator[](size_t index) const noexcept { return arr()[index]; }
				constexpr       auto& operator[](size_t index)       noexcept { return arr()[index]; }

				constexpr auto begin  () noexcept { return arr().begin  (); }
				constexpr auto end    () noexcept { return arr().end    (); }
				constexpr auto rbegin () noexcept { return arr().rbegin (); }
				constexpr auto rend   () noexcept { return arr().rend   (); }
				constexpr auto cbegin () noexcept { return arr().cbegin (); }
				constexpr auto cend   () noexcept { return arr().cend   (); }
				constexpr auto crbegin() noexcept { return arr().crbegin(); }
				constexpr auto crend  () noexcept { return arr().crend  (); }  
			};
		};
	}