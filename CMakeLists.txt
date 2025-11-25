cmake_minimum_required(VERSION 3.5)

project(cpp-terrain-project LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Necessário para classes Qt com Q_OBJECT
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Eigen3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(Qt5 REQUIRED COMPONENTS Widgets OpenGL)
find_package(GLU REQUIRED)

set(SOURCES
    main.cpp
    src/terrain/GradientField.cpp
    src/terrain/PoissonSolver.cpp
    src/terrain/TerrainGenerator.cpp
    src/terrain/MeshBuilder.cpp
    src/opengl/OpenGLWidget.cpp
)

add_executable(cpp-terrain-project ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE
    src
    src/terrain
    /usr/include/eigen3
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    Eigen3::Eigen
    OpenGL::GL
    OpenGL::GLU
    Qt5::Widgets
    Qt5::OpenGL
)
