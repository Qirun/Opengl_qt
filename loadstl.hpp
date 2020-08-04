#pragma once



// include files
#include <iostream>
#include <string>
#include <fstream>
#include <cmath>
#include <QVector3D>
#include <QVector2D>

int read_binary_STL_file(const char* path,
                         QVector<QVector3D>& out_vertices,
                         QVector<QVector2D>& out_uvs,
                         QVector<QVector3D>& out_normals
                         );

int read_binary_STL_file_change(const char* path,
                         QVector<QVector3D>& out_vertices,
                         QVector<QVector2D>& out_uvs,
                         QVector<QVector3D>& out_normals,
                         float box_x,
                         float box_y
                         );


