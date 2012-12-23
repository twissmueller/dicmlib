#ifdef TW_WITH_QT_UI

#include "DicomDumpQtListView.h"

#include "DcmtkConvert.h"
#include "DicomLibException.h"

#include <vector>
#include <sstream>

#include <dcobject.h>
#include <dcelem.h>

////////////////////////////////////////////////////////////////////////////////

DicomDumpQtListViewItem::DicomDumpQtListViewItem(
												 DicomDumpQtListView* parent,
												 const std::string& tag,  const std::string& vr,
												 const std::string& name, const std::string& value,
												 const std::string& vm,   const std::string& length)
	: QListViewItem(parent, tag.c_str(), vr.c_str(), name.c_str(),
		                      value.c_str(), vm.c_str(), length.c_str())
{}

////////////////////////////////////////////////////////////////////////////////

DicomDumpQtListViewItem::~DicomDumpQtListViewItem()
{}

////////////////////////////////////////////////////////////////////////////////

DicomDumpQtListView::DicomDumpQtListView(QWidget* parent, const char* name, WFlags fl)
	: QListView(parent, name, fl)
{
	this->addColumn("Tag");
	this->addColumn("VR");
	this->addColumn("Name");
	this->addColumn("Value");
	this->addColumn("VM");
	this->addColumn("Length");

	this->setRootIsDecorated(false);
	this->setSorting(-1);

	dcmObject = NULL;
}

////////////////////////////////////////////////////////////////////////////////

DicomDumpQtListView::~DicomDumpQtListView()
{}

////////////////////////////////////////////////////////////////////////////////

void DicomDumpQtListView::setData(DcmObject* _dcmObject)
{
	try
	{
		if (_dcmObject == NULL)
		{
			throw DicomLibException("No data!", __FILE__, __LINE__);
		}

		dcmObject = _dcmObject;
	}
	catch (...)
	{
		throw;
	}
}

////////////////////////////////////////////////////////////////////////////////

void DicomDumpQtListView::createView()
{
	try
	{
		if (dcmObject == NULL)
		{
			throw DicomLibException("No data!", __FILE__, __LINE__);
		}

		std::vector<std::string> tag;
		std::vector<std::string> vr;
		std::vector<std::string> name;
		std::vector<std::string> value;
		std::vector<std::string> vm;
		std::vector<std::string> length;

		DcmStack stack;
		DcmEVR devr; 
		DcmObject* dobj = NULL;

		while ( (dcmObject->nextObject(stack, 1)).good() )
		{
			dobj = stack.elem(0);
			devr = dobj->ident();

			if ((devr != EVR_ox) && (devr != EVR_xs) &&
					(devr != EVR_na) && (devr != EVR_up) &&
					(devr != EVR_item) && (devr != EVR_metainfo) &&
					(devr != EVR_dataset) && (devr != EVR_fileFormat) &&
					(devr != EVR_dicomDir) && (devr != EVR_dirRecord) &&
					(devr != EVR_pixelSQ) && (devr != EVR_pixelItem) &&
					(devr != EVR_UNKNOWN) && (devr != EVR_PixelData) &&
					(devr != EVR_OverlayData) && (devr != EVR_UNKNOWN2B))
			{
				DcmTag dtag;
				dtag = dobj->getTag();
				
				DcmVR dvr  = dtag.getVR();

				unsigned int tmpVm = dobj->getVM();
				std::stringstream vmStream;
				vmStream << tmpVm;
				
				unsigned int tmpLength = dobj->getLength();
				std::stringstream lengthStream;
				lengthStream << tmpLength;

				tag.push_back(dtag.toString().c_str());
				vr.push_back(dvr.getValidVRName());
				name.push_back(dtag.getTagName());
				value.push_back(DcmtkConvert::DcmElementValueToString((DcmElement*)dobj));
				vm.push_back(vmStream.str().c_str());
				length.push_back(lengthStream.str().c_str());
			}
		}

		DicomDumpQtListViewItem* tmpItem = NULL;

		std::vector<std::string>::size_type size = tag.size();
		for (int i=size; i>0; i--)
		{
			tmpItem = new DicomDumpQtListViewItem(this,
																						tag[i-1], vr[i-1], name[i-1],
																						value[i-1], vm[i-1], length[i-1]);
		}
	}
	catch (...)
	{
		throw;
	}
}

////////////////////////////////////////////////////////////////////////////////

#endif // TW_WITH_QT_UI