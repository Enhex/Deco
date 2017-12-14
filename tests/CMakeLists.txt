if (MSVC)
	cmake_minimum_required(VERSION 3.10)	# to be able to set C++17
else()
	cmake_minimum_required(VERSION 3.8)		# to be able to set C++17
endif(MSVC)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(${CMAKE_CURRENT_BINARY_DIR}/conanbuildinfo.cmake)

project(deco_tests)
conan_basic_setup()


# add single cpp file project
function(add_test name)
	set(file ${name}.cpp)

	if (MSVC)
		add_compile_options(/W4) 
	endif(MSVC)

	add_executable(${name} ${file})
	conan_target_link_libraries(${name})
	
	if (MSVC)
		set_target_properties(${name} PROPERTIES COMPILE_FLAGS "/permissive-")
	endif(MSVC)
endfunction(add_test)


add_test(parse)
add_test(serialize)
add_test(serialize_nested)
add_test(deco_set)
add_test(deco_NVP)
add_test(integral)
add_test(floating_point)
add_test(string)
add_test(unescaped_string)
add_test(multiline_string)
add_test(array)
add_test(vector)
add_test(deque)
add_test(list)
add_test(forward_list)
add_test(set)
add_test(multiset)
add_test(unordered_set)
add_test(unordered_multiset)
add_test(map)
add_test(multimap)
add_test(unordered_map)
add_test(unordered_multimap)


# startup project
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT serialize)
