cmake_minimum_required(VERSION 3.21)

# info
project(
	PluginTemplate
	VERSION 1.0.1
	LANGUAGES CXX
)

# standards & flags
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
include(CheckIPOSupported)
check_ipo_supported(RESULT USE_IPO OUTPUT IPO_OUTPUT)
if (USE_IPO)
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
else ()
    message(WARNING "Interprocedural optimizations are not supported.")
endif ()
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_STATIC_RUNTIME ON)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# out-of-source builds only
if(${PROJECT_SOURCE_DIR} STREQUAL ${PROJECT_BINARY_DIR})
	message(FATAL_ERROR "In-source builds are not allowed.")
endif()

# in-place configuration
configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/cmake/Version.h.in
	${CMAKE_CURRENT_BINARY_DIR}/Version.h
	@ONLY
)

configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/cmake/version.rc.in
	${CMAKE_CURRENT_BINARY_DIR}/version.rc
	@ONLY
)

# dependencies
find_package(CommonLibSSE CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)

# source files
execute_process(COMMAND powershell -ExecutionPolicy Bypass -File "${CMAKE_CURRENT_SOURCE_DIR}/!Update.ps1" "SOURCEGEN" "${PROJECT_VERSION}" "${CMAKE_CURRENT_BINARY_DIR}")
include(${CMAKE_CURRENT_BINARY_DIR}/sourcelist.cmake)
source_group(
	TREE 
		${CMAKE_CURRENT_SOURCE_DIR}
	FILES 
		${SOURCES}
		${CMAKE_CURRENT_BINARY_DIR}/Version.h
)

# cmake target
add_commonlibsse_plugin(
	${PROJECT_NAME}
	AUTHOR
		$ENV{SKSEPluginAuthor}
	SOURCES
		${SOURCES}
		${CMAKE_CURRENT_BINARY_DIR}/Version.h
		${CMAKE_CURRENT_BINARY_DIR}/version.rc
)

# compiler def
if (MSVC)
	add_compile_definitions(_UNICODE)

	target_compile_options(
		${PROJECT_NAME}
		PRIVATE
			/MP
			/await
			/W0
			/WX
			/permissive-
			/Zc:alignedNew
			/Zc:auto
			/Zc:__cplusplus
			/Zc:externC
			/Zc:externConstexpr
			/Zc:forScope
			/Zc:hiddenFriend
			/Zc:implicitNoexcept
			/Zc:lambda
			/Zc:noexceptTypes
			/Zc:preprocessor
			/Zc:referenceBinding
			/Zc:rvalueCast
			/Zc:sizedDealloc
			/Zc:strictStrings
			/Zc:ternary
			/Zc:threadSafeInit
			/Zc:trigraphs
			/Zc:wchar_t
			/wd4200 # nonstandard extension used : zero-sized array in struct/union
	)
endif()

# PCH
target_precompile_headers(
	${PROJECT_NAME}
	PRIVATE
		src/PCH.h
)

# include dir
target_include_directories(
	${PROJECT_NAME}
	PRIVATE
		$<INSTALL_INTERFACE:src>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
)

# linkage
target_link_libraries(
	${PROJECT_NAME} 
	PRIVATE
		spdlog::spdlog
)

# called via !Rebuild.ps1
if(DEFINED GROUP)
set_target_properties(
	${PROJECT_NAME}
	PROPERTIES
	FOLDER
	${GROUP}
)
endif()
