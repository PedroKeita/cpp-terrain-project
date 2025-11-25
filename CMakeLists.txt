cmake_minimum_required(VERSION 3.5)
project(cpp-terrain-project LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Procurar Eigen3
find_package(Eigen3 REQUIRED)
include_directories(${EIGEN3_INCLUDE_DIR})

# Arquivos fontes
set(SOURCES
    main.cpp
    src/terrain/GradientField.cpp
    src/terrain/PoissonSolver.cpp
    src/terrain/TerrainGenerator.cpp
    src/terrain/MeshBuilder.cpp
)

# Criar executável
add_executable(cpp-terrain-project ${SOURCES})

# Incluir diretórios dos seus arquivos
target_include_directories(cpp-terrain-project PRIVATE
    src
    src/terrain
)

# Linkar Eigen
target_link_libraries(cpp-terrain-project PRIVATE Eigen3::Eigen)
