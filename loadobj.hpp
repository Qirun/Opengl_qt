//#ifndef LOADOBJ_H
//#define LOADOBJ_H


//class loadOBJ
//{
//public:
//    loadOBJ();
//};

//#endif // LOADOBJ_H
#pragma once
#include <QVector3D>
#include <QVector2D>


bool loadOBJ(
    const char* path,
//    std::vector<glm::vec3>& out_vertices,
//    std::vector<glm::vec2>& out_uvs,
//    std::vector<glm::vec3>& out_normals

    QVector<QVector3D>& out_vertices,
    QVector<QVector2D>&  out_uvs,
    QVector<QVector3D>&  out_normals
);

bool loadOBJ_change(
    const char* path,
    //std::vector<glm::vec3>& out_vertices,
    //std::vector<glm::vec3>& out_normals
    QVector<QVector3D>& out_vertices,
    QVector<QVector2D>&  out_uvs,
    QVector<QVector3D>&  out_normals,
    float box_x,
    float box_y);



