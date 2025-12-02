project(MachineTests)

set(TEST_FILES
    gtest_main.cpp
    MachineTest.cpp)

# Include the MachineLib source directory to support testing of any classes there
include_directories("../${MACHINE_LIBRARY}")

# Get Google Tests
include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.11.0
)

#
# Use Box2D
#
include(FetchContent)
FetchContent_Declare(
        box2d
        GIT_REPOSITORY https://github.com/erincatto/box2d.git
        GIT_TAG v2.4.1
)

FetchContent_MakeAvailable(box2d)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Include directories we need for Google Test
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

# adding the Tests_run target
add_executable(${PROJECT_NAME}_run ${TEST_FILES})

# linking Tests_run with library which will be tested and wxWidgets
target_link_libraries(${PROJECT_NAME}_run ${MACHINE_LIBRARY} ${wxWidgets_LIBRARIES})

# linking Tests_run with the Google Test libraries
target_link_libraries(${PROJECT_NAME}_run gtest)

target_precompile_headers(${PROJECT_NAME}_run PRIVATE "../${MACHINE_LIBRARY}/pch.h")
