#ifndef ZIP_VISUALIZER_H
#define ZIP_VISUALIZER_H

#include "minizip/mz_compat.h"

#include <qabstractitemmodel.h>

class ZipVisualizer final: public /*QTableView*/QAbstractTableModel
{
public:
	using cache_type = QVector<QPair<QString, unz_file_info>>;

	ZipVisualizer(QWidget* parent);
	auto rowCount(const QModelIndex& parent) const -> int override;
	auto columnCount(const QModelIndex&) const -> int override;
	auto headerData(int section, Qt::Orientation orientation, int role) const -> QVariant override;
	auto data(const QModelIndex& index, int role) const -> QVariant override;
	auto fillData( ) -> void;

private:
	cache_type m_cache;
	QString m_last_file_path;
};

#endif // !ZIP_VISUALIZER_H
#define ZIP_VISUALIZER_H
