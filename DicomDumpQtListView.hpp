#include "dicmlib_global.hpp"

#ifdef TW_WITH_QT_UI

#ifndef DicomDumpQtListView_h
#define DicomDumpQtListView_h

////////////////////////////////////////////////////////////////////////////////

class DcmObject;

////////////////////////////////////////////////////////////////////////////////

#include <qlistview.h>
#include <string>

////////////////////////////////////////////////////////////////////////////////

//! Items of the DicomDumpQtListView. Every node corresponds to a dicom element.
/**
	\author Tobias Wissmüller (tobias@wissmueller.net)
*/
class DicomDumpQtListViewItem : public QListViewItem
{
friend class DicomDumpQtListView;
public:

	//! Construcor.
	DicomDumpQtListViewItem(DicomDumpQtListView* parent = 0,
													const std::string& tag  = 0, const std::string& vr     = 0,
													const std::string& name = 0, const std::string& value  = 0,
													const std::string& vm   = 0, const std::string& length = 0);

	//! Destructor.
	~DicomDumpQtListViewItem();
};

////////////////////////////////////////////////////////////////////////////////

//! This is a widget for viewing the content of dicom objects.
/**
	\author Tobias Wissmüller (tobias@wissmueller.net)
*/
class DicomDumpQtListView : public QListView
{
public:

	//! Constructor.
	DicomDumpQtListView(QWidget* parent = 0, const char* name = 0, WFlags fl = 0);

	//! Destructor.
	~DicomDumpQtListView();

	//! Sets the dcmtk data.
	void setData(DcmObject* _dcmObject);

	//! Creates the view (sets the items).
	void createView();

private:
	DcmObject* dcmObject;
};

////////////////////////////////////////////////////////////////////////////////

#endif // DicomDumpQtListView_h

#endif // TW_WITH_QT_UI