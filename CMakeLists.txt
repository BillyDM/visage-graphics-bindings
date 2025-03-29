cmake_minimum_required(VERSION 3.17)
project(VisageGraphicsC VERSION 0.1.1)

if (NOT DEFINED CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
endif ()

option(VISAGE_GRAPHICS_C_BUILD_TEST_APP "Build the the C Test App example" OFF)
option(VISAGE_ENABLE_BACKGROUND_GRAPHICS_THREAD "Offloads graphics rendering to a background thread" OFF)
option(VISAGE_ENABLE_GRAPHICS_DEBUG_LOGGING "Shows graphics debug log in console in debug mode" OFF)

add_subdirectory(visage-graphics-c)

if (VISAGE_C_BUILD_TEST_APP)
    add_subdirectory(c_test_app)
endif ()

