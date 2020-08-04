#ifndef CHANGEPOINTSDIALOG_H
#define CHANGEPOINTSDIALOG_H

#include <QDialog>

namespace Ui {
class ChangePointsDialog;
}

class ChangePointsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ChangePointsDialog(QWidget *parent = nullptr);
    ~ChangePointsDialog();


    void on_pushButton_clicked();

signals:
    void sendData(QString,QString);


private:
    Ui::ChangePointsDialog *ui;

};

#endif // CHANGEPOINTSDIALOG_H
