
# 🏔️ cpp-terrain-project: 3D Terrain Generator

[![Build Status](https://img.shields.io/badge/build-passing-brightgreen)]()
[![License](https://img.shields.io/badge/license-MIT-blue)]()
[![C++](https://img.shields.io/badge/C++-17-00599C)]()
[![Doxygen](https://img.shields.io/badge/docs-Doxygen-6A0DAD)]()

<div align="center">
  <img src="images/a-demo-image-.png" alt="Terrain Generation Demo" width="500"/>
  <p><em>Transform 2D sketches into detailed 3D terrains</em></p>
</div>

📄 Leia esta documentação em Português: [README_BR.md](README_BR.md)

---

## 📖 Overview

**cpp-terrain-project** is an interactive C++ application that converts 2D drawings into detailed 3D terrains using advanced computer graphics and numerical methods. The system combines image processing, gradient-based reconstruction, and procedural noise generation to create realistic landscapes.

### 🎯 Purpose & Key Features

| Feature | Description |
|---------|-------------|
| **🎨 Interactive Drawing** | Draw terrain profiles with adjustable brush sizes |
| **📐 Gradient Extraction** | Uses Sobel operators to analyze image gradients |
| **🧮 Poisson Reconstruction** | Solves Poisson equation to reconstruct 3D surfaces |
| **🌊 Perlin Noise** | Adds natural-looking multi-octave procedural noise |
| **🎭 3D Visualization** | Real-time OpenGL rendering with Libigl viewer |
| **⚙️ Erosion Simulation** | Applies soft erosion algorithms for realism |

### 📦 Required Libraries
| Library | Minimum Version | Purpose                                | Installation Command             |
| ------- | --------------- | -------------------------------------- | -------------------------------- |
| Eigen3  | 3.4+            | Linear algebra & sparse solvers        | `sudo apt install libeigen3-dev` |
| OpenCV  | 4.5+            | Image processing & gradient extraction | `sudo apt install libopencv-dev` |
| Qt6     | 6.0+            | Graphical user interface               | `sudo apt install qt6-base-dev`  |
| Libigl  | Git submodule   | 3D mesh processing & visualization     | Included via Git submodule       |
| CMake   | 3.16+           | Build system                           | `sudo apt install cmake`         |


## 📁 Project Structure


```bash
cpp-terrain-project/
├── include/terrain-generator/
│   ├── TerrainGenerator.h
│   ├── ImageToGradient.h
│   ├── PerlinNoise.h
│   ├── ShowTerrain.h
│   ├── MainWindow.h
│   └── PaintWidget.h
├── src/
│   ├── core/
│   │   ├── TerrainGenerator.cpp
│   │   ├── ImageToGradient.cpp
│   │   └── PerlinNoise.cpp
│   └── gui/
│       ├── MainWindow.cpp
│       ├── ShowTerrain.cpp
│       └── PaintWidget.cpp
├── external/libigl/     # graphics library 3D
├── docs/doxygen/        # document generated
├── .github/workflows/   # CI/CD (docs.yml)
├── CMakeLists.txt
├── Doxyfile
├── main.cpp
├── README_BR.md
└── README.md
```

## 🖼️ Before & After: Visual Transformation

### Input: 2D Drawing

![2D](images/before-convert.png)

### Output: 3D Terrain

![3D](images/after-convert.png)

#### **With 3D mesh**

![3D_mesh](images/after-convert-malha3D.png)

## 🎮 Application Controls
### 🖌 Drawing Interface
| Control             | Action               | Description            |
| ------------------- | -------------------- | ---------------------- |
| Left-click + Drag   | Draw terrain profile | Create mountain shapes |
| Brush Size Slider   | Adjust thickness     | 1px to 20px range      |
| Clear Button        | Reset canvas         | Start new drawing      |
| Generate 3D Terrain | Process & visualize  | Opens 3D viewer        |

### 🧊 3D Viewer Controls (Libigl)

| Key / Control | Action            | Effect                     |
| ------------- | ----------------- | -------------------------- |
| Mouse Drag    | Rotate scene      | Orbit around terrain       |
| Scroll        | Zoom in/out       | Adjust viewing distance    |
| Shift + Drag  | Pan camera        | Move view horizontally     |
| L, l          | Toggle wireframe  | Switch rendering mode      |
| F, f          | Toggle face mode  | Change face rendering      |
| O, o          | Toggle projection | Orthographic / Perspective |
| S, s          | Toggle shadows    | Lighting effects           |
| T, t          | Face fill toggle  | Solid / transparent faces  |
| Z             | Reset view        | Default camera position    |
| [ , ]         | Rotation mode     | Change control type        |
| < , >         | Model toggle      | Switch models              |
| ESC           | Close viewer      | Exit application           |


## 🧩 Building from Source

### 🛠️ Compilation Steps

#### **1️⃣ Clone Repository**

```bash 
git clone --recursive https://github.com/PedroKeita/cpp-terrain-project.git
cd cpp-terrain-project
```

#### **2️⃣ Configure Build**
```bash
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
```

#### **3️⃣ Compile**
```bash
make -j$(nproc)
```

#### **4️⃣ Run Application**
```bash
./cpp-terrain-project
```

## 📓 Documentation

### 📚 API Reference
Full auto-generated documentation available at:
https://pedrokeita.github.io/cpp-terrain-project/

### 🔧 Generate Docs Locally
```bash
sudo apt install doxygen graphviz
doxygen Doxyfile
xdg-open docs/doxygen/html/index.html
```

