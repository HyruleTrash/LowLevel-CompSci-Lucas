add_executable(collision-crisis
    main.cpp
    Profiler.hpp
    Ball.h
    BallSimulation.cpp
    BallSimulation.h
)

target_link_libraries(collision-crisis
 PUBLIC
  ImGui-SFML::ImGui-SFML
)

# Do we need this?
# include(Install.cmake)