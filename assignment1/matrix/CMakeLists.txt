SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -pedantic")

#add_compile_options(-DMATRIX_DEBUG)
add_executable( MatrixTest MatrixTest.cpp)
add_executable( MatrixProduct MatrixProduct.cpp)
