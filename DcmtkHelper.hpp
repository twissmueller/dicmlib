#include "dicmlib_global.hpp"

#ifndef DcmtkHelper_h
#define DcmtkHelper_h

////////////////////////////////////////////////////////////////////////////////

class DcmElement;
class DcmObject;

#include <string>

////////////////////////////////////////////////////////////////////////////////

//! Just some helper methods that encapulate dcmtk functions.
/**
  \author Tobias Wissmüller (tobias@wissmueller.net)
*/
class DcmtkHelper
{
public:

	//! Searches an element in a dicom object. Returns only one element (there could be more!).
	static DcmElement* searchElement(DcmObject* _object, unsigned int _group, unsigned int _element); // 0x0000

private:
	DcmtkHelper();
	~DcmtkHelper();
};

////////////////////////////////////////////////////////////////////////////////

#endif // DcmtkHelper_h