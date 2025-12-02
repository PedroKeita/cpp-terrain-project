set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

cmake_minimum_required(VERSION 3.16)
project(cpp-terrain-project LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
# Caminho do libigl
set(LIBIGL_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external/libigl/include/igl")

# Adicionar includes ao projeto
include_directories(${LIBIGL_INCLUDE_DIR})

set(LIBIGL_USE_STATIC_LIBRARY ON CACHE BOOL "" FORCE)
# ---------------------------------
# Qt, OpenCV, Eigen
# ---------------------------------
find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(OpenCV REQUIRED)
find_package(Eigen3 REQUIRED)
#find_package(igl CONFIG REQUIRED)

include_directories(
        ${EIGEN3_INCLUDE_DIR}
        ${OpenCV_INCLUDE_DIRS}
)

# ---------------------------------
# libigl como submodule
# ---------------------------------

# ---------------------------------
# Fontes
# ---------------------------------
file(GLOB SRC
        src/*.cpp
        src/*.h
)

add_executable(cpp-terrain-project ${SRC}
        src/PoissonSolver.cpp
        src/PoissonSolver.cpp
        src/ShowTerrain.cpp
        src/ShowTerrain.h
        main.cpp
        main.cpp
        src/TerrainGenerator.cpp
        src/TerrainGenerator.h
)

target_link_libraries(cpp-terrain-project
        Qt5::Widgets
        ${OpenCV_LIBS}
        Eigen3::Eigen
        Qt5::Widgets


)
