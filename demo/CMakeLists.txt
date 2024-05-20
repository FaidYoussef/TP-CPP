add_executable(exempleUnivers exempleUnivers.cxx)
add_executable(collision collision.cxx)
# add_executable(cellTest cellTest.cpp)

# add_executable(absorptionTest absorptionTest.cpp)

target_link_libraries(exempleUnivers Univers)
target_link_libraries(collision Univers)

# target_link_libraries(cellTest Univers)

# target_link_libraries(absorptionTest Univers)


