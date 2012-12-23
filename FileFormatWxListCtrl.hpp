#ifndef FileFormatWxListCtrl_HPP
#define FileFormatWxListCtrl_HPP

#include "dicmlib_global.hpp"

#include <memory>

#include <wx/textctrl.h>
#include <wx/listctrl.h>

#include "DcmtkBuilder.hpp"

class TW_DLL_EXPORT FileFormatWxListCtrl : public wxListCtrl
{
public:
	FileFormatWxListCtrl(wxWindow* _parent);

	void setData(DcmFileFormatPtr _fFormat);
	void createView();

private:
	DcmFileFormatPtr fFormat;

	void clear();
};

#endif // FileFormatWxListCtrl_HPP