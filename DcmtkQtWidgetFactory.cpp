#ifdef TW_WITH_QT_UI

#include "DcmtkQtWidgetFactory.h"

#include <dcobject.h>
#include <qwidget.h>

#include "DicomLibException.h"
#include "DicomDumpQtListView.h"
#include "DicomDirQtWidget.h"
#include "DicomDirQtListView.h"

DicomDumpQtListView* DcmtkQtWidgetFactory::getDumpWidget(DcmObject* _dcmObject, QWidget* _parent)
{
	try
	{
		if (_dcmObject == NULL)
		{
			throw DicomLibException("No data!", __FILE__, __LINE__);
		}

		DicomDumpQtListView* dicomDumpListView = new DicomDumpQtListView();
		dicomDumpListView -> setData(_dcmObject);
		dicomDumpListView -> createView();

		return dicomDumpListView;
	}
	catch (...)
	{
		throw;
	}
}


DicomDirQtWidget* DcmtkQtWidgetFactory::getDicomDirWidget(DcmDicomDir* _dicomDir, QWidget* _parent)
{
	try
	{	
		if (_dicomDir == NULL)
		{
			throw DicomLibException("No data!", __FILE__, __LINE__);
		}

		DicomDirQtWidget* dicomDirWidget = new DicomDirQtWidget(_parent);
		dicomDirWidget -> setData(_dicomDir);
		dicomDirWidget -> createView();

		return dicomDirWidget;
	}
	catch (...)
	{
		throw;
	}
}

DicomDirQtListView* DcmtkQtWidgetFactory::getDicomDirListView(DcmDicomDir* _dicomDir, QWidget* _parent)
{
	try
	{
		if (_dicomDir == NULL)
		{
			throw DicomLibException("No data!", __FILE__, __LINE__);
		}

		DicomDirQtListView* dicomDirListView = new DicomDirQtListView(_parent);
		dicomDirListView -> setData(_dicomDir);
		dicomDirListView -> createView();

		return dicomDirListView;
	}
	catch (...)
	{
		throw;
	}
}

DcmtkQtWidgetFactory::DcmtkQtWidgetFactory()
{}

DcmtkQtWidgetFactory::~DcmtkQtWidgetFactory()
{}

#endif // TW_WITH_QT_UI