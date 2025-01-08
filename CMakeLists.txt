cmake_minimum_required(VERSION 3.15)
project(video-player CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(FETCHCONTENT_BASE_DIR "${CMAKE_SOURCE_DIR}/third_party" CACHE PATH "Location for downloaded dependencies")


include(FetchContent)


FetchContent_Declare(
    wxwidgets
    GIT_REPOSITORY https://github.com/wxWidgets/wxWidgets.git
    GIT_TAG v3.2.4
)

# Configure wxWidgets build options
set(wxBUILD_SHARED OFF CACHE BOOL "Build wx libraries as shared libs")
set(wxBUILD_PRECOMP OFF CACHE BOOL "Use precompiled headers")
set(wxBUILD_TESTS OFF CACHE BOOL "Build tests")
set(wxBUILD_DEMOS OFF CACHE BOOL "Build demos")
set(wxBUILD_SAMPLES OFF CACHE BOOL "Build samples")
set(wxBUILD_INSTALL OFF CACHE BOOL "Install wxWidgets")

FetchContent_MakeAvailable(wxwidgets)

# Get and build Skia
FetchContent_Declare(
    skia
    GIT_REPOSITORY https://skia.googlesource.com/skia.git
    GIT_TAG chrome/m121
)

FetchContent_GetProperties(skia)
if(NOT skia_POPULATED)
    message(STATUS "Fetching Skia - this may take a few minutes...")
    FetchContent_Populate(skia)
    
    message(STATUS "Setting up Skia dependencies...")
    execute_process(
        COMMAND python3 tools/git-sync-deps
        WORKING_DIRECTORY ${skia_SOURCE_DIR}
        RESULT_VARIABLE DEPS_RESULT
        OUTPUT_VARIABLE DEPS_OUTPUT
        ERROR_VARIABLE DEPS_ERROR
    )
    
    message(STATUS "Fetching GN build tool...")
    execute_process(
        COMMAND bin/fetch-gn
        WORKING_DIRECTORY ${skia_SOURCE_DIR}
        RESULT_VARIABLE GN_RESULT
        OUTPUT_VARIABLE GN_OUTPUT
        ERROR_VARIABLE GN_ERROR
    )
    if(NOT GN_RESULT EQUAL 0)
        message(FATAL_ERROR "Failed to fetch GN: ${GN_ERROR}\nOutput: ${GN_OUTPUT}")
    endif()
    
    message(STATUS "Configuring Skia build...")
    execute_process(
        COMMAND ${skia_SOURCE_DIR}/bin/gn gen ${skia_SOURCE_DIR}/out/Release
        "--args=is_official_build=true is_debug=false target_os=\"mac\" target_cpu=\"arm64\" \
        skia_enable_pdf=false \
        skia_use_harfbuzz=true \
        skia_use_dng_sdk=false \
        skia_use_system_libjpeg_turbo=false \
        skia_use_system_libpng=false \
        skia_use_system_libwebp=false \
        skia_use_system_zlib=false \
        skia_enable_tools=false \
        skia_enable_skottie=false \
        skia_enable_gpu=true \
        skia_gl_standard=\"gl\" \
        skia_use_gl=true \
        skia_enable_sksl=true \
        skia_use_metal=true \
        skia_enable_ganesh=true \
        skia_enable_graphite=false \
        skia_enable_direct3d=false \
        skia_enable_vulkan=false \
        skia_use_angle=false \
        skia_use_dawn=false \
        skia_enable_svg=true \
        skia_enable_particles=true \
        skia_enable_stroke_geometry=true \
        skia_enable_pathops=true \
        skia_enable_gpu_debug_layers=false \
        cc=\"clang\" \
        cxx=\"clang++\""
        WORKING_DIRECTORY ${skia_SOURCE_DIR}
        RESULT_VARIABLE CONFIG_RESULT
        OUTPUT_VARIABLE CONFIG_OUTPUT
        ERROR_VARIABLE CONFIG_ERROR
    )
    
    if(NOT CONFIG_RESULT EQUAL 0)
        message(FATAL_ERROR "Failed to configure Skia:\nOutput: ${CONFIG_OUTPUT}\nError: ${CONFIG_ERROR}")
    endif()

    message(STATUS "Building Skia (this will take a while)...")
    execute_process(
        COMMAND ninja -C out/Release :skia
        WORKING_DIRECTORY ${skia_SOURCE_DIR}
        RESULT_VARIABLE BUILD_RESULT
        OUTPUT_VARIABLE BUILD_OUTPUT
        ERROR_VARIABLE BUILD_ERROR
    )
    
    if(NOT BUILD_RESULT EQUAL 0)
        message(FATAL_ERROR "Failed to build Skia:\nOutput: ${BUILD_OUTPUT}\nError: ${BUILD_ERROR}")
    endif()
endif()


add_executable(video-player
    src/utils/skia_utils.cpp
    src/skip_button.cpp
    src/main.cpp
    src/video_player.cpp
    src/progress_bar.cpp
    src/play_button.cpp
    src/pause_button.cpp
)

target_include_directories(video-player PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${skia_SOURCE_DIR}
)

target_link_libraries(video-player PRIVATE
    wx::core
    wx::base
    wx::gl
    ${skia_SOURCE_DIR}/out/Release/libskia.a
)

if(APPLE)
    target_link_libraries(video-player PRIVATE
        "-framework OpenGL"
        "-framework AppKit"
        "-framework CoreFoundation"
        "-framework CoreGraphics"
        "-framework CoreText"
        "-framework Metal"
        "-framework MetalKit"
        "-framework QuartzCore"
        "-framework Foundation"
    )
endif()