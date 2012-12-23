#include "dicmlib_global.hpp"

#ifdef TW_WITH_QT_UI

#ifndef DcmtkQtWidgetFactory_h
#define DcmtkQtWidgetFactory_h

////////////////////////////////////////////////////////////////////////////////

class DcmObject;
class DcmDicomDir;


class DicomDumpQtListView;
class DicomDirQtWidget;
class DicomDirQtListView;

class QWidget;

////////////////////////////////////////////////////////////////////////////////

//! Creates Qt widgets that correspond to dcmtk dicom data structures.
/**
	\author Tobias Wissmüller (tobias@wissmueller.net)
*/
class DcmtkQtWidgetFactory
{
public:
	
	//! Creates a so called dump widget. All content of the dicom object is "dumped" into it.
	static DicomDumpQtListView* getDumpWidget(DcmObject* _dcmObject, QWidget* parent = 0);

	//! Creates a widget for viewing dicomdir files.
	static DicomDirQtWidget* getDicomDirWidget(DcmDicomDir* _dicomDir, QWidget* parent = 0);

	//! Creates a tree view that corresponds to a specific dicom directory tree.
	static DicomDirQtListView* getDicomDirListView(DcmDicomDir* _dicomDir, QWidget* parent = 0);

private:
	DcmtkQtWidgetFactory();
	~DcmtkQtWidgetFactory();
};

////////////////////////////////////////////////////////////////////////////////

#endif // DcmtkQtWidgetFactory_h

#endif // TW_WITH_QT_UI