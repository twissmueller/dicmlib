#ifndef DicomdirFactory_hpp
#define DicomdirFactory_hpp

//#define BUILD_DCMGPDIR_AS_DCMMKDIR
#define WITH_ZLIB

#include "osconfig.h"    

#define INCLUDE_CSTDLIB
#define INCLUDE_CSTDIO
#define INCLUDE_CSTRING
#define INCLUDE_CCTYPE
#include "ofstdinc.h"

#include <windows.h>
#include <string>
#include <vector>

#include "ofstream.h"
#include "ofconapp.h"
#include "ofstring.h"
#include "oflist.h"
#include "ofbmanip.h"
#include "ofstd.h"

#include "dctk.h"
#include "dcdebug.h"
#include "dcuid.h"         // for dcmtk version name
#include "cmdlnarg.h"
#include "dcvrda.h"
#include "dcvrtm.h"

#ifdef BUILD_DCMGPDIR_AS_DCMMKDIR
# include "dcpxitem.h"     // for class DcmPixelItem
# include "dcmimage.h"     // for class DicomImage
# include "diregist.h"     // include to support color images
# include "discalet.h"     // for direct image scaling
# include "djdecode.h"     // for dcmjpeg decoders
# include "dipijpeg.h"     // for dcmimage JPEG plugin
#endif

#include <zlib.h>         // for zlibVersion()

# define OFFIS_CONSOLE_APPLICATION "dcmmkdir"
# define OFFIS_CONSOLE_DESCRIPTION "Create a DICOMDIR file"

static char rcsid[] = "$dcmtk: " OFFIS_CONSOLE_APPLICATION " v"
  OFFIS_DCMTK_VERSION " " OFFIS_DCMTK_RELEASEDATE " $";

#define DEFAULT_FSID "DCMTK_MEDIA_DEMO"	// default File-Set ID
#define DEFAULT_FSDFID "README"			// default File-Set Descriptor File ID
#define DEFAULT_SCSFSDF "ISO_IR 100"	// default Specific Character Set of File-Set Descriptor File

#define MAX_FNAME_COMPONENT_SIZE 8		// an ISO 9660 format only allows 8 characters in file name
#define MAX_FNAME_COMPONENTS 8			// DICOM only allows max 8 path components in a file name

#define SHORTCOL 4
#define LONGCOL 22

class DicomdirFactory
{
private:
	// list of supported profiles
	enum E_DicomDirProfile {
		EDDP_None,
		EDDP_GeneralPurpose,
		EDDP_BasicCardiac,
		EDDP_XrayAngiographic,
		EDDP_CTandMR,
		EDDP_UltrasoundIDSF,
		EDDP_UltrasoundSCSF,
		EDDP_UltrasoundCCSF,
		EDDP_UltrasoundIDMF,
		EDDP_UltrasoundSCMF,
		EDDP_UltrasoundCCMF,
		EDDP_TwelveLeadECG,
		EDDP_HemodynamicWaveform
	};

	OFString ofname;

	// default profile
	E_DicomDirProfile dicomdirProfile;

	// actual File-Set ID
	OFString fsid;
	// actual File-Set Descriptor File ID
	OFString fsdfid; // can be set to DEFAULT_FSDFID during option handling
	// actual Specific Character Set of File-Set Descriptor File
	OFString scsfsdf;

	// external/default icons
	OFString iconPrefix;
	OFString defaultIcon;

	// various command line options
	OFBool verbosemode;
	OFBool writeDicomdir;
	OFBool appendMode;
	OFBool createBackupFile;
	OFBool inventAttributes;
	OFBool mapFilenames;
	OFBool recurseFilesystem;
	OFBool resolutionCheck;
	OFBool addIconImage;
	OFBool inventPatientID;
	OFBool abortInconsistFile;

	E_EncodingType lengthEncoding;
	E_GrpLenEncoding groupLengthEncoding;

