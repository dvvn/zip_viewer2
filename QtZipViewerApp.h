#ifndef QT_ZIP_VIEWER_APP_H
#define QT_ZIP_VIEWER_APP_H

#include "ui_QtZipViewerApp.h"

class QtZipViewerApp: public QMainWindow
{
Q_OBJECT

public:
    QtZipViewerApp(QWidget* parent = Q_NULLPTR);

private:

    Ui::QtWidgetsApplication1Class ui;

    //std::unique_ptr<QAbstractTableModel> mz_table;
 //std::unique_ptr<QAbstractItemView>    view;
};

#endif