#include "QtZipViewerApp.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QtZipViewerApp w;
    w.show();
    return a.exec();
}
