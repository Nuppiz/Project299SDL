cmake_minimum_required(VERSION 3.16)

# set the output directory for built objects.
# This makes sure that the dynamic library goes into the build directory automatically.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

project(Project299)

set(SDL_SHARED ON)

if(MSVC)
    if(NOT CMAKE_GENERATOR STREQUAL "Ninja")
        add_definitions(/MP)				# parallelize each target, unless Ninja is the generator
    endif()
endif()

# Create an executable or a shared library based on the platform and add our sources to it
set(EXECUTABLE_NAME ${PROJECT_NAME})
add_executable(${EXECUTABLE_NAME})

target_sources(${EXECUTABLE_NAME} 
PRIVATE 
    src/Main.cpp
    src/Draw.cpp
    src/Vectors.cpp
    src/Video.cpp
)
    
# Set C++ version
target_compile_features(${EXECUTABLE_NAME} PUBLIC cxx_std_17)

# Set paths to SDL libraries and include them in the project
set(SDL3_DIR D:/Dev/SDL3/cmake)
set(SDL3_image_DIR D:/Dev/SDL3_image/cmake)
set(SDL3_ttf_DIR D:/Dev/SDL3_ttf/cmake)
set(SDL2_mixer_DIR D:/Dev/SDL2_mixer/cmake)
find_package(SDL3 REQUIRED)
find_package(SDL3_image REQUIRED)
find_package(SDL3_ttf REQUIRED)
find_package(SDL2_mixer REQUIRED)

# Link SDL to our executable. This also makes its include directory available to us. 
target_link_libraries(${EXECUTABLE_NAME} PUBLIC 
	SDL3_ttf::SDL3_ttf      # remove if you are not using SDL_ttf
	SDL2_mixer::SDL2_mixer  # remove if you are not using SDL_mixer
	SDL3_image::SDL3_image	# remove if you are not using SDL_image
    SDL3::SDL3              # If using satelite libraries, SDL must be the last item in the list. 
)

# on Visual Studio, set our app as the default project
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT "${EXECUTABLE_NAME}")