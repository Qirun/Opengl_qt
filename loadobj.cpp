#include "loadobj.hpp"

#include <stdio.h>
#include <QKeyEvent>
#include <QMatrix4x4>
#include <string>
#include <cstring>
#include<QFile>
#include<QtDebug>


// Very, VERY simple OBJ loader.
// Here is a short list of features a real function would provide :
// - Binary files. Reading a model should be just a few memcpy's away, not parsing a file at runtime. In short : OBJ is not very great.
// - Animations & bones (includes bones weights)
// - Multiple UVs
// - All attributes should be optional, not "forced"
// - More stable. Change a line in the OBJ file and it crashes.
// - More secure. Change another line and you can inject code.
// - Loading from memory, stream, etc

bool loadOBJ(
    const char* path,
    //std::vector<glm::vec3>& out_vertices,
    //std::vector<glm::vec3>& out_normals
    QVector<QVector3D>& out_vertices,
    QVector<QVector2D>&  out_uvs,
    QVector<QVector3D>&  out_normals
   )

{
    std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
    QVector<QVector3D> temp_vertices;
    QVector<QVector2D> temp_uvs;
    QVector<QVector3D> temp_normals;

    FILE* file = fopen(path, "r");
    if (file == NULL) {
        qDebug() << "Impossible to open the file ! Are you in the right path ? See Tutorial 1 for details\n" ;
        //printf("Impossible to open the file ! Are you in the right path ? See Tutorial 1 for details\n");
        getchar();
        return false;
    }
int cc=0;
    while (1) {

        char lineHeader[128];
        // read the first word of the line
        int res = fscanf(file, "%s", lineHeader);
        if (res == EOF)
            break; // EOF = End Of File. Quit the loop.

        // else : parse lineHeader

        if (strcmp(lineHeader, "v") == 0) {
            QVector3D vertex;
            float v_x,v_y,v_z;
            fscanf(file, "%f %f %f\n", &v_x, &v_y, &v_z);
            vertex.setX(v_x);
            vertex.setY(v_y);
            vertex.setZ(v_z);
            temp_vertices.push_back(vertex);
        }
        else if (strcmp(lineHeader, "vt") == 0) {
            QVector2D uv;
            float uv_x,uv_y;
            fscanf(file, "%f %f\n", &uv_x, &uv_y);
            uv_y = -uv_y; // Invert V coordinate since we will only use DDS texture, which are inverted. Remove if you want to use TGA or BMP loaders.

            uv.setX(uv_x);
            uv.setY(uv_y);
            temp_uvs.push_back(uv);
        }
        else if (strcmp(lineHeader, "vn") == 0) {
            QVector3D normal;
            float n_x,n_y,n_z;
            fscanf(file, "%f %f %f\n", &n_x, &n_y, &n_z);
            normal.setX(n_x);
            normal.setY(n_y);
            normal.setZ(n_z);
            temp_normals.push_back(normal);
        }
        else if (strcmp(lineHeader, "f") == 0) {
            std::string vertex1, vertex2, vertex3;
            unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
            int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
            if (matches != 9) {
                printf("File can't be read by our simple parser :-( Try exporting with other options\n");
                return false;
            }
            vertexIndices.push_back(vertexIndex[0]);
            vertexIndices.push_back(vertexIndex[1]);
            vertexIndices.push_back(vertexIndex[2]);
            uvIndices.push_back(uvIndex[0]);
            uvIndices.push_back(uvIndex[1]);
            uvIndices.push_back(uvIndex[2]);
            normalIndices.push_back(normalIndex[0]);
            normalIndices.push_back(normalIndex[1]);
            normalIndices.push_back(normalIndex[2]);
        }
        else {
            // Probably a comment, eat up the rest of the line
            char stupidBuffer[1000];
            fgets(stupidBuffer, 1000, file);
        }

    }

    // For each vertex of each triangle
    for (unsigned int i = 0; i < vertexIndices.size(); i++) {

        // Get the indices of its attributes
        unsigned int vertexIndex = vertexIndices[i];
        unsigned int uvIndex = uvIndices[i];
        unsigned int normalIndex = normalIndices[i];

        // Get the attributes thanks to the index
        QVector3D vertex = temp_vertices[vertexIndex - 1];
        QVector2D uv = temp_uvs[uvIndex - 1];
        QVector3D normal = temp_normals[normalIndex - 1];

        // Put the attributes in buffers
        out_vertices.push_back(vertex);
        out_uvs.push_back(uv);
        out_normals.push_back(normal);


    }

    return true;
}


