#ifndef CHANGEPOINTDIALOG_H
#define CHANGEPOINTDIALOG_H

#include <QDialog>

namespace Ui {
class ChangePointDialog;
}

class ChangePointDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ChangePointDialog(QWidget *parent = nullptr);
    ~ChangePointDialog();

    void on_pushButton_clicked();

signals:
    void sendData(QString,QString);

private:
    Ui::ChangePointDialog *ui;
};

#endif // CHANGEPOINTDIALOG_H
