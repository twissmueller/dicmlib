#include "dicmlib_global.hpp"

#ifndef DcmtkConvert_h
#define DcmtkConvert_h

////////////////////////////////////////////////////////////////////////////////

#include <string>

////////////////////////////////////////////////////////////////////////////////

class DcmElement;
class DcmDirectoryRecord;

////////////////////////////////////////////////////////////////////////////////

//! Converts dcmtk data structures to C++ data structures.
/**
	\author Tobias Wissmüller (tobias@wissmueller.net)
*/
class DcmtkConvert
{
public:

	//! Takes a dicom element and converts its value to a string.
	static std::string DcmElementValueToString(DcmElement* _element);

	//! Converts the type of a directory record to a string (patient, study, series, etc.).
	static std::string DcmDirectoryRecordToString(DcmDirectoryRecord* _dirRecord);

	//! Returns the associated filename of an image directory record.
	static std::string DcmImageRecordToFileName(DcmDirectoryRecord* _dirRecord);

private:
	DcmtkConvert();
	~DcmtkConvert();

	static std::string padNum(const std::string& _num);
};

////////////////////////////////////////////////////////////////////////////////

#endif // DcmtkConvert_h