#
# FindPNG.cmake
#

find_package(ZLIB)

if (WIN32)

	set(WIN_LIB_PATH ${CMAKE_SOURCE_DIR}/ext)
	if (MSVC)
		set(WIN_LIB_PATH "${WIN_LIB_PATH}/msvc")
	else()
		set(WIN_LIB_PATH "${WIN_LIB_PATH}/mingw")
	endif()
	if (${CMAKE_SIZEOF_VOID_P} EQUAL 8)
		set (WIN_LIB_PATH "${WIN_LIB_PATH}64")
	endif()

	if (CMAKE_BUILD_TYPE MATCHES "Debug")
		set (WIN_DEBUG "d")
	endif()

	set (PNG_LIBRARIES "${WIN_LIB_PATH}/lib/libpng16${WIN_DEBUG}.lib")
	set (PNG_INCLUDE_DIRS "${WIN_LIB_PATH}/include")
	set (PNG_RUNTIME "${WIN_LIB_PATH}/bin/libpng16${WIN_DEBUG}.dll")

else()

	set(PNG_SEARCH_PATH
		/usr
		/usr/local
		~/.brew
	)

	find_library(
		PNG_LIBRARIES
		libpng.a
		PATHS
		${PNG_SEARCH_PATH}
		PATH_SUFFIXES lib lib64
	)

	find_path(
	 	PNG_INCLUDE_DIRS
	 	png.h
	 	PATHS
	 	${PNG_SEARCH_PATH}
	 	PATH_SUFFIXES include
	 	NO_DEFAULT_PATH
	)

endif()

message(STATUS "Found libpng: ${PNG_LIBRARIES}")

list(APPEND PNG_LIBRARIES ${ZLIB_LIBRARIES})