	std::string path;

public:
	DicomdirFactory();
	DcmDicomDir* Create(std::string& _path, const std::vector<std::string>& _filenames);
private:
	const char* getProfileName(E_DicomDirProfile profile);
	OFBool isaValidCharSetName(const OFString& cs);
	OFBool dcmTagExists(DcmItem* d, const DcmTagKey& key);
	OFBool dcmTagExistsWithValue(DcmItem* d, const DcmTagKey& key);
	OFString dcmFindString(DcmItem* d, const DcmTagKey& key, OFBool searchIntoSub = OFFalse);
#ifdef BUILD_DCMGPDIR_AS_DCMMKDIR
	long dcmFindInteger(DcmItem* d, const DcmTagKey& key, OFBool searchIntoSub = OFFalse);
#endif
	// dataset manipulation
	OFString dcmFindStringInFile(const OFString& fname, const DcmTagKey& key, OFBool searchIntoSub = OFFalse);
	OFBool dcmInsertString(DcmItem* d, const DcmTagKey& key, const OFString& s, OFBool replaceOld = OFTrue);
	OFBool dcmInsertInteger(DcmItem* d, const DcmTagKey& key, const long i, OFBool replaceOld = OFTrue);
#ifdef BUILD_DCMGPDIR_AS_DCMMKDIR
	OFBool dcmCopyInteger(DcmItem* sink, const DcmTagKey& key, DcmItem* source);
#endif
	OFBool dcmCopyString(DcmItem* sink, const DcmTagKey& key, DcmItem* source);
	OFBool dcmCopyOptString(DcmItem* sink, const DcmTagKey& key, DcmItem* source);
#ifdef BUILD_DCMGPDIR_AS_DCMMKDIR
	OFBool dcmCopyStringWithDefault(DcmItem* sink, const DcmTagKey& key, DcmItem* source, const char* defaultString = "");
#endif
	OFBool dcmCopySequence(DcmItem* sink, const DcmTagKey& key, DcmItem* source);
	OFBool dcmCopyOptSequence(DcmItem* sink, const DcmTagKey& key, DcmItem* source);
	OFString currentDate();
	OFString currentTime();
	OFString alternativeStudyDate(DcmItem* d);
	OFString alternativeStudyTime(DcmItem* d);
	OFString defaultNumber(int number);
	OFString defaultID(const OFString& prefix, int number);
	// filename manipulation
	void hostToDicomFilename(OFString& fname);
	void dicomToHostFilename(OFString& fname, OFBool mapToLower = OFFalse);
	int componentCount(const OFString& fname, char separator = PATH_SEPARATOR);
	OFBool isComponentTooLarge(const OFString& fname, int componentLimit, char separator = PATH_SEPARATOR);
	// check, help functions
	OFBool cmp(const OFString& s1, const OFString& s2);
	OFBool checkExists(DcmItem* d, const DcmTagKey& key, const OFString& fname);
	OFBool checkExistsWithValue(DcmItem* d, const DcmTagKey& key, const OFString& fname);
#ifdef BUILD_DCMGPDIR_AS_DCMMKDIR
	OFBool checkExistsWithStringValue(DcmItem* d, const DcmTagKey& key, const OFString &value, const OFString& fname, OFBool normalize = OFTrue);
	OFBool checkExistsWithIntegerValue(DcmItem* d, const DcmTagKey& key, const long value, const OFString& fname);
	OFBool checkExistsWithMinMaxValue(DcmItem* d, const DcmTagKey& key, const long min, const long max, const OFString& fname, const OFBool reject = OFTrue);
#endif
	OFString recordTypeToName(E_DirRecType t);
	OFBool checkImage(const OFString& fname, DcmFileFormat *ff);
	void addConceptModContentItems(DcmItem* d, DcmDirectoryRecord* &rec);
	DcmDirectoryRecord* buildPatientRecord(const OFString& referencedFileName, DcmItem* d, const OFString& sourceFileName);
	DcmDirectoryRecord* buildStudyRecord(const OFString& referencedFileName, DcmItem* d, const OFString& sourceFileName);
	DcmDirectoryRecord* buildSeriesRecord(const OFString& referencedFileName, DcmItem* d, const OFString& sourceFileName);
#ifdef BUILD_DCMGPDIR_AS_DCMMKDIR
	OFBool getExternalIcon(const OFString &filename, Uint8 *&pixel, const unsigned long count, const unsigned long width, const unsigned long height);
	OFBool getIconFromDataset(DcmItem *d, Uint8 *&pixel, const unsigned long count, const unsigned long width, const unsigned long height);
#endif
	DcmDirectoryRecord* buildImageRecord(const OFString& referencedFileName, DcmItem* d, const OFString& sourceFileName);
	DcmDirectoryRecord* buildOverlayRecord(const OFString& referencedFileName, DcmItem* d, const OFString& sourceFileName);
	DcmDirectoryRecord* buildModalityLutRecord(const OFString& referencedFileName, DcmItem* d, const OFString& sourceFileName);
	DcmDirectoryRecord* buildVoiLutRecord(const OFString& referencedFileName, DcmItem* d, const OFString& sourceFileName);
	DcmDirectoryRecord* buildCurveRecord(const OFString& referencedFileName, DcmItem* d, const OFString& sourceFileName);
	DcmDirectoryRecord* buildStructReportRecord(const OFString& referencedFileName, DcmItem* d, const OFString& sourceFileName);
	DcmDirectoryRecord* buildPresentationRecord(const OFString& referencedFileName, DcmItem* d, const OFString& sourceFileName);
	DcmDirectoryRecord* buildWaveformRecord(const OFString& referencedFileName, DcmItem* d, const OFString& sourceFileName);
	DcmDirectoryRecord* buildRTDoseRecord(const OFString& referencedFileName, DcmItem* d, const OFString& sourceFileName);
	DcmDirectoryRecord* buildRTStructureSetRecord(const OFString& referencedFileName, DcmItem* d, const OFString& sourceFileName);
	DcmDirectoryRecord* buildRTPlanRecord(const OFString& referencedFileName, DcmItem* d, const OFString& sourceFileName);
	DcmDirectoryRecord* buildRTTreatmentRecord(const OFString& referencedFileName, DcmItem* d, const OFString& sourceFileName);
	DcmDirectoryRecord* buildStoredPrintRecord(const OFString& referencedFileName, DcmItem* d, const OFString& sourceFileName);
	DcmDirectoryRecord* buildKeyObjectDocRecord(const OFString& referencedFileName, DcmItem* d, const OFString& sourceFileName);
	OFString locateDicomFile(const OFString& fname);
	OFBool recordMatchesDataset(DcmDirectoryRecord *rec, DcmItem* dataset);
	DcmDirectoryRecord* findExistingRecord(DcmDirectoryRecord *root, E_DirRecType dirtype, DcmItem* dataset);
	DcmDirectoryRecord* buildRecord(E_DirRecType dirtype, const OFString& referencedFileName, DcmItem* dataset, const OFString& sourceFileName);
#ifdef BUILD_DCMGPDIR_AS_DCMMKDIR
	void printIntegerAttribute(ostream& out, DcmTagKey& key, const long l);
#endif
	void printStringAttribute(ostream& out, DcmTagKey& key, const OFString& s);
	void printRecordUniqueKey(ostream& out, DcmDirectoryRecord *rec);
#ifdef BUILD_DCMGPDIR_AS_DCMMKDIR
	OFBool compareIntegerAttributes(DcmTagKey& tag, DcmDirectoryRecord *rec, DcmItem* dataset, const OFString& sourceFileName);
#endif
	OFBool compareStringAttributes(DcmTagKey& tag, DcmDirectoryRecord *rec, DcmItem* dataset, const OFString& sourceFileName);
	OFBool areTagsEqual(DcmObject* obj1, DcmObject* obj2);
	OFString constructTagName(DcmObject *obj);
	OFString intToOFString(int i);
	OFString constructTagNameWithSQ(DcmObject *obj, DcmSequenceOfItems* fromSequence, int itemNumber);
	OFBool compareBinaryValues(DcmElement* elem1, DcmElement* elem2, OFString& reason);
	OFBool compareSQAttributes(DcmSequenceOfItems* sq1, DcmSequenceOfItems* sq2, OFString& reason);
	OFBool compareAttributes(DcmElement* elem1, DcmElement* elem2, DcmSequenceOfItems* fromSequence, int itemNumber, OFString& reason);
	OFBool compareItems(DcmItem* item1, DcmItem* item2, DcmSequenceOfItems* fromSequence, int itemNumber, OFString& reason);
	DcmSequenceOfItems* dcmFindSequence(DcmItem* d, const DcmTagKey& key, OFBool searchIntoSub = OFFalse);
	OFBool compareSequenceAttributes(DcmTagKey& tag, DcmDirectoryRecord *rec, DcmItem* dataset, const OFString& sourceFileName);
	OFBool warnAboutInconsistantAttributes(DcmDirectoryRecord *rec, DcmItem* dataset, const OFString& sourceFileName, const OFBool abortCheck = OFFalse);
	int getISNumber(DcmItem *i, const DcmTagKey& key);
	OFCondition insertWithISCriterion(DcmDirectoryRecord *parent, DcmDirectoryRecord *child, const DcmTagKey& criterionKey);
	OFCondition insertSortedUnder(DcmDirectoryRecord *parent, DcmDirectoryRecord *child);
	DcmDirectoryRecord* includeRecord(DcmDirectoryRecord *parentRec, E_DirRecType dirtype, DcmItem* dataset, const OFString& referencedFileName, const OFString& sourceFileName);
	OFBool addToDir(DcmDirectoryRecord* rootRec, const OFString& ifname);
	OFBool areCSCharsValid(const OFString& fname);
	OFString locateInvalidFileNameChars(const OFString& fname, char separator = PATH_SEPARATOR);
	OFBool isaValidFileName(const OFString& fname, char sep = PATH_SEPARATOR);
	OFBool isaValidFileSetID(const OFString& aFsid);
	OFBool checkFileCanBeUsed(const OFString& fname);
	void inventMissingImageLevelAttributes(DcmDirectoryRecord *parent);
	void inventMissingSeriesLevelAttributes(DcmDirectoryRecord *parent);
	void inventMissingStudyLevelAttributes(DcmDirectoryRecord *parent);
	void inventMissingAttributes(DcmDirectoryRecord *root, const OFBool recurse = OFTrue);
	OFBool copyFile(const OFString& fromfname, const OFString& tofname);
	DcmDicomDir* createDicomdirFromFiles(OFList<OFString>& fileNames);
	OFBool expandFileNames(OFList<OFString>& fileNames, OFList<OFString>& expandedNames);
};

#endif // DicomdirFactory_hpp