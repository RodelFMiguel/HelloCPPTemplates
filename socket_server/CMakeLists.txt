add_library(SERVER server.cpp)
target_link_libraries(SERVER -pthread)

add_executable (server_app server_sample.cpp)
target_link_libraries (server_app SERVER -pthread)