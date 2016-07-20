cmake_minimum_required(VERSION 2.4.6)

project(michigan)

if(COMMAND cmake_policy)
	cmake_policy(SET CMP0003 NEW)
	cmake_policy(SET CMP0015 NEW)
endif(COMMAND cmake_policy)


#set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build/bin)
#set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build/lib)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -ggdb -Wextra -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -Werror-implicit-function-declaration")
SET(CMAKE_CXX_FLAGS "-std=c++0x -g -ggdb")

add_executable (test-queues
    src/queue/Queue.h
    src/queue/Fragmentation.h
    src/queue/Compression.h
    src/queue/PriorityQueue.h
    src/queue/Multiqueue.h
    src/test/main-queue.cpp
    src/rt-wmp/State.h
)
target_link_libraries(test-queues z pthread rt pthread ${Boost_LIBRARIES}  ${catkin_LIBRARIES} )

add_executable (test-graph
    src/test/main-graph.cpp
    src/rt-wmp/State.h
    src/graph/Matrix.h
    src/network/layer-1.h
    src/rt-wmp/Frame.h
)
target_link_libraries(test-graph z pthread rt pthread ${Boost_LIBRARIES}  ${catkin_LIBRARIES} )

add_executable (rt-wmp
    src/rt-wmp/main.cpp
    src/rt-wmp/State.h
    src/graph/Matrix.h
    src/network/layer-1.h
    src/rt-wmp/Frame.h
    src/rt-wmp/Network.h
    src/rt-wmp/PAP.h
    src/rt-wmp/ATP.h
    src/rt-wmp/MTP.h
)
target_link_libraries(rt-wmp z yaml-cpp pthread rt pthread ${Boost_LIBRARIES}  ${catkin_LIBRARIES} )

add_executable (sniffer
    src/rt-wmp/sniffer.cpp
    src/rt-wmp/State.h
    src/graph/Matrix.h
    src/network/layer-1.h
    src/rt-wmp/Frame.h
    src/rt-wmp/Network.h
)
target_link_libraries(sniffer z yaml-cpp pthread rt pthread ${Boost_LIBRARIES}  ${catkin_LIBRARIES} )


add_custom_command(TARGET rt-wmp
        POST_BUILD
        COMMAND echo "Applying sticky bit to the executable..."
        COMMAND echo ${CMAKE_BINARY_DIR}/../build/michigan/rt-wmp
        COMMAND sudo chmod a+rwx ${CMAKE_BINARY_DIR}/../build/michigan/rt-wmp
        COMMAND sudo chown root:root ${CMAKE_BINARY_DIR}/../build/michigan/rt-wmp
        COMMAND sudo chmod +s ${CMAKE_BINARY_DIR}/../build/michigan/rt-wmp
)
add_custom_command(TARGET sniffer
        POST_BUILD
        COMMAND echo "Applying sticky bit to the executable..."
        COMMAND echo ${CMAKE_BINARY_DIR}/../build/michigan/sniffer
        COMMAND sudo chmod a+rwx ${CMAKE_BINARY_DIR}/../build/michigan/sniffer
        COMMAND sudo chown root:root ${CMAKE_BINARY_DIR}/../build/michigan/sniffer
        COMMAND sudo chmod +s ${CMAKE_BINARY_DIR}/../build/michigan/sniffer
)
