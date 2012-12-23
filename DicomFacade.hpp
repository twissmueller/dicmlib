#ifndef DicomFacade_h
#define DicomFacade_h

#include "dicmlib_global.hpp"

////////////////////////////////////////////////////////////////////////////////

#include <string>

////////////////////////////////////////////////////////////////////////////////

#ifdef TW_WITH_QT_UI
	class DicomDumpQtListView;
	class DicomDirQtWidget;
	class QWidget;
#endif // TW_WITH_QT_UI

#ifdef TW_WITH_WX_UI
	class wxWindow;
	class DumpWxListCtrl;
#endif // TW_WITH_WX_UI

////////////////////////////////////////////////////////////////////////////////

//! This class encapsulates the most important functions of the dicomlib
/**
	\author Tobias Wissmüller (tobias@wissmueller.net)
	\todo Implement dicomToVtk();
*/
class TW_DLL_EXPORT DicomFacade
{
public:

	//! Should return a vtkImageData object. This is not implemented, yet!
#ifdef TW_WITH_VTK
	static void dicomToVtk();
#endif // TW_WITH_VTK

	//! Returns a dump widget.
#ifdef TW_WITH_QT_UI
	static DicomDumpQtListView* getDumpWidget(const std::string& _filename, QWidget* parent = 0);
#endif // TW_WITH_QT_UI

	//! Returns a dicomdir widget.
#ifdef TW_WITH_QT_UI
	static DicomDirQtWidget* getDirWidget(const std::string& _filename, QWidget* parent = 0);
#endif // TW_WITH_QT_UI

	//! Returns a dump widget for wxWindows
#ifdef TW_WITH_WX_UI
	static DumpWxListCtrl* getDumpWxListCtrl(const std::string& _filename, wxWindow* _parent);
#endif // TW_WITH_WX_UI
};

////////////////////////////////////////////////////////////////////////////////

#endif // DicomFacade_h