#include "loadstl.hpp"
#include <stdio.h>
#include <QMatrix4x4>
#include <string>
#include <cstring>
#include <iostream>
#include<QFile>
#include<QtDebug>
#include <QKeyEvent>



int read_binary_STL_file_change(const char* path,QVector<QVector3D>& out_vertices,QVector<QVector2D>& out_uvs,QVector<QVector3D>& out_normals,float box_x,float box_y)

{
    float  x_min=0;
    float  x_max=0;
    float  y_min=0;
    float  y_max=0;
    float  z_min=0;
    float  z_max=0;

    float box_x_cnt=0;
    float box_y_cnt=0;

    QVector3D P0;//P点指向三角面片第一个点的向量
    QVector3D P1;//P点指向三角面片第二个点的向量
    QVector3D P2;//P点指向三角面片第三个点的向量

    QVector3D P0_P1;//P0叉乘P1
    QVector3D P1_P2;//P1叉乘P2
    QVector3D P2_P0;//P2叉乘P0

    QVector3D face[3];

    // specify the location of STL files on this computer
    //std::string STL_files_path = "../stl/";
    qDebug() <<"Loading STL file ...\n";
    char name[80];
    // declare an (input) file object
    //std::ifstream binaryInputFile;
    FILE* file = fopen(path, "rb");
    if (file == NULL) {
        qDebug() << "Impossible to open the stl file ! Are you in the right path ? See Tutorial 1 for details\n" ;
        //printf("Impossible to open the file ! Are you in the right path ? See Tutorial 1 for details\n");
        //getchar();
        return false;
    }
    fread(name,80,1,file);
    
    // open the STL file by using the full path and the name
    // specify that the file is opened in "read-only" mode
    // specify that the file is opened in binary format

    // read the number of facets (triangles) in the STL geometry
    int numberOfTriangles;
    fread(&numberOfTriangles,4,1,file); ///得到facet个数
    qDebug()<<"numberOfTriangles = "<<numberOfTriangles;

    //binaryInputFile.read((char*) &numberOfTriangles, sizeof(int));

    // declare an object "tri" of type triangle (see main.h for the definition of the triangle class)
    //class_vertices tri;
    //class_normals nor;

    QVector3D tri;
    QVector3D point_new;
    QVector2D uv;
    QVector3D nor;

    // storage space for the "unused bytes"
    char unused_bytes[2];

    // initialize parameters that will be used to store the minimum and maximum extents of the geometry
    // described by the STL file
    //x_min = 1e+30, x_max = -1e+30;
    //y_min = 1e+30, y_max = -1e+30;
    //z_min = 1e+30, z_max = -1e+30;

    for(int count=0;count<numberOfTriangles;count++)
    {

        // read the three components of the normal vector
        //QVector3D nor;
        float n_x,n_y,n_z;
       //fscanf(file, "%f %f %f\n", &n_x, &n_y, &n_z);
        fread(&n_x,4,1,file);
        fread(&n_y,4,1,file);
        fread(&n_z,4,1,file);

        nor.setX(n_x);
        nor.setY(n_y);
        nor.setZ(n_z);

        out_normals.push_back(nor);

        unsigned int count1;
        for (count1=0;count1<=2;count1++)
        {
        // read the three coordinates of vertex 1
         //QVector3D tri;
         float v_x,v_y,v_z;
         fread(&v_x,4,1,file);
         fread(&v_y,4,1,file);
         fread(&v_z,4,1,file);

         tri.setX(v_x);
         tri.setY(v_y);
         tri.setZ(v_z);

         switch (count1)
         {

         case 0:
             face[0].setX(v_x);
             face[0].setY(v_y);
             face[0].setZ(v_z);
             break;
         case 1:
             face[1].setX(v_x);
             face[1].setY(v_y);
             face[1].setZ(v_z);
             break;
         case 2:
             face[2].setX(v_x);
             face[2].setY(v_y);
             face[2].setZ(v_z);
             break;

         }


         uv.setX(0.5);
         uv.setY(0.5);
         out_uvs.push_back(uv);
        //file.read(unused_bytes,2);

        if(tri.x()<x_min) x_min =  tri.x();
        if(tri.x()>x_max) x_max =  tri.x();
        if(tri.y()<y_min) y_min =  tri.y();
        if(tri.y()>y_max) y_max =  tri.y();
        if(tri.z()<z_min) z_min =  tri.z();
        if(tri.z()>z_max) z_max =  tri.z();
        out_vertices.push_back(tri);
        }

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
                     point_new.setZ(face[0].z()-(((nor.x()*(box_x_cnt-face[0].x()))+(nor.y()*(box_y_cnt-face[0].y())))/nor.z()));
                     out_vertices.push_back(point_new);
                 }
                 box_y_cnt=box_y_cnt+box_y;
            }
            box_x_cnt=box_x_cnt+box_x;
        }
         fread(&unused_bytes,2,1,file);

    }

    // explicitly close the connection to the input STL file
    fclose(file);
    //binaryInputFile.close();

    return (0);  // all is well
}

