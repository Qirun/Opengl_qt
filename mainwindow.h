#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "setdialog.h"
#include"changepointdialog.h"
#include <QMainWindow>
#include <QTextCodec>
#include "corefunctionwidget.h"
#include<QKeyEvent>
#include "loadobj.hpp"
#include "loadstl.hpp"
#include"Camera.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
//槽函数

    //void pressBtn();
    void updateData(char data);//用于确定显示模式

    void updatePointData(QString data_x,QString data_y);


protected:
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

private:
    Ui::MainWindow *ui;
    bool obj_or_stl=false;
    float box_x;
    float box_y;

    QString name;//文件路径

    SetDialog *dialog_set;

    ChangePointDialog *dialog_points;


};
#endif // MAINWINDOW_H
