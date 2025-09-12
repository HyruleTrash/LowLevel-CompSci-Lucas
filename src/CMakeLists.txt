add_executable(low-level_setup
        main.cpp
        Ball.cpp
        Ball.h
        Entity.h
        EntityManager.cpp
        EntityManager.h
)

target_link_libraries(low-level_setup
 PUBLIC
  ImGui-SFML::ImGui-SFML
)

# Do we need this?
# include(Install.cmake)