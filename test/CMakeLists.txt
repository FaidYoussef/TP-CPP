# executable de exemple_univers
add_executable(exempleUnivers exempleUnivers.cxx)
# add_executable(gtest gtest.cxx)

# Include the src directory for header files
include_directories(${CMAKE_SOURCE_DIR}/src)

# Fait le lien avec la bibliothèque créée précedemment
# Les entêtes sont automatiquement détectés.
target_link_libraries(exempleUnivers Cellule)
target_link_libraries(exempleUnivers Particule3D)
target_link_libraries(exempleUnivers Univers)
target_link_libraries(exempleUnivers Vector3D)



# include(GoogleTest)
# gtest_discover_tests(gtest)