add_executable(low-level_setup
        main.cpp
        Profiler.hpp
        Ball.cpp
        Ball.h
        Entity.h
        EntityManager.cpp
        EntityManager.h
        Component.cpp
        Component.h
        Entity.cpp
        RigidBody.cpp
        RigidBody.h
        Config.cpp
        Config.h
        OffScreenTeleporter.cpp
        OffScreenTeleporter.h
)

target_link_libraries(low-level_setup
 PUBLIC
  ImGui-SFML::ImGui-SFML
)

# Do we need this?
# include(Install.cmake)