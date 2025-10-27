cmake_minimum_required(VERSION 3.28)

project(chat-client
        VERSION 1.0
        LANGUAGES CXX
)

include(FetchContent)
set(FETCHCONTENT_TRY_FIND_PACKAGE_MODE NEVER)

FetchContent_Declare(
        zmq
        GIT_REPOSITORY https://github.com/zeromq/libzmq.git
        GIT_TAG "master"
        # GIT_TAG v4.3.5
        OVERRIDE_FIND_PACKAGE
)
FetchContent_MakeAvailable(zmq)

FetchContent_Declare(
        cppzmq
        GIT_REPOSITORY https://github.com/zeromq/cppzmq.git
        #        GIT_TAG v4.11.0
        GIT_TAG "master"
        OVERRIDE_FIND_PACKAGE
)
FetchContent_MakeAvailable(cppzmq)

FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 3.0.2
        GIT_SHALLOW ON
        EXCLUDE_FROM_ALL
        SYSTEM)
FetchContent_MakeAvailable(SFML)

FetchContent_Declare(ImGui
        GIT_REPOSITORY https://github.com/ocornut/imgui
        GIT_TAG v1.91.1
        GIT_SHALLOW ON
        EXCLUDE_FROM_ALL
        SYSTEM)
FetchContent_MakeAvailable(ImGui)
FetchContent_GetProperties(ImGui SOURCE_DIR IMGUI_DIR)

set(IMGUI_SFML_FIND_SFML OFF)
FetchContent_Declare(ImGui-SFML
        GIT_REPOSITORY https://github.com/SFML/imgui-sfml
        GIT_TAG v3.0
        GIT_SHALLOW ON
        EXCLUDE_FROM_ALL
        SYSTEM)
FetchContent_MakeAvailable(ImGui-SFML)

add_executable(${PROJECT_NAME}
        src/main.cpp
        src/Profiler.hpp
        src/ChatClient.cpp
        src/ChatClient.h
        src/ChatServer.cpp
        src/ChatServer.h
)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)
target_link_libraries(${PROJECT_NAME} PUBLIC ImGui-SFML::ImGui-SFML PRIVATE zmq cppzmq)