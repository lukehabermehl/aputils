cmake_minimum_required(VERSION 3.0)

project(libaputils)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS "-std=c++11")

include_directories(/usr/local/include "include")
file(GLOB SOURCES "src/*.cpp" "src/*.hpp")

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)

set(PUBLIC_HEADERS "include/apu_audioprocessingunit.hpp"
	"include/apu_array.hpp"
	"include/apu_file.hpp"
	"include/apu_host.hpp"
	"include/apu_loader.hpp"
	"include/apu_logger.hpp"
	"include/apu_mod.hpp"
	"include/apu_number.hpp"
	"include/apu_obj.hpp"
	"include/apu_obj_enum.hpp"
	"include/apu_osc.hpp"
	"include/apu_pahost.hpp"
	"include/apu_parameter.hpp"
	"include/apu_parametermap.hpp"
	"include/apu_str.hpp"
	"include/apu_thread_manager.hpp"
	"include/aputils.h")

add_library(aputils STATIC ${SOURCES})

install(TARGETS aputils DESTINATION /usr/local/lib/)
install(FILES ${PUBLIC_HEADERS} DESTINATION /usr/local/include/aputils)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/test/gunit)

add_custom_command(TARGET aputils
					PRE_BUILD
					COMMAND mkdir ARGS -p lib
					WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_custom_target("uninstall")
add_custom_command(TARGET "uninstall" 
					POST_BUILD
					COMMAND rm -rf /usr/local/lib/libaputils.a
					COMMAND rm -rf /usr/local/include/aputils*)

add_custom_target("docs")
add_custom_command(TARGET "docs"
				   POST_BUILD
				   COMMAND scripts/docs.sh
				   COMMAND echo "Index written to docs/doxy/html/index.html"
				   WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
