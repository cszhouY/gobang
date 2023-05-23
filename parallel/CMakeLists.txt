file(GLOB SOURCES *.cpp)

add_executable(gobang2 ${SOURCES})

find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    target_link_libraries(gobang2 PUBLIC OpenMP::OpenMP_CXX)
endif()