#include "dicmlib_global.hpp"

#ifdef TW_WITH_QT_UI

#ifndef DicomDirQtListView_h
#define DicomDirQtListView_h

////////////////////////////////////////////////////////////////////////////////

#include <qlistview.h>
#include <vector>

////////////////////////////////////////////////////////////////////////////////

class DcmDicomDir;
class DcmDirectoryRecord;

////////////////////////////////////////////////////////////////////////////////

//! The corresponding items of the DicomDirQtListView. Every node corresponds to a dicom directory record.
/**
	\author Tobias Wissmüller (tobias@wissmueller.net)
*/
class DicomDirQtListViewItem : public QListViewItem
{
friend class DicomDirQtListView;
public:

	//! Constructor.
	DicomDirQtListViewItem(DcmDirectoryRecord* _dirRec, DicomDirQtListView* parent = 0, const char* text = "not set");

	//! Constructor.
	DicomDirQtListViewItem(DcmDirectoryRecord* _dirRec, DicomDirQtListViewItem* parent, const char* text = "not set");

	//! Destructor.
	~DicomDirQtListViewItem();

	//! Returns the dcmtk data of that node.
	DcmDirectoryRecord* getData() { return dirRec; }

private:
	DcmDirectoryRecord* dirRec;
};

////////////////////////////////////////////////////////////////////////////////

//! Represents a dicomdir
/**
	\author Tobias Wissmüller (tobias@wissmueller.net)
*/
class DicomDirQtListView : public QListView
{
public:

	//! Constructor.
	DicomDirQtListView(QWidget* parent = 0, const char* name = 0, WFlags fl = 0);

	//! Destructor.
	~DicomDirQtListView();

	//! Sets dcmtk data.
	void setData(DcmDicomDir* _dicomDir);

	//! Creates the view (sets the items).
	void createView();

	//! Creates the view (sets the items) recursively, called by createView().
	void traverseDirRecord(DcmDirectoryRecord* _dirRecord, DicomDirQtListViewItem* _viewItem);

	//! Returns the selected items of the list view.
	std::vector<DicomDirQtListViewItem*> getSelectedItems();

	//! Searches all selected items in the list view recursively, called by getSelectedItems().
	void searchRec(DicomDirQtListViewItem* _item);

private:
	DcmDicomDir* dicomDir;
	std::vector<DicomDirQtListViewItem*> searchResult;
};

////////////////////////////////////////////////////////////////////////////////

#endif // DicomDirQtListView_h

#endif // TW_WITH_QT_UI