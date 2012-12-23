#ifndef DirWxTreeCtrl_HPP
#define DirWxTreeCtrl_HPP

#include "dicmlib_global.hpp"

#include "DcmtkBuilder.hpp"

#include <wx/textctrl.h>
#include <wx/treectrl.h>

class DcmObjectWxListCtrl;

class DcmDicomDir;
class DcmDirectoryRecord;

class DirTreeItemData : public wxTreeItemData
{
public:
	DirTreeItemData(DcmDirectoryRecord* _dirRec) : dirRec(_dirRec) { }
	DcmDirectoryRecord* getData() {return dirRec;}

private:
	DcmDirectoryRecord* dirRec;
};

class TW_DLL_EXPORT DirWxTreeCtrl : public wxTreeCtrl
{
public:
	DirWxTreeCtrl() { }
    
	DirWxTreeCtrl(wxWindow *parent, const wxWindowID id,
               const wxPoint& pos, const wxSize& size,
               long style);

	void setData(DcmDicomDirPtr _ddir);
	void createView();

	void OnSelChanged(wxTreeEvent& event);

	void setDumpView(DcmObjectWxListCtrl* _dumpView);

private:
	void createViewRecursive(DcmDirectoryRecord* _dcmtkParent, const wxTreeItemId& _wxParent);

	DcmDicomDirPtr ddir;

	// when item has been selected -> send data to dumpView;
	DcmObjectWxListCtrl* dumpView;

	DECLARE_EVENT_TABLE()
};

enum
{
	DirTree_Ctrl = 0
};

#endif // DirWxTreeCtrl_HPP