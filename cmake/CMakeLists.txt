project(json2osc)
cmake_minimum_required(VERSION 2.6)

#######################################
if ( JSONONLY )
	message (STATUS "Generates json project - Use -DJSONONLY=0 to change.")
	add_definitions(-DJSON_ONLY)
	set (target json)
else()
	message (STATUS "Generates json2osc project - Use -DJSONONLY=1 to change.")
	set (target json2osc)
endif ()


#######################################
# target and versions management
set (VERSION 0.5)
set (STRVERS "v.0.5")
set (SOVERS 1)
set (STATIC "yes")

#######################################
# cmake build types configuration
set (CMAKE_CONFIGURATION_TYPES Debug Release)

if(WIN32)
	add_definitions(-DWINVER=0x0500)
	add_definitions(-D_CRT_SECURE_NO_DEPRECATE)
else(WIN32)
	add_definitions(-Wall -fPIC)
endif(WIN32)
add_definitions(-DJSONExport)

if(APPLE)

#######################################
#   iOS support
	if ( IOS STREQUAL "yes" )
		message (STATUS "Generates project for iOS - Use -DIOS=no to change.")
		set (CMAKE_XCODE_EFFECTIVE_PLATFORMS "-iphoneos;-iphonesimulator")
		set (CMAKE_OSX_ARCHITECTURES "$(ARCHS_STANDARD_32_BIT)")
		set (CMAKE_OSX_SYSROOT "iphoneos")
		set (lib "STATIC")
	else ()
		message (STATUS "Generates Mac OS project- Use -DIOS=yes to change.")
	endif ()
endif(APPLE)


#######################################
# SET MAIN DIRECTORY.
set(SRC ${CMAKE_CURRENT_SOURCE_DIR}/../src)


#######################################
# SET SOURCES AND HEADERS
if ( JSONONLY )
	FILE(GLOB JSON_SRC "${SRC}/json/*.cpp" "${SRC}/parser/*.cpp")
	FILE(GLOB JSON_H "${SRC}/json/*.h" "${SRC}/parser/*.h")
	FILE (GLOB PUBLICHEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "${SRC}/json/*.h" "${SRC}/parser/*.h")
else()
	FILE(GLOB JSON_SRC "${SRC}/*/*.cpp"*)
	FILE(GLOB JSON_H "${SRC}/*/*.h")
	FILE (GLOB PUBLICHEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "${SRC}/*/*.h")
	include_directories(${INCL} ${SRC}/json ${SRC}/osc ${SRC}/parser ${SRC}/include ${OSC_PATH})
endif()

include_directories(${INCL} ${SRC}/json ${SRC}/parser ${SRC}/include)

#######################################
# set libraries
if ( NOT JSONONLY )
	set (OSC_PATH  "${CMAKE_CURRENT_SOURCE_DIR}/../lib/oscpack")
	if(APPLE)
		set(OSCPACK  ${OSC_PATH}/cmake/Release/liboscpack.a)
	elseif(UNIX)
		set(OSCPACK  -L${OSC_PATH}/cmake -loscpack)
	elseif(WIN32)
		set(OSCPACK  ${OSC_PATH}/cmake/release/oscpack.lib)
	endif()
	include_directories(${INCL} ${SRC}/osc ${OSC_PATH})
endif()

#######################################
# target lib settings
if ( NOT IOS STREQUAL "yes" )
	if (STATIC STREQUAL "yes")
		message (STATUS "Generates a static library - Use -DSTATIC=no to change.")
		set (lib "STATIC")
		set (instlib "ARCHIVE")
	else ()
		message (STATUS "Generates a dynamic library - Use -DSTATIC=yes to change.")
		set (lib "SHARED")
		set (instlib "LIBRARY")
	endif ()
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
if ( NOT JSONONLY )
	add_executable( jsontest ${CMAKE_CURRENT_SOURCE_DIR}/../test/jsontest.cpp )
	target_link_libraries(jsontest ${target} ${OSCPACK} )
endif()

#######################################
# install setup
if (NOT WIN32 AND NOT IOS)
 if (DEFINED LIBINSTALL)
	set(LIBPATH ${LIBINSTALL})
 else (DEFINED LIBINSTALL)
	set(LIBPATH "/usr/local/lib")
 endif (DEFINED LIBINSTALL)
 message (STATUS "Library install path set to ${LIBPATH} - Use -DLIBINSTALL=path to change.")

install ( TARGETS ${target}
    	${instlib} DESTINATION ${LIBPATH}
    	PUBLIC_HEADER DESTINATION /usr/local/include/${target}
)
endif()
