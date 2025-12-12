set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

cmake_minimum_required(VERSION 3.16)
project(cpp-terrain-project LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Evitar conflito com macros min/max do Windows
add_definitions(-DNOMINMAX)

include(FetchContent)

# Caminho do libigl
set(LIBIGL_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external/libigl/include")

# Adicionar includes ao projeto
include_directories(${LIBIGL_INCLUDE_DIR})

set(LIBIGL_USE_STATIC_LIBRARY ON CACHE BOOL "" FORCE)

find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(OpenCV REQUIRED)
find_package(Eigen3 REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)

include_directories(
        ${EIGEN3_INCLUDE_DIR}
        ${OpenCV_INCLUDE_DIRS}
)


# Sources
set(SOURCE_FILES
        src/gui/MainWindow.cpp
        src/gui/PaintWidget.cpp
        src/gui/ShowTerrain.cpp
        src/core/PerlinNoise.cpp
        src/core/TerrainGenerator.cpp
        src/core/ImageToGradient.cpp
        main.cpp
)

# Headers
set(HEADER_FILES
        include/terrain-generator/MainWindow.h
        include/terrain-generator/PaintWidget.h
        include/terrain-generator/ShowTerrain.h
        include/terrain-generator/PerlinNoise.h
        include/terrain-generator/TerrainGenerator.h
        include/terrain-generator/ImageToGradient.h
        src/gui/PaintWidget.cpp
        src/core/ImageToGradient.cpp
)

# Criar executável
add_executable(cpp-terrain-project
        ${SOURCE_FILES}
        ${HEADER_FILES}
)



target_link_libraries(cpp-terrain-project
        Qt5::Widgets
        ${OpenCV_LIBS}
        Eigen3::Eigen
        glad::glad
        glfw
)
