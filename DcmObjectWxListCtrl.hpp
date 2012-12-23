#ifndef DcmObjectWxListCtrl_HPP
#define DcmObjectWxListCtrl_HPP

#include "dicmlib_global.hpp"

#include <memory>

#include <wx/textctrl.h>
#include <wx/listctrl.h>

class DcmObject;

class TW_DLL_EXPORT DcmObjectWxListCtrl : public wxListCtrl
{
public:
	DcmObjectWxListCtrl(wxWindow* _parent);

	void setData(DcmObject* _obj);
	void createView();

private:
	DcmObject* obj;

	void clear();
};

#endif // DcmObjectWxListCtrl_HPP