# executable de exemple_univers
#add_executable(exempleUnivers exempleUnivers.cxx)
# add_executable(gtest gtest.cxx)

# Include the src directory for header files
include_directories(${CMAKE_SOURCE_DIR}/src)

# Fait le lien avec la bibliothèque créée précedemment
# Les entêtes sont automatiquement détectés.

add_executable(testToto testToto.cxx)

target_link_libraries(
        testToto
        gtest_main
)

include(GoogleTest)
gtest_discover_tests(testToto)