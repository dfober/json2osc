project(json2osc)
cmake_minimum_required(VERSION 2.6)

#######################################
# target and versions management
set (target json2osc)
set (VERSION 0.5)
set (STRVERS "v.0.5")
set (SOVERS 1)

#######################################
# cmake build types configuration
set (CMAKE_CONFIGURATION_TYPES Debug Release)

if(WIN32)
	add_definitions(-DWINVER=0x0500)
	add_definitions(-D_CRT_SECURE_NO_DEPRECATE)
else(WIN32)
	add_definitions(-Wall -fPIC)
endif(WIN32)

if(APPLE)
	set (CMAKE_OSX_ARCHITECTURES "x86_64 i386")
	if (${CMAKE_SYSTEM_VERSION} VERSION_LESS 9.0.0)
		set (CMAKE_OSX_SYSROOT /Developer/SDKs/MacOSX10.4u.sdk)
	elseif (${CMAKE_SYSTEM_VERSION} VERSION_LESS 10.8.0)
		set (CMAKE_OSX_SYSROOT /Developer/SDKs/MacOSX10.5.sdk)
	elseif (${XCODE_VERSION} VERSION_LESS 4.0.0)
		set (CMAKE_OSX_SYSROOT /Developer/SDKs/MacOSX10.6.sdk)
	else ()
		set (CMAKE_OSX_SYSROOT "macosx10.7")
	endif ()
	set(CMAKE_C++_FLAGS -mmacosx-version-min=10.4)
endif(APPLE)

#######################################
# SET MAIN DIRECTORY.
set(SRC ${CMAKE_CURRENT_SOURCE_DIR}/../src)
set (OSC_PATH  "${CMAKE_CURRENT_SOURCE_DIR}/../lib/oscpack")

#######################################
# SET SOURCES
FILE(GLOB JSON_SRC "${SRC}/*/*.cpp")
#######################################
# SET HEADERS
FILE(GLOB JSON_H "${SRC}/*/*.h")

#######################################
# set libraries
if(APPLE)
	set(OSCPACK  ${OSC_PATH}/cmake/Release/liboscpack.a)
elseif(UNIX)
	set(OSCPACK  -L${OSC_PATH}/cmake -loscpack)
elseif(WIN32)
	set(OSCPACK  ${OSC_PATH}/cmake/release/oscpack.lib)
endif()

#######################################
include_directories(${INCL} ${SRC}/json ${SRC}/osc ${SRC}/parser ${SRC}/include ${OSC_PATH})

#######################################
# target lib settings
if (STATIC STREQUAL "yes")
	message (STATUS "Generates a static library - Use -DSTATIC=no to change.")
	set (lib "STATIC")
else ()
	message (STATUS "Generates a dynamic library - Use -DSTATIC=yes to change.")
	set (lib "SHARED")
endif ()

add_library(${target} ${lib} ${JSON_SRC} ${JSON_H} )
set_target_properties (${target} PROPERTIES
			PUBLIC_HEADER "${PUBLICHEADERS}"
			VERSION ${VERSION}
			SOVERSION ${SOVERS})
target_link_libraries(${target} ${OSCPACK} )

if (FRAMEWORK STREQUAL "yes")
	set_target_properties (${target} PROPERTIES 
			FRAMEWORK TRUE
			MACOSX_FRAMEWORK_SHORT_VERSION_STRING ${STRVERS}
			MACOSX_FRAMEWORK_BUNDLE_VERSION ${VERSION}
			MACOSX_FRAMEWORK_BUNDLE_VERSION "A")
endif()

#######################################
# test utility settings
add_executable( jsontest ${CMAKE_CURRENT_SOURCE_DIR}/../test/jsontest.cpp )
target_link_libraries(jsontest ${target} ${OSCPACK} )

