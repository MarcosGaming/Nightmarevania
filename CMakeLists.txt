cmake_minimum_required(VERSION 3.11)
# Require modern C++
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(Nightmarevania)

#### Setup Directories ####
#Main output directory

SET(OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/")
# Ouput all DLLs from all libs into main build folder
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIRECTORY})
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_DIRECTORY})

#Resources
add_custom_target(copy_resources ALL COMMAND ${CMAKE_COMMAND} 
  -E copy_directory
    "${PROJECT_SOURCE_DIR}/res"
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIGURATION>/res
)

#### Add External Dependencies ####
add_subdirectory("lib/sfml")
set(SFML_INCS "lib/sfml/include")
link_directories("${CMAKE_BINARY_DIR}/lib/sfml/lib")

### BOX 2D PHYSICS LIBRARY ###
add_subdirectory("lib/b2d/Box2D")
set(B2D_INCS "lib/b2d/Box2D/")
#link_directories("${CMAKE_BINARY_DIR}/lib/sfml/lib")


### NIGHTMAREVANIA ###
file(GLOB_RECURSE SOURCES Nightmarevania/*.cpp Nightmarevania/*.h)
file(GLOB_RECURSE CMPNTS Nightmarevania/components/*.cpp Nightmarevania/components/*.h)
file(GLOB_RECURSE SCENES Nightmarevania/scenes/*.cpp Nightmarevania/scenes/*.h)
add_executable(NIGHTMAREVANIA ${SOURCES} ${CMPNTS} ${SCENES} ${RESOURCE_FILES})
source_group("components" FILES ${CMPNTS})
source_group("resources" FILES ${RESOURCE_FILES})
source_group("scenes" FILES ${SCENES})
target_include_directories(NIGHTMAREVANIA SYSTEM PRIVATE ${SFML_INCS} ${B2D_INCS})
set(EXECUTABLES ${EXECUTABLES} NIGHTMAREVANIA)



#### LINK DEPENDENCIES ####
foreach(exe ${EXECUTABLES})
	#Set working directory to build dir in visual studio
	set_target_properties(${exe} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$(Configuration))
	#Add all SFML libs 
	target_link_libraries(${exe}
		optimized sfml-graphics debug sfml-graphics-d
		optimized sfml-audio debug sfml-audio-d
		optimized sfml-window debug sfml-window-d
		optimized sfml-system debug sfml-system-d
		optimized sfml-graphics debug sfml-graphics-d
		optimized sfml-main debug sfml-main-d
		lib_tile_level_loader
		lib_ecm
		Box2D
		lib_engine)
	add_dependencies(${exe} sfml-graphics sfml-audio copy_resources)
endforeach ()

### TILE LOADER LIBRARY ###
file(GLOB_RECURSE SOURCE_FILES lib_tile_level_loader/*.cpp lib_tile_level_loader/*.h)
add_library(lib_tile_level_loader STATIC ${SOURCE_FILES})
target_include_directories(lib_tile_level_loader INTERFACE "${CMAKE_SOURCE_DIR}/lib_tile_level_loader/")
target_link_libraries(lib_tile_level_loader lib_maths sfml-graphics)

### MATHS LIBRARY ###
add_library(lib_maths INTERFACE)
target_sources(lib_maths INTERFACE "${CMAKE_SOURCE_DIR}/lib_maths/maths.h")
target_include_directories(lib_maths INTERFACE "${CMAKE_SOURCE_DIR}/lib_maths" SYSTEM INTERFACE "${SFML_INCS}")

### ENGINE LIBRARY ###
file(GLOB_RECURSE SOURCE_FILES engine/*.cpp engine/*.h)
add_library(lib_engine STATIC ${SOURCE_FILES} ${RESOURCE_FILES})
target_include_directories(lib_engine INTERFACE "${CMAKE_SOURCE_DIR}/engine/" PUBLIC SYSTEM ${B2D_INCS} )
target_link_libraries(lib_engine lib_tile_level_loader lib_maths lib_ecm Box2D sfml-graphics sfml-audio)

### ECM LIBRARY ###
file(GLOB_RECURSE SOURCE_FILES lib_ecm/*.cpp lib_ecm/*.h)
add_library(lib_ecm STATIC ${SOURCE_FILES})
target_include_directories(lib_ecm INTERFACE "${CMAKE_SOURCE_DIR}/lib_ecm")
target_link_libraries(lib_ecm PRIVATE lib_maths)

