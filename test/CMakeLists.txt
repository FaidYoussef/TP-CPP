
# Include the src directory for header files
include_directories(${CMAKE_SOURCE_DIR}/src)

# Fait le lien avec la bibliothèque créée précedemment
# Les entêtes sont automatiquement détectés.

add_executable(testToto testToto.cxx)
add_executable(CelluleTests CelluleTests.cxx)
add_executable(Vector3DTests Vector3DTests.cxx)
add_executable(ParticuleTests ParticuleTests.cxx)

# Link with the library
target_link_libraries(
        CelluleTests
        Univers
)

target_link_libraries(
        ParticuleTests
        Univers
)

target_link_libraries(
        Vector3DTests
        Vector3D
)

target_link_libraries(
        testToto
        gtest_main
)

target_link_libraries(
        Vector3DTests
        gtest_main
)

target_link_libraries(
        CelluleTests
        gtest_main
)

target_link_libraries(
        ParticuleTests
        gtest_main
)

include(GoogleTest)
gtest_discover_tests(testToto)
gtest_discover_tests(CelluleTests)
gtest_discover_tests(Vector3DTests)
gtest_discover_tests(ParticuleTests)