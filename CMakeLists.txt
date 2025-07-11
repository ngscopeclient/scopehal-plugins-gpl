cmake_minimum_required(VERSION 3.20)
project(scopehal-plugins-gpl VERSION 0.1)
enable_language(CXX)

# Build with C++ 17 on Linux for maximum compatibility with older system
# but for Windows, enable C++ 20 to bypass a deprecation warning
# (workaround for https://github.com/KhronosGroup/Vulkan-Hpp/issues/2034)
# even though we do not yet use any C++ 20 features in our codebase
if(WIN32)
	set(CMAKE_CXX_STANDARD 20)
else()
	set(CMAKE_CXX_STANDARD 17)
endif()

set(CMAKE_CXX_STANDARD_REQUIRED ON) # error if compiler doesn't support c++17
set(CMAKE_CXX_EXTENSIONS OFF) # use c++17 instead of gnu++17

# Compiler warnings for GCC/Clang
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
	add_compile_options(-Wall -Wextra -Wuninitialized)
	add_compile_options(-Wshadow -Wpedantic -Wcast-align -Wwrite-strings)
	add_compile_options(-Wmissing-declarations -Wvla)
  add_compile_options(-Werror -Wno-error=deprecated-declarations -Wno-error=unused-parameter -Wno-error=unused-variable -Wno-error=unused-result -Wno-error=shadow)
endif()

# Compiler warnings specific to GCC or Clang
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 13)
	add_compile_options(-Woverloaded-virtual=1)
elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Woverloaded-virtual -Wpointer-sign -Wno-gnu-zero-variadic-macro-arguments -Wno-unknown-warning-option)
endif()
add_compile_options(-mtune=native)
add_compile_options("$<$<CONFIG:RELEASE>:-O3>")
add_compile_options("$<$<CONFIG:RELWITHDEBINFO>:-O3;-g>")
add_compile_options("$<$<CONFIG:DEBUG>:-Og;-D_DEBUG;-g>")
add_compile_options("$<$<CONFIG:DEBUGNOOPT>:-O0;-D_DEBUG;-g>")

# Needed for %zu/%zd on MSYS2, must be added here to avoid redefinition
if(WIN32 AND (NOT MSVC))
    add_compile_definitions(__USE_MINGW_ANSI_STDIO=1)
endif()

# Package detection
find_package(PkgConfig MODULE REQUIRED)

# Default build type
if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE "Release")
endif()

if(SANITIZE)
	add_compile_options(-fsanitize=address -fsanitize=undefined)
	add_link_options(-fsanitize=address -fsanitize=undefined)
endif()

if(WIN32)
	add_compile_options(-D_USE_MATH_DEFINES -D_POSIX_THREAD_SAFE_FUNCTIONS)
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	pkg_check_modules(LINUXGPIB libgpib QUIET IMPORTED_TARGET)
	if(LINUXGPIB_FOUND)
		message("-- Found Linux GPIB: ${LINUXGPIB_LINK_LIBRARIES}")
	else()
		message("-- Linux GPIB not found, GPIB instrument support will not be available.")
	endif()
endif()

pkg_check_modules(SIGCXX QUIET IMPORTED_TARGET sigc++-3.0) # look for latest version first
if(NOT SIGCXX_FOUND)
	pkg_check_modules(SIGCXX QUIET IMPORTED_TARGET sigc++-2.0) # look for older version too
endif()
if(NOT SIGCXX_FOUND)
	message(FATAL_ERROR "Unable to find any version of sigc++; this is required to build the plugin.")
endif()

########################################################################################################################
# The actual plugin library

# You must configure these to point to the relevant paths
if(NOT SCOPEHAL_INCLUDE_DIR)
	message(FATAL_ERROR "You must set SCOPEHAL_INCLUDE_DIR to the folder containing libscopehal headers")
endif()

if(NOT SCOPEHAL_LIB_DIR)
	message(FATAL_ERROR "You must set SCOPEHAL_LIB_DIR to the folder containing libscopehal.so")
endif()

add_library(scopehal-plugins-gpl SHARED
	plugin-main.cpp
	SCPILinuxGPIBTransport.cpp
	)

target_link_libraries(scopehal-plugins-gpl
	${SCOPEHAL_LIB_DIR}/libscopehal.so
	PkgConfig::SIGCXX
	)

include_directories(scopehal-plugins-gpl
	${SCOPEHAL_INCLUDE_DIR}
	${SCOPEHAL_LIB_DIR}	# because config.h ends up here
	)

if(LINUXGPIB_FOUND)
	target_link_libraries(scopehal-plugins-gpl
		PkgConfig::LINUXGPIB
	)
	target_compile_definitions(scopehal-plugins-gpl PUBLIC HAS_LINUXGPIB)

endif()
set_property(TARGET scopehal-plugins-gpl PROPERTY POSITION_INDEPENDENT_CODE ON)
