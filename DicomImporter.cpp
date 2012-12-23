#include "DicomImporter.hpp"

#include "DicomLibException.hpp"
#include "DicomdirFactory.hpp"
#include "DcmtkHelper.hpp"
#include "DcmtkConvert.hpp"

//#include <iostream>

#include <dcfilefo.h>
#include <dcmetinf.h>
#include <dcdatset.h>
#include <dcdicdir.h>

DicomImporter::VectorOfStrings DicomImporter::files;
DicomImporter::VectorOfStrings DicomImporter::validFiles;
DicomImporter::VectorOfStrings DicomImporter::nonValidFiles;
DicomImporter::VectorOfStrings DicomImporter::dicomdirFiles;
DicomImporter::VectorOfStrings DicomImporter::nonDicomdirFiles;

DcmDicomDir* DicomImporter::Import(String& _path, VectorOfStrings& _files)
{
	try
	{
		files.clear();
		validFiles.clear();
		nonValidFiles.clear();
		dicomdirFiles.clear();
		nonDicomdirFiles.clear();

		//
		// check if there is at least one filename in the list
		//
		if (_files.empty()) throw DicomLibException("Empty list", __FILE__, __LINE__);
	
		//
		// _files must be a vector with absolute filenames, NO directory names allowed!
		//
		files = _files;

		//
		// check for valid dicom files
		//
		VectorOfStringsIterator iter = files.begin();
		while (iter != files.end())
		{
			if (DicomImporter::IsFileValid(*iter))
			{
				validFiles.push_back(*iter);
			}
			else
			{
				nonValidFiles.push_back(*iter);
			}
			++iter;
		}
		if (validFiles.empty())	throw DicomLibException("No valid DICOM files!", __FILE__, __LINE__);

		//
		// determine the SOP classes for the valid files
		//
		iter = validFiles.begin();
		while (iter != validFiles.end())
		{
			DicomImporter::DetermineSopClass(*iter);
			++iter;
		}

		//
		// let's see what is left and decide what to import
		//
		int dirs = dicomdirFiles.size();
		int others = nonDicomdirFiles.size();

		if (dirs == 0 && others == 0)
		{
			throw DicomLibException("No valid DICOM files!", __FILE__, __LINE__);
		}
		else if (dirs == 0 && others > 0)
		{
			// reconstruct DICOMDIR from files
			DicomdirFactory dirFactory;
			DcmDicomDir* dir = NULL;
			dir = dirFactory.Create(_path, nonDicomdirFiles);
			if (dir == NULL)
			{
				throw DicomLibException("Unable to reconstruct DICOM from files!", __FILE__, __LINE__);
			}
			else
			{
				return dir;
			}
		}
		else if (dirs == 1 && others == 0)
		{
			// open DICOMDIR directly
			return (new DcmDicomDir(dicomdirFiles[0].c_str()));
		}
		else if (dirs == 1 && others > 0)
		{
			throw DicomLibException("Don't mix DICOMDIR with other files!", __FILE__, __LINE__);
		}
		else if (dirs > 0 && others == 0)
		{
			throw DicomLibException("More than one DICOMDIR selected! Only open one at a time!", __FILE__, __LINE__);
		}
		else //(dirs > 0 && others > 0)
		{
			throw DicomLibException("More than one DICOMDIR and other DICOM files selected. Just open one DICOMDIR or other DICOM files at a time!", __FILE__, __LINE__);
		}
	}
	catch (...)
	{
		throw;
	}
}

bool DicomImporter::IsFileValid(const String& _file)
{
	try
	{
		//
		// create the dicom fileformat and check if this is possible
		//
		DcmFileFormat ff;
		OFCondition cond = ff.loadFile(_file.c_str());
		if (cond.bad()) return false;
	    
		//
		// ist it in part 10 format (does meta-header exist)?
		//
		DcmMetaInfo *m = ff.getMetaInfo();
		if (m == NULL || m->card() == 0) return false;

		//
		// does the file contain data (does the dataset exist)?
		//
		DcmDataset *d = ff.getDataset();
		if (d == NULL) return false;

		//
		// yes, it is a valid dicom file!
		//
		return true;
	}
	catch (...)
	{
		throw;
	}
}

void DicomImporter::DetermineSopClass(const String& _file)
{
	try
	{
		DcmFileFormat ff;
		ff.loadFile(_file.c_str());
		DcmMetaInfo *m = ff.getMetaInfo();
		
		DcmElement* elem = NULL;
		elem = DcmtkHelper::searchElement(m, 0002, 0002);

		std::string sopClass;
		if (elem != NULL)
		{
			sopClass = DcmtkConvert::DcmElementValueToString(elem);
		}
		else
		{
			return;
		}

		if (sopClass == "1.2.840.10008.1.3.10")
		{
			dicomdirFiles.push_back(_file);
		}
		else
		{
			nonDicomdirFiles.push_back(_file);
		}
	}
	catch (...)
	{
		throw;
	}
}

DicomImporter::DicomImporter()
{}

//#include <dcdicdir.h>
//DcmDicomDir* dcmgpdir_main(int argc, char* argv[]); // taken from dcmgpdir.cxx
//DcmDicomDir* DicomImporter::ImportFromDicomdir(const std::string& _filename)
//{
//	try
//	{
//		// assuming that filenames are correct and that files really exists
//		// do only check for dicom validity stuff
//
//		// checks go here
//
//		DcmDicomDir* dicomdir = new DcmDicomDir(_filename.c_str());
//
//		return dicomdir;
//	}
//	catch (...)
//	{
//		throw;
//	}
//}
//
//DcmDicomDir* DicomImporter::ImportFromFileList(const std::vector<std::string>& _filenames)
//{
//	try
//	{
//		//char* dirne = "C:\\Documents and Settings\\mrjones\\My Documents\\Projects\\DicmSuite\\1234\\*";
//		//char* myargv[2] = {app, dirne};
//
//		char* app  = "dcmgpdir";
//		
//		int files = _filenames.size();
//		int myargc = files+1;
//		char** myargv = &(new char[myargc]);
//
//		myargv[0] = app;
//		for (int i=0; i<files; ++i)
//		{
//			myargv[i+1] = (char*)(_filenames[i].c_str());
//		}
//		
//		return dcmgpdir_main(myargc, myargv);
//	}
//	catch (...)
//	{
//		throw;
//	}
//}
//
//DcmDicomDir* DicomImporter::ImportFromScannedDir(const std::string& _dirname)
//{
//	// assuming that filenames are correct and that files really exists
//	// do only check for dicom validity stuff
//
//	
//
//	return NULL;
//}
//
