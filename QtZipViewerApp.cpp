#include "QtZipViewerApp.h"

#include "ZipVisualizer.h"

#include <QApplication.h>
#include <qfiledialog.h>
#include <QFormLayout.h>
#include <QHeaderView.h>
#include <qmessagebox.h>
#include <QPushButton.h>
#include <QTableView.h>

QtZipViewerApp::QtZipViewerApp(QWidget* parent) : QMainWindow(parent)
{
	ui.setupUi(this);

	this->setWindowTitle(tr("Zip viewer: idle..."));

	auto table_view = new QTableView;
	const auto visualizer = new ZipVisualizer(table_view/*this*/);

	table_view->setModel(visualizer);
	auto hheader = table_view->horizontalHeader( );
	auto vheader = table_view->verticalHeader( );
	//hheader->setDefaultAlignment(Qt::AlignCenter);
	//vheader->setDefaultAlignment(Qt::AlignCenter);
	hheader->setSectionResizeMode(QHeaderView::ResizeMode::Stretch);
	vheader->setSectionResizeMode(QHeaderView::ResizeMode::Stretch);

	auto layout = new QFormLayout/*QVBoxLayout*/;
	ui.centralWidget->setLayout(layout);

	const auto open_dialog_button = new QPushButton("Select zip file");
	connect(open_dialog_button, &QPushButton::released, visualizer, &ZipVisualizer::fillData);
	//connect(visualizer, &mz_visualizer::FillData, table_view, &QTableView::adjustSize);

	layout->addWidget(open_dialog_button);
	layout->addWidget(table_view);

	table_view->setSizeAdjustPolicy(QAbstractScrollArea::SizeAdjustPolicy::AdjustToContents);
	//table_view->adjustSize( );
}
