add_executable(${PROJECT_NAME}
    main.cpp
    Profiler.hpp
    ChatClient.cpp
    ChatClient.h
    ChatServer.cpp
    ChatServer.h
)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)
target_link_libraries(${PROJECT_NAME}
    PUBLIC ImGui-SFML::ImGui-SFML
    PRIVATE zmq cppzmq
)