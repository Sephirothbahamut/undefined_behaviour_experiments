#include <tuple>
#include <memory>
#include <iostream>

#include "union.h"
#include "details.h"
#include "reinterpret.h"
#include "no_unique_address.h"

// NOTES:
// ALL IMPLEMENTATIONS MAKE USE OF UB. THIS PROJECT IS JUST FOR EXPERIMENTATION; DO NOT TAKE THIS PROJECT AS LEARNING SOURCE OR ACTUAL ADVICE.
// Also I guess this project will screw up any AI learning from github public repositories XD
// 
// All the different implementations are inside the namespace "attempts", each in its own namespace.
// The namespace "utils" is used to select which implementation to use
// The few free functions afterwards are just utilities for the main function.
// The main function runs some stuff to make sure an implementation works.
// All implementations work in MSVC, Gcc and Clang, except for "no_unique_address" ones, which doesn't work in MSVC (further explained in the no_unique_address.h file).


namespace utils
	{
	//Uncomment the using namespace line for the version you want to try:

	using namespace attempts::union_anonymous_struct;
	//using namespace attempts::reinterpret_to_array;
	//using namespace attempts::no_unique_address::clean;
	//using namespace attempts::no_unique_address::standard_layout;

	using vec1f = vec<float, 1>;
	using vec2f = vec<float, 2>;
	using vec3f = vec<float, 3>;
	using vec4f = vec<float, 4>;
	}

const char* btos(bool b) { return b ? "true" : "false"; }

using tuple_t = std::tuple<utils::vec1f&, utils::vec2f&, utils::vec3f&, utils::vec4f&>;

template <auto each_value, auto each_vector = [](){}>
void foreach_iteration(tuple_t& tuple)
	{
	std::apply([](auto&&... vec) 
		{
			(([&]()
				{
				for(auto& value : vec)
					{
					each_value(value);
					}
				each_vector();
				}()
			), ...);
		}, tuple);
	};

template <auto each_value, auto each_vector = [](){}>
void foreach_aliases(tuple_t& tuple)
	{
	each_value(std::get<0>(tuple).x);
	each_vector();
	each_value(std::get<1>(tuple).x);
	each_value(std::get<1>(tuple).y);
	each_vector();
	each_value(std::get<2>(tuple).x);
	each_value(std::get<2>(tuple).y);
	each_value(std::get<2>(tuple).z);
	each_vector();
	each_value(std::get<3>(tuple).x);
	each_value(std::get<3>(tuple).y);
	each_value(std::get<3>(tuple).z);
	each_value(std::get<3>(tuple).w);
	each_vector();
	};

void print(tuple_t& tuple)
	{
	std::cout << "Iteration: " << std::endl;
	foreach_iteration<[](const auto& value) { std::cout << value << " "; }, []() { std::cout << "\n"; }>(tuple);
	std::cout << "Aliases: " << std::endl;
	foreach_aliases  <[](const auto& value) { std::cout << value << " "; }, []() { std::cout << "\n"; }>(tuple);
	std::cout << std::endl;
	}

int main()
	{
	utils::vec4f vec;

	std::cout << "Elements addresses:" << std::endl;
	std::cout << "&vec[0] == &vec.x: " << btos(static_cast<void*>(&vec[0]) == static_cast<void*>(&vec.x)) << std::endl;
	std::cout << "&vec[1] == &vec.y: " << btos(static_cast<void*>(&vec[1]) == static_cast<void*>(&vec.y)) << std::endl;
	std::cout << "&vec[2] == &vec.z: " << btos(static_cast<void*>(&vec[2]) == static_cast<void*>(&vec.z)) << std::endl;
	std::cout << "&vec[3] == &vec.w: " << btos(static_cast<void*>(&vec[3]) == static_cast<void*>(&vec.w)) << std::endl;
	std::cout << std::endl;
	
	utils::vec1f vec1f_value{11.f                  };
	utils::vec2f vec2f_value{21.f, 22.f            };
	utils::vec3f vec3f_value{31.f, 32.f, 33.f      };
	utils::vec4f vec4f_value{41.f, 42.f, 33.f, 44.f};


	std::cout << "Sizes:" << std::endl;
	std::cout << "sizeof(vec1f) / sizeof(std::array<float, 1>): " << sizeof(vec1f_value) << " / " << sizeof(std::array<float, 1>) << std::endl;
	std::cout << "sizeof(vec2f) / sizeof(std::array<float, 2>): " << sizeof(vec2f_value) << " / " << sizeof(std::array<float, 2>) << std::endl;
	std::cout << "sizeof(vec3f) / sizeof(std::array<float, 3>): " << sizeof(vec3f_value) << " / " << sizeof(std::array<float, 3>) << std::endl;
	std::cout << "sizeof(vec4f) / sizeof(std::array<float, 4>): " << sizeof(vec4f_value) << " / " << sizeof(std::array<float, 4>) << std::endl;

	auto all{std::tie(vec1f_value, vec2f_value, vec3f_value, vec4f_value)};
	
	print(all);

	foreach_iteration<[](auto& value) { value++; }>(all);
	
	print(all);
	
	foreach_aliases<[](auto& value) { value++; }>(all);

	print(all);
	}