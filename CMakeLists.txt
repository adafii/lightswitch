cmake_minimum_required(VERSION 3.18)
project("mqtt lightswitch" CXX)

find_package(PahoMqttCpp)

set(source_dir src)
set(client_dir ${source_dir}/client)

set(main_target lightswitch)
set(main_src ${source_dir}/main.cpp)
set(client_src
        ${client_dir}/Client.cpp
        ${client_dir}/ActionListener.cpp
        ${client_dir}/Callback.cpp
        ${client_dir}/ConnectionException.cpp)

add_executable(${main_target} ${main_src} ${client_src})
target_include_directories(${main_target} PRIVATE ${source_dir})
target_link_libraries(${main_target} PahoMqttCpp::paho-mqttpp3)

target_compile_features(${main_target} PRIVATE cxx_std_20)
target_compile_options(${main_target} PRIVATE $<$<CONFIG:Debug>:-Wall -Wextra -Wpedantic -fsanitize=address -fsanitize=undefined>)
target_compile_options(${main_target} PRIVATE $<$<OR:$<CONFIG:Release>,$<CONFIG:MinSizeRel>>:-Wall -Wextra -Wpedantic -flto -fpermissive>)
target_link_options(${main_target} PRIVATE $<$<CONFIG:Debug>:-fsanitize=address -fsanitize=undefined>)
target_link_options(${main_target} PRIVATE $<$<OR:$<CONFIG:Release>,$<CONFIG:MinSizeRel>>:-flto>)
