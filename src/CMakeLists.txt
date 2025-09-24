add_executable(low-level_setup
 main.cpp
        Texture.cpp
        Texture.h
        Pixel.cpp
        Pixel.h
        Profiler.hpp
)

target_link_libraries(low-level_setup
 PUBLIC
  ImGui-SFML::ImGui-SFML
)

# Do we need this?
# include(Install.cmake)