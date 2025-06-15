cmake_minimum_required(VERSION 3.31)
project(MQTTAlarms)

set(CMAKE_CXX_STANDARD 20)

include_directories(/usr/local/include)
add_executable(MQTTAlarms main.cpp)

target_link_libraries(MQTTAlarms
        paho-mqttpp3
        paho-mqtt3as
)
link_directories(/usr/local/lib)