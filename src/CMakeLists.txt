add_executable(low-level_setup
 main.cpp
        Texture.cpp
        Texture.h
        PixelNeighbours.h
        Profiler.hpp
        DefaultValues.h
)

target_link_libraries(low-level_setup
 PUBLIC
  ImGui-SFML::ImGui-SFML
)

# Do we need this?
# include(Install.cmake)