int read_binary_STL_file(const char* path,QVector<QVector3D>& out_vertices,QVector<QVector2D>& out_uvs,QVector<QVector3D>& out_normals)
                         //,double & x_min, double & x_max, double & y_min, double & y_max, double & z_min, double & z_max)
{
    // specify the location of STL files on this computer
    //std::string STL_files_path = "../stl/";
    qDebug() <<"Loading STL file ...\n";
    char name[80];
    // declare an (input) file object
    //std::ifstream binaryInputFile;
    FILE* file = fopen(path, "rb");
    if (file == NULL) {
        qDebug() << "Impossible to open the stl file ! Are you in the right path ? See Tutorial 1 for details\n" ;
        //printf("Impossible to open the file ! Are you in the right path ? See Tutorial 1 for details\n");
        //getchar();
        return false;
    }
    fread(name,80,1,file);

    // open the STL file by using the full path and the name
    // specify that the file is opened in "read-only" mode
    // specify that the file is opened in binary format
   // binaryInputFile.open(path, std::ifstream::in | std::ifstream::binary);

    // check whether the file was opened successfully
    // if yes then continue otherwise terminate program execution
    //if(binaryInputFile.fail())
    //{
    //    std::cout << "ERROR: Input STL file could not be opened!" << std::endl;
    //    return (1);
    //}

    // position the pointer to byte number 80
    //binaryInputFile.seekg(80);

    // read the number of facets (triangles) in the STL geometry
    int numberOfTriangles;
    fread(&numberOfTriangles,4,1,file); ///得到facet个数
    qDebug()<<"numberOfTriangles = "<<numberOfTriangles;

    //binaryInputFile.read((char*) &numberOfTriangles, sizeof(int));

    // declare an object "tri" of type triangle (see main.h for the definition of the triangle class)
    //class_vertices tri;
    //class_normals nor;

    QVector3D tri;
    QVector2D uv;
    QVector3D nor;

    // storage space for the "unused bytes"
    char unused_bytes[2];

    // initialize parameters that will be used to store the minimum and maximum extents of the geometry
    // described by the STL file
    //x_min = 1e+30, x_max = -1e+30;
    //y_min = 1e+30, y_max = -1e+30;
    //z_min = 1e+30, z_max = -1e+30;

    for(int count=0;count<numberOfTriangles;count++)
    {

        // read the three components of the normal vector
        //QVector3D nor;
        float n_x,n_y,n_z;
       //fscanf(file, "%f %f %f\n", &n_x, &n_y, &n_z);
        fread(&n_x,4,1,file);
        fread(&n_y,4,1,file);
        fread(&n_z,4,1,file);

        nor.setX(n_x);
        nor.setY(n_y);
        nor.setZ(n_z);

        out_normals.push_back(nor);
        out_normals.push_back(nor);
        out_normals.push_back(nor);

        //binaryInputFile.read((char*)&temp_float_var,4); nor.setX((double) temp_float_var);//nor.normal.x = (double) temp_float_var;
        //binaryInputFile.read((char*)&temp_float_var,4); nor.setY((double) temp_float_var);//nor.normal.y = (double) temp_float_var;
        //binaryInputFile.read((char*)&temp_float_var,4); nor.setZ((double) temp_float_var);//nor.normal.z = (double) temp_float_var;

        unsigned int count1;
        for (count1=0;count1<=2;count1++)
        {
        // read the three coordinates of vertex 1
         //QVector3D tri;
         float v_x,v_y,v_z;
         fread(&v_x,4,1,file);
         fread(&v_y,4,1,file);
         fread(&v_z,4,1,file);

         tri.setX(v_x);
         tri.setY(v_y);
         tri.setZ(v_z);
       //binaryInputFile.read((char*)&temp_float_var,4); tri.setX((double) temp_float_var);//tri.point[0].x = (double) temp_float_var;
       //binaryInputFile.read((char*)&temp_float_var,4); tri.setY((double) temp_float_var);//tri.point[0].y = (double) temp_float_var;
       //binaryInputFile.read((char*)&temp_float_var,4); tri.setZ((double) temp_float_var);//tri.point[0].z = (double) temp_float_var;

         out_vertices.push_back(tri);
         uv.setX(0.5);
         uv.setY(0.5);
         out_uvs.push_back(uv);
        //file.read(unused_bytes,2);

        }
         fread(&unused_bytes,2,1,file);


        // add data for this triangle to the "facet" vector
        //out_vertices.push_back(tri);
        //out_normals.push_back(nor);
    }

    // explicitly close the connection to the input STL file
    fclose(file);
    //binaryInputFile.close();

    return (0);  // all is well
}

