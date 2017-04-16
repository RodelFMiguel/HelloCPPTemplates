add_library(CLIENT client.cpp)
target_link_libraries(CLIENT -pthread)

add_executable (client_app client_sample.cpp)
target_link_libraries (client_app CLIENT -pthread)