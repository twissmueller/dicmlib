#include "DcmObjectWxListCtrl.hpp"

#include "DcmtkConvert.hpp"
#include "DicomLibException.hpp"

#include <vector>
#include <sstream>

#include <dcobject.h>
#include <dcelem.h>

DcmObjectWxListCtrl::DcmObjectWxListCtrl(wxWindow* _parent)
{
	Create(_parent, -1, wxDefaultPosition, wxDefaultSize, wxLC_REPORT, wxDefaultValidator, wxT("DcmObjectlistctrl"));

	this->InsertColumn(0, _T("Tag"));
	this->InsertColumn(1, _T("VR"));
	this->InsertColumn(2, _T("Name"));
	this->InsertColumn(3, _T("Value"));
	this->InsertColumn(4, _T("VM"));
	this->InsertColumn(5, _T("Length"));

	this->SetColumnWidth(0, 70);
	this->SetColumnWidth(1, 30);
	this->SetColumnWidth(2, 200);
	this->SetColumnWidth(3, 300);
	this->SetColumnWidth(4, 30);
	this->SetColumnWidth(5, 60);
}

void DcmObjectWxListCtrl::setData(DcmObject* _obj)
{
	try
	{
		this->clear();
		obj = _obj;
	}
	catch (...)
	{
		throw;
	}
}

void DcmObjectWxListCtrl::createView()
{
	try
	{
		std::vector<std::string> tag;
		std::vector<std::string> vr;
		std::vector<std::string> name;
		std::vector<std::string> value;
		std::vector<std::string> vm;
		std::vector<std::string> length;

		DcmStack stack;
		DcmEVR devr; 
		DcmObject* dobj = NULL;

		int count = 0;
		while ( (obj->nextObject(stack, 1)).good() )
		{
			dobj = stack.elem(0);
			devr = dobj->ident();

			/*if ((devr != EVR_ox) && (devr != EVR_xs) &&
				(devr != EVR_na) && (devr != EVR_up) &&
				(devr != EVR_item) && (devr != EVR_metainfo) &&
				(devr != EVR_dataset) && (devr != EVR_fileFormat) &&
				(devr != EVR_dicomDir) && (devr != EVR_dirRecord) &&
				(devr != EVR_pixelSQ) && (devr != EVR_pixelItem) &&
				(devr != EVR_UNKNOWN) && (devr != EVR_PixelData) &&
				(devr != EVR_OverlayData) && (devr != EVR_UNKNOWN2B))
			{*/
				DcmTag dtag;
				dtag = dobj->getTag();
					
				DcmVR dvr  = dtag.getVR();

				unsigned int tmpVm = dobj->getVM();
				std::stringstream vmStream;
				vmStream << tmpVm;
						
				unsigned int tmpLength = dobj->getLength();
				std::stringstream lengthStream;
				lengthStream << tmpLength;

				this->InsertItem(count, dtag.toString().c_str());
				this->SetItem(count, 1, dvr.getValidVRName());
				this->SetItem(count, 2, dtag.getTagName());
				this->SetItem(count, 3, (DcmtkConvert::DcmElementValueToString((DcmElement*)dobj)).c_str());
				this->SetItem(count, 4, vmStream.str().c_str());
				this->SetItem(count, 5, lengthStream.str().c_str());

				++count;
			/*}*/
		}
	}
	catch (...)
	{
		throw;
	}
}

void DcmObjectWxListCtrl::clear()
{
	try
	{
		this->DeleteAllItems();
	}
	catch (...)
	{
		throw;
	}
}