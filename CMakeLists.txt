cmake_minimum_required(VERSION 3.0)

project(libaputils)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS "-std=c++11")

include_directories(/usr/local/include "include")
file(GLOB SOURCES "src/*.cpp", "src/*.hpp")

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)

set(PUBLIC_HEADERS "include/autil_audioprocessingunit.hpp"
	"include/autil_dspkernel.hpp"
	"include/autil_file.hpp"
	"include/autil_host.hpp"
	"include/autil_logger.hpp"
	"include/autil_manager.hpp"
	"include/autil_number.hpp"
	"include/autil_obj.hpp"
	"include/autil_osc.hpp"
	"include/autil_parameter.hpp"
	"include/autil_parametermap.hpp"
	"include/autil_stringlist.hpp"
	"include/autil_thread_manager.hpp"
	"include/autil_uiattrs.h"
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
