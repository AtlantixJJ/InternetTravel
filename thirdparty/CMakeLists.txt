#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
set(BUILD_PROGRAM make)
# add_dependencies(rk build_routingkit)

#set(CMAKE_C_COMPILER /usr/local/bin/mpicc)
#set(CMAKE_CXX_COMPILER /usr/local/bin/mpicxx)
#set(METIS_LIBRARY ${CMAKE_CURRENT_SOURCE_DIR}/build/lib/libmetis.a)
#set(CONFIG_PROGRAM make config prefix=../../../../build/)
#set(BUILD_PROGRAM make -i install)
#add_custom_target(config_metis ALL
#        COMMAND ${CONFIG_PROGRAM}
#        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/metis/
#        COMMENT "Original metis makefile target")
#add_custom_target(build_metis ALL
#        COMMAND ${BUILD_PROGRAM}
#        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/metis/
#        COMMENT "Original metis makefile target")
#add_library(metis SHARED IMPORTED)
#set_property(TARGET metis APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
#set_target_properties(metis PROPERTIES IMPORTED_LOCATION_NOCONFIG "${METIS_LIBRARY}")
#add_dependencies(metis config_metis)
#add_dependencies(metis build_metis)

# Build METIS lib
# execute_process (COMMAND bash -c "cd ../thirdparty/metis && make config prefix=../../../../build/ && make install")

# Build GPTree
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../build/include/)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../build/lib/)
#add_executable(GPTree ${CMAKE_CURRENT_SOURCE_DIR}/GTree/src/gtree_new_p2p/GPTree.cpp)
#target_link_libraries(GPTree metis)
add_library(gptree SHARED ${CMAKE_CURRENT_SOURCE_DIR}/lib/GPTree.cpp)
target_link_libraries(gptree metis)

#execute_process (COMMAND g++ -std=c++0x -O2 ../thirdparty/GTree/src/gtree_new_p2p/GPTree.cpp -L/usr/local/lib/ -L./lib/ -I./include/ -lmetis -o GPTree)
#execute_process (COMMAND g++ -std=c++0x -O2 -shared -fPIC ../thirdparty/lib/GPTree.cpp -L/usr/local/lib/ -L./lib/ -I./include/ -lmetis -o lib/libgptree.so)

#execute_process (COMMAND g++ -std=c++0x -O2 ../thirdparty/lib/GPTree.cpp -L/usr/local/lib/ -L./lib/ -I./include/ -lmetis -c -o libgptree.o)
#execute_process (COMMAND ar -cr lib/libgptree.a libgptree.o lib/libmetis.a)
