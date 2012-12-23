#ifndef DicomLibException_h
#define DicomLibException_h

#include "dicmlib_global.hpp"

#include <string>

//! This is the exception class of the dicomlib
/**
	\author Tobias Wissmüller (tobias@wissmueller.net)
*/
class TW_DLL_EXPORT DicomLibException
{
public:

	//! Constructor
	DicomLibException(std::string _msg="", std::string _srcfile="", unsigned int _srcline=0)
		: msg(_msg), srcfile(_srcfile), srcline(_srcline) {}

	//! Returns the exception description.
	inline std::string what() {return msg;}

	//! Returns the file where the exception has been thrown.
	inline std::string file() {return srcfile;}

	//! Returns the line of the file where the exception has been thrown.
	inline unsigned int line() {return srcline;}

private:
	std::string msg;
	std::string srcfile;
	unsigned int srcline;
};

#endif // DicomLibException_h