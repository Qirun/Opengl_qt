#include "changepointdialog.h"
#include "ui_changepointdialog.h"

ChangePointDialog::ChangePointDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChangePointDialog)
{
    ui->setupUi(this);

    connect(ui->pushButton_ok,&QPushButton::clicked,this,&ChangePointDialog::on_pushButton_clicked);

    connect(ui->pushButton_cancle,&QPushButton::clicked,this,&ChangePointDialog::close);

}

ChangePointDialog::~ChangePointDialog()
{
    delete ui;
}


void ChangePointDialog::on_pushButton_clicked()
{

    emit sendData(ui->lineEdit_x->text(),ui->lineEdit_y->text());

    this->close();

}
