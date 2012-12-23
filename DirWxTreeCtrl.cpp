#include "DirWxTreeCtrl.hpp"

#include "DicomLibException.hpp"
#include "DcmtkConvert.hpp"
#include "DcmObjectWxListCtrl.hpp"
#include <dcdicdir.h>

#include <wx/msgdlg.h>

BEGIN_EVENT_TABLE(DirWxTreeCtrl, wxTreeCtrl)
	EVT_TREE_SEL_CHANGED(DirTree_Ctrl, DirWxTreeCtrl::OnSelChanged)
END_EVENT_TABLE()

//IMPLEMENT_DYNAMIC_CLASS(DirWxTreeCtrl, wxTreeCtrl)

DirWxTreeCtrl::DirWxTreeCtrl(wxWindow* _parent, const wxWindowID id,
                       const wxPoint& pos, const wxSize& size,
                       long style)
			: wxTreeCtrl(_parent, id, pos, size, style)
{
	dumpView = NULL;
}

void DirWxTreeCtrl::setData(DcmDicomDirPtr _ddir)
{
	try
	{
		ddir = _ddir;
	}
	catch (...)
	{
		throw;
	}
}

void DirWxTreeCtrl::createView()
{
	try
	{
		this -> DeleteAllItems();
		DcmDirectoryRecord& root = ddir->getRootRecord();

		wxTreeItemId rootId = AddRoot(wxT(DcmtkConvert::DcmDirectoryRecordToString(&root).c_str()),
									  0, 0, new DirTreeItemData(&root));

		createViewRecursive(&root, rootId);
	}
	catch (...)
	{
		throw;
	}
}

void DirWxTreeCtrl::createViewRecursive(DcmDirectoryRecord* _dcmtkParent, const wxTreeItemId& _wxParent)
{
	try
	{	
		unsigned int card = _dcmtkParent->cardSub();

		for (unsigned int i=0; i<card; ++i)
		{
			DcmDirectoryRecord* tmpDcmtk = _dcmtkParent->getSub(i);

			wxTreeItemId tmpWx = AppendItem(_wxParent, DcmtkConvert::DcmDirectoryRecordToString(tmpDcmtk).c_str(),
											0, 0, new DirTreeItemData(tmpDcmtk));

			createViewRecursive(tmpDcmtk, tmpWx);
		}
	}
	catch (...)
	{
		throw;
	}
}

void DirWxTreeCtrl::OnSelChanged(wxTreeEvent& event)
{
	try
	{
		if (dumpView != NULL)
		{
			wxTreeItemId selId = event.GetItem();
			DirTreeItemData* selData = (DirTreeItemData*)(this->GetItemData(selId));
			dumpView ->setData(selData->getData());
			dumpView -> createView();
		}
	}
	catch (...)
	{
		throw;
	}
}

void DirWxTreeCtrl::setDumpView(DcmObjectWxListCtrl* _dumpView)
{
	try
	{
		dumpView = _dumpView;
	}
	catch (...)
	{
		throw;
	}
}