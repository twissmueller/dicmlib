#ifndef DicomImporter_hpp
#define DicomImporter_hpp

#include <string>
#include <vector>

//#include <ofstd.h>
//
class DcmDicomDir;
//class DcmFileFormat;
//class OFString;
//class DcmItem;
//class DcmTagKey;

class DicomImporter
{
	typedef std::vector<std::string> VectorOfStrings;
	typedef std::vector<std::string>::const_iterator VectorOfStringsIterator;
	typedef std::string String;

public:
	static DcmDicomDir* Import(String& _path, VectorOfStrings& _files);

private:
	DicomImporter();
	static bool IsFileValid(const String& _file);
	static void DetermineSopClass(const String& _file);

	static VectorOfStrings files;
	static VectorOfStrings validFiles;
	static VectorOfStrings nonValidFiles;
	static VectorOfStrings dicomdirFiles;
	static VectorOfStrings nonDicomdirFiles;

	//DcmDicomDir* ImportFromDicomdir(const std::string& _filename);
	//DcmDicomDir* ImportFromFileList(const std::vector<std::string>& _filenames);
	//DcmDicomDir* ImportFromScannedDir(const std::string& _dirname);
};

#endif // DicomImporter_hpp