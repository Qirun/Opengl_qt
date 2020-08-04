#include "mainwindow.h"
#include  "QMessageBox"
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFile file(":/image/style.qss");
        if( file.open(QFile::ReadOnly))
        {
            QString styleSheet = QLatin1String(file.readAll());

            a.setStyleSheet(styleSheet);
            file.close();

        }
        else  // 引入出错
        {

            QMessageBox::warning(NULL, "warning", "Open failed", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);

        }



    MainWindow w;

    w.show();
    return a.exec();
}
