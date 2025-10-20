add_executable(low-level_setup
    main.cpp
    Profiler.hpp
        ChatClient.cpp
        ChatClient.h
        ChatServer.cpp
        ChatServer.h
)

target_link_libraries(low-level_setup
 PUBLIC
  ImGui-SFML::ImGui-SFML
)

# Do we need this?
# include(Install.cmake)