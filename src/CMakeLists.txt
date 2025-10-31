add_executable(${PROJECT_NAME}
    main.cpp
    Profiler.hpp
    ParticleSystem.cpp
    ParticleSystem.h
    Particle.cpp
    Particle.h
    WorkerThreadPool.cpp
    WorkerThreadPool.h
        BasicWaitingThread.cpp
        BasicWaitingThread.h
)

target_link_libraries(${PROJECT_NAME}
 PUBLIC
  ImGui-SFML::ImGui-SFML
)

# Do we need this?
# include(Install.cmake)