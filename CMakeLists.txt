cmake_minimum_required(VERSION 2.6)

project(log4boost CXX)

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/tools/cmake/modules)
message(STATUS CMAKE_MODULE_PATH: ${CMAKE_MODULE_PATH})

if(UNIX)
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -fpermissive" )
endif(UNIX)

if(WIN32)
	option(BUILD_STATIC_RUNTIME "Use static runtime lib or dll." OFF)
endif(WIN32)

option(BUILD_STATIC_LIB "Build static logging lib or shared logging lib." OFF)
option(BUILD_OUTPUT_NAME_WITH_CONFIG_SUFFIX "The output file name with suffix by config." OFF)
option(BUILD_LOGGING_TEST ""	OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin CACHE PATH "Output directory in which to build RUNTIME target files.")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib CACHE PATH "Output directory in which to build ARCHIVE target files.")
if(NOT WIN32)
	option(CMAKE_COMPILER_IS_GNUCC "Used by FindBoost module to guess the lib name"    ON)
endif(NOT WIN32)


if(WIN32)
	if(MSVC)
		add_definitions(/D_SCL_SECURE_NO_WARNINGS /D_CRT_SECURE_NO_WARNINGS /wd4819)
	endif(MSVC)
	
    if( NOT BUILD_STATIC_LIB )
        add_definitions(-DLOG4BOOST_SHARED)
    endif( NOT BUILD_STATIC_LIB )

	if(BUILD_STATIC_RUNTIME)
		# Debug
		STRING(REPLACE "/MDd" "" CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG}")
		SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} /MTd")
		STRING(REPLACE "/MDd" "" CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
		SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")

		# Release
		STRING(REPLACE "/MD" "" CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE}")
		SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /MT")
		STRING(REPLACE "/MD" "" CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
		SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")

		# RelWithDebInfo
		STRING(REPLACE "/MD" "" CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_RELWITHDEBINFO}")
		SET(CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_RELWITHDEBINFO} /MT")
		STRING(REPLACE "/MD" "" CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")
		SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} /MT")

		# MinSizeRel
		STRING(REPLACE "/MD" "" CMAKE_C_FLAGS_MINSIZEREL "${CMAKE_C_FLAGS_MINSIZEREL}")
		SET(CMAKE_C_FLAGS_MINSIZEREL "${CMAKE_C_FLAGS_MINSIZEREL} /MT")
		STRING(REPLACE "/MD" "" CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_MINSIZEREL}")
		SET(CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_MINSIZEREL} /MT")
	else(BUILD_STATIC_RUNTIME)
		# Debug
		STRING(REPLACE "/MTd" "" CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG}")
		SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} /MDd")
		STRING(REPLACE "/MTd" "" CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
		SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd")

		# Release
		STRING(REPLACE "/MT" "" CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE}")
		SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /MD")
		STRING(REPLACE "/MT" "" CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
		SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD")

		# RelWithDebInfo
		STRING(REPLACE "/MT" "" CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_RELWITHDEBINFO}")
		SET(CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_RELWITHDEBINFO} /MD")
		STRING(REPLACE "/MD" "" CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")
		SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} /MD")

		# MinSizeRel
		STRING(REPLACE "/MT" "" CMAKE_C_FLAGS_MINSIZEREL "${CMAKE_C_FLAGS_MINSIZEREL}")
		SET(CMAKE_C_FLAGS_MINSIZEREL "${CMAKE_C_FLAGS_MINSIZEREL} /MD")
		STRING(REPLACE "/MT" "" CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_MINSIZEREL}")
		SET(CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_MINSIZEREL} /MD")
	endif(BUILD_STATIC_RUNTIME)
endif(WIN32)

set(Boost_USE_MULTITHREAD ON)
if(WIN32)
    set(Boost_USE_STATIC_LIBS ON)
endif(WIN32)
#add_definitions(/DBOOST_ALL_DYN_LINK)

#set(BOOST_ROOT "" CACHE PATH "Set this to the include directory of Boost, if the module has problems finding the proper Boost installation")
#set(Boost_LIBRARYDIR "" CACHE PATH "Set this to the lib directory of Boost, if the module has problems finding the proper Boost installation")
set(Boost_VERSION_USE "1.51" CACHE STRING "The version of boost library should used.")

message(STATUS "BOOST_ROOT: " ${BOOST_ROOT})
message(STATUS "Boost_LIBRARYDIR: " ${Boost_LIBRARYDIR})
message(STATUS "Boost_VERSION_USE: " ${Boost_VERSION_USE})

find_package( Boost ${Boost_VERSION_USE} REQUIRED
    COMPONENTS 
        chrono regex system program_options signals date_time filesystem thread
    )
message(STATUS ${Boost_FOUND})
message(STATUS ${Boost_INCLUDE_DIRS})
message(STATUS ${Boost_LIBRARIES})
if(Boost_FOUND)
    include_directories(${})
endif(Boost_FOUND)

function(link_boost target_name)
if(Boost_FOUND AND NOT WIN32)
    set(components_list ${ARGV})
    list(REMOVE_AT components_list 0)
    foreach(component ${components_list})
        string(TOUPPER ${component} component_name)
        if(Boost_${component_name}_FOUND)
		    target_link_libraries(${target_name}
		        debug ${Boost_${component_name}_LIBRARY_DEBUG}
		        optimized ${Boost_${component_name}_LIBRARY_RELEASE}
		    )
        endif(Boost_${component_name}_FOUND)
    endforeach(component)
endif(Boost_FOUND AND NOT WIN32)
endfunction(link_boost)

include_directories(${BOOST_ROOT})
link_directories(${Boost_LIBRARYDIR})

add_subdirectory(src)

if(BUILD_LOGGING_TEST)
	add_subdirectory(test)
endif(BUILD_LOGGING_TEST)

# Use Doxygen to generate the documents.
find_package(Doxygen)
if(DOXYGEN_FOUND)
	add_custom_target(HTMLRef ${DOXYGEN_EXECUTABLE} ${PROJECT_SOURCE_DIR}/doc/Doxyfile
    	DEPENDS  ${logging_src}	${logging_header}
		WORKING_DIRECTORY	${PROJECT_SOURCE_DIR}/doc
		COMMENT	"Generate the reference documents using doxygen."
    )
    # Prepair the config file for Doxygen
	configure_file(${PROJECT_SOURCE_DIR}/doc/Doxyfile.in 
		${PROJECT_SOURCE_DIR}/doc/Doxyfile
		)

	find_package(HTMLHelp)

	if(HTML_HELP_COMPILER)
		message(STATUS ${HTML_HELP_COMPILER})
	endif(HTML_HELP_COMPILER)

endif(DOXYGEN_FOUND)



SET(CPACK_PACKAGE_VERSION_MAJOR "0")
SET(CPACK_PACKAGE_VERSION_MINOR "1")
SET(CPACK_PACKAGE_VERSION_PATCH "0")

set(CPACK_GENERATOR ZIP STGZ TBZ2)
INCLUDE(CPack)





