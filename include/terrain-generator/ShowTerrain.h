/**
* @file ShowTerrain.h
 * @brief 3D terrain visualization interface
 */
#ifndef SHOW_TERRAIN_H
#define SHOW_TERRAIN_H

#include <Eigen/Core>

/**
 * @brief Launches interactive 3D terrain viewer
 *
 * Displays heightmap as colored 3D mesh using LibIGL. Blocking call -
 * returns when viewer window is closed.
 *
 * @param heights Heightmap matrix (row-major, heights(y,x) = elevation)
 * @warning Requires LibIGL and OpenGL. For best performance, use heightmaps ≤512×512
 */
void showTerrain(const Eigen::MatrixXd& heights);

#endif