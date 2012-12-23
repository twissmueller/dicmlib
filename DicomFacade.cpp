#include "DicomFacade.hpp"

#include "DcmtkBuilder.hpp"
#include "DicomLibException.hpp"

#ifdef TW_WITH_QT_UI 
	#include "DcmtkQtWidgetFactory.hpp"
	#include "DicomDumpQtListView.hpp"
	#include "DicomDirQtWidget.hpp"
#endif // TW_WITH_QT_UI

#ifdef TW_WITH_WX_UI
	#include "DumpWxListCtrl.hpp"
#endif // TW_WITH_WX_UI

#ifdef TW_WITH_VTK
void DicomFacade::dicomToVtk()
{
	try
	{
		throw DicomLibException("Not implemented!", __FILE__, __LINE__);
	}
	catch (...)
	{
		throw;
	}
}
#endif // TW_WITH_VTK

#ifdef TW_WITH_QT_UI
DicomDumpQtListView* DicomFacade::getDumpWidget(const std::string& _filename, QWidget* _parent)
{
	try
	{	
		DcmtkBuilder builder;
		builder.setFilename(_filename);
		builder.readFile();
		builder.createFileFormat();

		return DcmtkQtWidgetFactory::getDumpWidget(reinterpret_cast<DcmObject*>(builder.getFileFormat()));
	}
	catch (...)
	{
		throw;
	}
}


#endif // TW_WITH_QT_UI

#ifdef TW_WITH_QT_UI
DicomDirQtWidget* DicomFacade::getDirWidget(const std::string& _filename, QWidget* _parent)
{
	try
	{
		DcmtkBuilder builder;
		builder.setFilename(_filename);
		builder.createDicomDir();
		
		return DcmtkQtWidgetFactory::getDicomDirWidget(builder.getDicomDir(), _parent);
	}
	catch (...)
	{
		throw;
	}
}
#endif // TW_WITH_QT_UI

//#ifdef TW_WITH_WX_UI
DumpWxListCtrl* DicomFacade::getDumpWxListCtrl(const std::string& _filename, wxWindow* _parent)
{
	try
	{
		return new DumpWxListCtrl(_parent);
	}
	catch (...)
	{
		throw;
	}
}
//#endif // TW_WITH_WX_UI