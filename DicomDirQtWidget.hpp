#include "dicmlib_global.hpp"

#ifdef TW_WITH_QT_UI

#ifndef DicomDirQtWidget_h
#define DicomDirQtWidget_h

////////////////////////////////////////////////////////////////////////////////

#include <qwidget.h>

class QHBoxLayout;
class QVBoxLayout;
class QGroupBox;
class QWidgetStack;
class QListViewItem;
class QStringList;

class DcmDicomDir;
class DicomDirQtListView;
class DicomDirQtListViewItem;

////////////////////////////////////////////////////////////////////////////////

//! A widget for browsing dicomdir files.
/**
	\author Tobias Wissmüller (tobias@wissmueller.net)
*/
class DicomDirQtWidget : public QWidget
{
	Q_OBJECT

public:

	//! Constructor.
	DicomDirQtWidget(QWidget* parent = 0, const char* name = 0, WFlags fl = 0);

	//! Destructor.
	~DicomDirQtWidget();

	//! Sets the dcmtk data
	void setData(DcmDicomDir* _dicomDir);

	//! Creates all child widgets.
	void createView();

	//! Returns the file names of the selected image directory records
	QStringList getSelected();

protected slots:
  void itemSelected(QListViewItem* _viewItem);

private:
	DcmDicomDir* dicomDir;

	DicomDirQtListView* listView;
	QWidgetStack* stack;

	QHBoxLayout* mainLayout;
	QGroupBox* treeBox;
	QVBoxLayout* treeLayout;
	QGroupBox* stackBox;
	QVBoxLayout* stackLayout;
};

////////////////////////////////////////////////////////////////////////////////

#endif // DicomDirQtWidget_h

#endif // TW_WITH_QT_UI