#ifndef DicomToVtk_hpp
#define DicomToVtk_hpp

#include "dicmlib_global.hpp"

//#ifdef TW_WITH_VTK

////////////////////////////////////////////////////////////////////////////////

class vtkImageData;

////////////////////////////////////////////////////////////////////////////////

#include <string>

////////////////////////////////////////////////////////////////////////////////

//! Converts a dicom file to a vtkImageData
/**
	\author Tobias Wissmüller (tobias@wissmueller.net)
*/
class DicomToVtk
{
public:

	//! Converts the file and returns the vktImageData.
	static vtkImageData* convert(const std::string& _filename);

private:
	std::string filename;
	vtkImageData* image;
};

////////////////////////////////////////////////////////////////////////////////

#endif // DicomToVtk_hpp

//#endif // TW_WITH_VTK