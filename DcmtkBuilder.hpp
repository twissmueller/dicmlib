#ifndef DcmtkBuilder_h
#define DcmtkBuilder_h

#include "dicmlib_global.hpp"

////////////////////////////////////////////////////////////////////////////////

#include <string>
#include <memory>

////////////////////////////////////////////////////////////////////////////////

class DcmInputFileStream;
class DcmFileFormat;
class DcmMetaInfo;
class DcmDataSet;
class DcmDicomDir;
class DcmObject;
class DicomImage;

////////////////////////////////////////////////////////////////////////////////

//! This class encapsulates functions of the DICOM Toolkit.
/**
	This class encapsulates functions of the DICOM Toolkit.
	It does create data structures that correspond to DICOM object. These are
	the fileformat, metainfo, dataset and dicomdir.

	\author Tobias Wissmüller (tobias@wissmueller.net)
	\todo Implement createMetaInfo()
	\todo Implement createDataset()
*/

#define DcmInputFileStreamPtr	std::auto_ptr<DcmInputFileStream>
#define DcmFileFormatPtr		std::auto_ptr<DcmFileFormat>
#define DcmDicomDirPtr			std::auto_ptr<DcmDicomDir>
//#define DicomImagePtr			std::auto_ptr<DicomImage>

	/*DcmInputFileStream* file;
	DcmFileFormat* fileFormat;
	DcmMetaInfo* metaInfo;
	DcmDataSet* dataset;
	DcmDicomDir* dicomDir;
	DcmObject* dicomObject;*/

class TW_DLL_EXPORT DcmtkBuilder
{
public:
	DcmtkBuilder();

	void setFilename(const std::string& _filename);

	DcmFileFormatPtr getFileFormat();
	DcmDicomDirPtr getDicomDirPtr();
	DcmDicomDir* getDicomDir();
	//DicomImagePtr getDicomImage();

protected:
	void createFileFormat();
	void createMetaInfo();
	void createDataset();
	void createDicomDirPtr();
	void createDicomDir();
	//void createDicomImage();

	std::string filename;

	DcmInputFileStreamPtr file;
	DcmFileFormatPtr fileFormat;
	DcmDicomDirPtr dicomDirPtr;
	DcmDicomDir* dicomDir;
//	DicomImagePtr dicomImage;

	//DcmMetaInfo* metaInfo;
	//DcmDataSet* dataset;
};

////////////////////////////////////////////////////////////////////////////////

#endif // DcmtkBuilder_h