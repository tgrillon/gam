add_executable(TMesh_utest TMesh_utest.cpp)
target_link_libraries(TMesh_utest PRIVATE gam_static)

AddTests(TMesh_utest)