bool loadOBJ_change(
    const char* path,
    //std::vector<glm::vec3>& out_vertices,
    //std::vector<glm::vec3>& out_normals
    QVector<QVector3D>& out_vertices,
    QVector<QVector2D>&  out_uvs,
    QVector<QVector3D>&  out_normals,
    float box_x,
    float box_y
   )
{
    float  x_min=0;
    float  x_max=0;
    float  y_min=0;
    float  y_max=0;
    float  z_min=0;
    float  z_max=0;

    float box_x_cnt=0;
    float box_y_cnt=0;
    unsigned char count_obj=0;//由于在更新点密度时计数

    QVector3D face[3];

    QVector3D P0;//P点指向三角面片第一个点的向量
    QVector3D P1;//P点指向三角面片第二个点的向量
    QVector3D P2;//P点指向三角面片第三个点的向量

    QVector3D P0_P1;//P0叉乘P1
    QVector3D P1_P2;//P1叉乘P2
    QVector3D P2_P0;//P2叉乘P0

    qDebug() <<"Loading OBJ file ...\n";
    std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
    QVector<QVector3D> temp_vertices;
    QVector<QVector2D> temp_uvs;
    QVector<QVector3D> temp_normals;

    //QFile file(path);
    //file.open(QIODevice::ReadOnly);
    FILE* file = fopen(path, "r");
    if (file == NULL) {
        qDebug() << "Impossible to open the file ! Are you in the right path ? See Tutorial 1 for details\n" ;
        //printf("Impossible to open the file ! Are you in the right path ? See Tutorial 1 for details\n");
        getchar();
        return false;
    }

    while (1) {

        char lineHeader[128];
        // read the first word of the line
        int res = fscanf(file, "%s", lineHeader);
        if (res == EOF)
            break; // EOF = End Of File. Quit the loop.

        // else : parse lineHeader
        if (strcmp(lineHeader, "v") == 0) {
            QVector3D vertex;
            float v_x,v_y,v_z;
            fscanf(file, "%f %f %f\n", &v_x, &v_y, &v_z);
            vertex.setX(v_x);
            vertex.setY(v_y);
            vertex.setZ(v_z);
            temp_vertices.push_back(vertex);

            if(vertex.x()<x_min) x_min =  vertex.x();
            if(vertex.x()>x_max) x_max =  vertex.x();
            if(vertex.y()<y_min) y_min =  vertex.y();
            if(vertex.y()>y_max) y_max =  vertex.y();
            if(vertex.z()<z_min) z_min =  vertex.z();
            if(vertex.z()>z_max) z_max =  vertex.z();
        }
        else if (strcmp(lineHeader, "vt") == 0) {
            QVector2D uv;
            float uv_x,uv_y;
            fscanf(file, "%f %f\n", &uv_x, &uv_y);
            uv_y = -uv_y; // Invert V coordinate since we will only use DDS texture, which are inverted. Remove if you want to use TGA or BMP loaders.

            uv.setX(uv_x);
            uv.setY(uv_y);
            temp_uvs.push_back(uv);
        }
        else if (strcmp(lineHeader, "vn") == 0) {
            QVector3D normal;
            float n_x,n_y,n_z;
            fscanf(file, "%f %f %f\n", &n_x, &n_y, &n_z);
            normal.setX(n_x);
            normal.setY(n_y);
            normal.setZ(n_z);
            temp_normals.push_back(normal);
        }
        else if (strcmp(lineHeader, "f") == 0) {   
            std::string vertex1, vertex2, vertex3;
            unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
            int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
            if (matches != 9) {
                printf("File can't be read by our simple parser :-( Try exporting with other options\n");
                return false;
            }
            vertexIndices.push_back(vertexIndex[0]);
            vertexIndices.push_back(vertexIndex[1]);
            vertexIndices.push_back(vertexIndex[2]);
            uvIndices.push_back(uvIndex[0]);
            uvIndices.push_back(uvIndex[1]);
            uvIndices.push_back(uvIndex[2]);
            normalIndices.push_back(normalIndex[0]);
            normalIndices.push_back(normalIndex[1]);
            normalIndices.push_back(normalIndex[2]);
        }
        else {
            // Probably a comment, eat up the rest of the line
            char stupidBuffer[1000];
            fgets(stupidBuffer, 1000, file);
        }

    }

    // For each vertex of each triangle
    for (unsigned int i = 0; i < vertexIndices.size(); i++) {
        qDebug()<<vertexIndices.size();
        // Get the indices of its attributes
        unsigned int vertexIndex = vertexIndices[i];
        unsigned int uvIndex = uvIndices[i];
        unsigned int normalIndex = normalIndices[i];

        // Get the attributes thanks to the index
        QVector3D vertex = temp_vertices[vertexIndex - 1];
        QVector2D uv = temp_uvs[uvIndex - 1];
        QVector3D normal = temp_normals[normalIndex - 1];
        QVector3D point_new;

        // Put the attributes in buffers
        out_vertices.push_back(vertex);
        out_uvs.push_back(uv);
        out_normals.push_back(normal);


        switch (count_obj)
        {
            case 0:
                face[0]= vertex;
                count_obj++;
                break;
            case 1:
                face[1]= vertex;
                count_obj++;
                break;
            case 2:
                face[2]= vertex;
                for(box_x_cnt=x_min;box_x_cnt<x_max;)
                {
                    for(box_y_cnt=y_min;box_y_cnt<y_max;)
                    {
                         P0.setX(face[0].x()-box_x_cnt);
                         P0.setY(face[0].y()-box_y_cnt);
                         P0.setZ(0);

                         P1.setX(face[1].x()-box_x_cnt);
                         P1.setY(face[1].y()-box_y_cnt);
                         P1.setZ(0);

                         P2.setX(face[2].x()-box_x_cnt);
                         P2.setY(face[2].y()-box_y_cnt);
                         P2.setZ(0);

                         P0_P1=QVector3D::crossProduct(P0,P1);
                         P1_P2=QVector3D::crossProduct(P1,P2);
                         P2_P0=QVector3D::crossProduct(P2,P0);

                         if(((P0_P1.z()>0)&&(P1_P2.z()>0)&&(P2_P0.z()>0))||((P0_P1.z()<0)&&(P1_P2.z()<0)&&(P2_P0.z()<0)))
                         {
                             //射线与三角面片有交点，求交运算
                             //将box_x_cnt、box_y_cnt带入方程
                             point_new.setX(box_x_cnt);
                             point_new.setY(box_y_cnt);
                             point_new.setZ(face[0].z()-(((normal.x()*(box_x_cnt-face[0].x()))+(normal.y()*(box_y_cnt-face[0].y())))/normal.z()));
                             out_vertices.push_back(point_new);
                         }
                         box_y_cnt=box_y_cnt+box_y;
                    }
                    box_x_cnt=box_x_cnt+box_x;
                }
                count_obj = 0;

                break;
            default:
                count_obj=0;
                break;
        }


    }

    return true;

}

