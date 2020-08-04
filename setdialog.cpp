#include "setdialog.h"
#include "ui_setdialog.h"
#include<QPushButton>
#include<QDebug>
SetDialog::SetDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SetDialog)
{
    ui->setupUi(this);



connect(ui->pushButtonok,&QPushButton::clicked,this,&SetDialog::pressBtn);

connect(ui->pushButton_cancel,&QPushButton::clicked,this,&SetDialog::close);

}



void SetDialog::pressBtn(){

    if(ui->radioButton1->isChecked())
         emit sendData(0);
    else if(ui->radioButton2->isChecked())
         emit sendData(1);

    else if(ui->radioButton3->isChecked())
         emit sendData(2);
    else


    emit sendData(ui->radioButton2->isChecked());

    this->close();

}
SetDialog::~SetDialog()
{
    delete ui;
}
