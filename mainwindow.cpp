#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include<QtDebug>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->actionloadobj,&QAction::triggered,[=](){


    name = QFileDialog::getOpenFileName(this,"打开文件",":/model","(*.obj);(*.OBJ)");




    obj_or_stl=false;
    ui->openGLWidget->file_changed = true;

    qDebug()<<name;
    ui->openGLWidget->vertices.clear();
    ui->openGLWidget->uvs.clear();
    ui->openGLWidget->normals.clear();
    box_x=0;
    box_y=0;
    ui->openGLWidget->camera->position=QVector3D(0,2,50);
    loadOBJ(name.toUtf8().data(), ui->openGLWidget->vertices, ui->openGLWidget->uvs, ui->openGLWidget->normals);
    ui->openGLWidget->loadflag = true;
    });


    connect(ui->actionloadstl,&QAction::triggered,[=](){


    name = QFileDialog::getOpenFileName(this,"打开文件",":/model","(*.STL);(*.stl)");
    obj_or_stl=true;
    ui->openGLWidget->file_changed = true;

    qDebug()<<name;
    ui->openGLWidget->vertices.clear();
    ui->openGLWidget->uvs.clear();
    ui->openGLWidget->normals.clear();
    box_x=0;
    box_y=0;
     ui->openGLWidget->camera->position=QVector3D(0,2,50);
    read_binary_STL_file(name.toUtf8().data(), ui->openGLWidget->vertices, ui->openGLWidget->uvs, ui->openGLWidget->normals);

    ui->openGLWidget->loadflag = true;

    });


    connect(ui->actionsetmode,&QAction::triggered,[=](){

    dialog_set=new SetDialog();

    void (SetDialog:: *setdialogSignal)(char) = &SetDialog::sendData;
    void (MainWindow:: *updateSlot)(char) = &MainWindow::updateData;


    connect(dialog_set,setdialogSignal,this,updateSlot);

    if(dialog_set->exec()==QDialog::Accepted){
    }
    delete dialog_set;

    });





    connect(ui->actionchangepoints,&QAction::triggered,[=](){

    dialog_points = new ChangePointDialog();

    void (ChangePointDialog:: *changePointSignal)(QString,QString) = &ChangePointDialog::sendData;

    void (MainWindow:: *updatePointData_Slot)(QString,QString) = &MainWindow::updatePointData;


    connect(dialog_points,changePointSignal,this,updatePointData_Slot);

    if(dialog_points->exec()==QDialog::Accepted){
    }
    delete dialog_points;


    });

}





MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::updateData(char data)
{

    ui->openGLWidget->mode=data;


}

void MainWindow::updatePointData(QString data_x,QString data_y)
{
    ui->openGLWidget->vertices.clear();
    ui->openGLWidget->uvs.clear();
    ui->openGLWidget->normals.clear();
    box_x = data_x.toFloat();
    box_y = data_y.toFloat();

    if(obj_or_stl==false)
    {

       loadOBJ_change(name.toUtf8().data(), ui->openGLWidget->vertices, ui->openGLWidget->uvs, ui->openGLWidget->normals,box_x,box_y);
    }
    else
    {

        read_binary_STL_file_change(name.toUtf8().data(), ui->openGLWidget->vertices, ui->openGLWidget->uvs, ui->openGLWidget->normals,box_x,box_y);
    }

}

void MainWindow::keyPressEvent(QKeyEvent *event)
{

    int key = event->key();
    if (key >= 0 && key < 1024)
        ui->openGLWidget->keyPressEvent(event);

}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    int key = event->key();
    if (key >= 0 && key < 1024)
    ui->openGLWidget->keyReleaseEvent(event);

}
