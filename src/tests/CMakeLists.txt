enable_testing()

include(GoogleTest)

add_executable(progress_bar_test
    progress_bar_tests.cpp
    ${CMAKE_SOURCE_DIR}/src/progress_bar.cpp
    ${CMAKE_SOURCE_DIR}/src/utils/skia_utils.cpp
)

target_include_directories(progress_bar_test PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
    ${skia_SOURCE_DIR}
    ${skia_SOURCE_DIR}/tools
    ${skia_SOURCE_DIR}/include/gpu
)

target_link_libraries(progress_bar_test PRIVATE
    GTest::gtest_main
    wx::core
    wx::base
    wx::gl
    ${skia_SOURCE_DIR}/out/Release/libskia.a
)

if(APPLE)
    target_link_libraries(progress_bar_test PRIVATE
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

gtest_discover_tests(progress_bar_test)
