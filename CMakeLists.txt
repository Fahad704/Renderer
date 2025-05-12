cmake_minimum_required(VERSION 3.10)

project(Renderer)

add_executable(Renderer WIN32 src/Window.cpp src/Vector.cpp)

target_link_libraries(Renderer user32 kernel32)

set(WINDOWS_SUBSYTEM "WINDOWS")

target_compile_definitions(Renderer PRIVATE 
	_UNICODE
	 UNICODE
	$<$<CONFIG:Release>:NDEBUG>
)

target_compile_options(Renderer PUBLIC $<$<CONFIG:Release>:-O3>)