#ifndef SETDIALOG_H
#define SETDIALOG_H
#include "corefunctionwidget.h"
#include <QDialog>

namespace Ui {
class SetDialog;
}

class SetDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SetDialog(QWidget *parent = nullptr);
    ~SetDialog();

    void pressBtn();

signals:
    void sendData(char data);


private:
    Ui::SetDialog *ui;
};

#endif // SETDIALOG_H
