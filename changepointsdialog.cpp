#include "changepointsdialog.h"
#include "ui_changepointsdialog.h"

ChangePointsDialog::ChangePointsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChangePointsDialog)
{
    ui->setupUi(this);

    connect(ui->pushButton,&QPushButton::clicked,this,&ChangePointsDialog::on_pushButton_clicked);

    connect(ui->pushButton_cancel,&QPushButton::clicked,this,&ChangePointsDialog::close);

}

ChangePointsDialog::~ChangePointsDialog()
{
    delete ui;
}

void ChangePointsDialog::on_pushButton_clicked()
{
    if(ui->pushButton->isChecked())
    {
    emit sendData(ui->lineEdit->text(),ui->lineEdit_2->text());

    this->close();

    }

}
