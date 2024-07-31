add_executable(testprog testit.cpp)

target_include_directories(testprog PUBLIC ../include)

add_executable(invoke invokeable_test.cpp)

add_executable(poly poly.cpp)
