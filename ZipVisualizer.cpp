#include "ZipVisualizer.h"

#include <QApplication>
#include <qfiledialog.h>
#include <qmessagebox.h>
#include <qobject.h>

ZipVisualizer::ZipVisualizer(QWidget* parent): QAbstractTableModel(parent)
{
	QAbstractTableModel::setHeaderData(0, Qt::Orientation::Horizontal, "file inside archive");
	QAbstractTableModel::setHeaderData(1, Qt::Orientation::Horizontal, "uncompressed size");
	QAbstractTableModel::setHeaderData(2, Qt::Orientation::Horizontal, "compressed size");
}

auto ZipVisualizer::rowCount(const QModelIndex& parent) const -> int
{
	return m_cache.size( );
}

auto ZipVisualizer::columnCount(const QModelIndex&) const -> int
{
	return 3;
}

auto ZipVisualizer::headerData(int section, Qt::Orientation orientation, int role) const -> QVariant
{
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
	{
		switch (section)
		{
			case 0: return tr("file inside archive");
			case 1: return tr("uncompressed size");
			case 2: return tr("compressed size");
			default:
				assert(0);
				break;
		}
	}

	return QAbstractTableModel::headerData(section, orientation, role);
}

auto ZipVisualizer::data(const QModelIndex& index, int role) const -> QVariant
{
	if (role == Qt::DisplayRole)
	{
		if (!m_cache.empty( ))
		{
			const auto& [text, data] = m_cache[index.row( )];

			switch (index.column( ))
			{
				case 0: return text;
				case 1: return QString("%1 bytes").arg(data.uncompressed_size);
				case 2: return QString("%1 bytes").arg(data.compressed_size);
				default:
					assert(0);
					break;
			}
		}
	}

	return QVariant( );
}

auto ZipVisualizer::fillData( ) -> void
{
	const auto parent = static_cast<QWidget*>(this->parent( ));
	auto file_path = QFileDialog::getOpenFileName(parent, tr("caption"), nullptr, tr("Zip archive files (*.zip)"));
	//todo: detect zip update time
	const auto update = m_cache.empty( ) || m_last_file_path != file_path;
	if (!update)
		return;

	//-------------------

	const auto showMsgBox = [&](const QString& error_msg) -> void
	{
		QMessageBox::warning(parent, "Warning", QString("Unable to load file! %1").arg(error_msg));
	};

	const auto unz_file = unzOpen(file_path.toUtf8( ).constData( ));
	if (unz_file == nullptr)
		return showMsgBox("Zip file open error!");

	//-------------------

	unz_global_info unz_info = { };
	if (unzGetGlobalInfo(unz_file, &unz_info) != MZ_OK)
		return showMsgBox("Zip file is corrupt!");

	//-------------------

	cache_type temp_cache;

	const auto last_entry = unz_info.number_entry;
	const auto prev_last_entry = last_entry - 1u;
	for (decltype(unz_info.number_entry) i = 0; i < last_entry; ++i)
	{
		// Get info about current file.
		unz_file_info zip_file_info;

		char filename[255];
		if (unzGetCurrentFileInfo(unz_file, &zip_file_info, filename, sizeof(filename), nullptr, 0, nullptr, 0) != UNZ_OK)
		{
			return showMsgBox("Unable to read file info!");
		}

		const auto filename_sv = std::string_view(filename);
		const auto filename_back = filename_sv.back( );
		if (filename_back != '\\' && filename_back != '/') //or use mz_path_has_slash
		{
			//file detected

			auto qstr = QString::fromUtf8(filename_sv.data( ), filename_sv.size( ));
			cache_type::value_type entry; //QPair have no rvalue constructor
			entry.first = qMove(qstr);
			entry.second = zip_file_info;
			temp_cache.push_back(qMove(entry));
			//info.push_back(QPair(qMove(str), zip_file_info));
		}
		else
		{
			//directory detected
		}

		if (i != prev_last_entry && unzGoToNextFile(unz_file) != UNZ_OK)
		{
			return showMsgBox("Unable to get next archived file!");
		}
	}

	if (temp_cache.empty( ))
		return showMsgBox("Zip file have no entries!");

	unzClose(unz_file);

	//-------------------

	this->layoutChanged( );
	QWidget* window = nullptr;
	auto top = QApplication::topLevelWidgets( );
	if (top.size( ) == 1)
		window = top[0];
	else
	{
		for (auto& w: top)
		{
			if (w->windowTitle( ).startsWith("Zip viewer: "))
			{
				window = (w);
				break;
			}
		}
	}

	window->setWindowTitle(tr("Zip viewer: %1").arg(file_path));

	//-------------------

	m_last_file_path = qMove(file_path);
	m_cache = qMove(temp_cache);
}
