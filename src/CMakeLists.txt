
FILE(GLOB SRCS *.cpp *.h)
find_package(PythonLibs REQUIRED)
#include_directories("/usr/include/python2.7")
#target_link_libraries("/usr/lib/python2.7/config-x86_64-linux-gnu/")
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../build/include/)

link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../build/lib/)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../build/thirdparty/)
include_directories(${PYTHON_INCLUDE_DIRS})

add_executable(main ${SRCS})
target_link_libraries(main ${PYTHON_LIBRARIES} metis gptree metis)

add_library(engine SHARED ${SRCS} )
target_link_libraries(engine ${PYTHON_LIBRARIES} gptree)