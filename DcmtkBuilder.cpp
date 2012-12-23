#include "DcmtkBuilder.hpp"

#include "DicomLibException.hpp"
#include <dcistrmf.h>
#include <dcfilefo.h>
#include <dcmetinf.h>
#include <dcdatset.h>
#include <dcdicdir.h>
#include <dcobject.h>
#include <ofcond.h>
#include <dcmimage.h>

DcmtkBuilder::DcmtkBuilder()
{
	filename = "not set";
}

void DcmtkBuilder::setFilename(const std::string& _filename)
{
	try
	{
		filename = _filename;
	}
	catch (...)
	{
		throw;
	}
}

void DcmtkBuilder::createFileFormat()
{
	try
	{
		if (filename == "not set")
		{
			throw DicomLibException("Filename not set!", __FILE__, __LINE__);
		}

		fileFormat = DcmFileFormatPtr(new DcmFileFormat);
		OFCondition cond = fileFormat -> loadFile(filename.c_str());

		if (cond.bad())
		{
			throw DicomLibException(cond.text(), __FILE__, __LINE__);
		}
	}
	catch (...)
	{
		throw;
	}
}

void DcmtkBuilder::createMetaInfo()
{
	try
	{
		//this -> createFileFormat();
		throw DicomLibException("Funtion not implemented!", __FILE__, __LINE__);
	}
	catch (...)
	{
		throw;
	}
}


void DcmtkBuilder::createDataset()
{
	try
	{
		//this -> createFileFormat();
		
		throw DicomLibException("Funtion not implemented!", __FILE__, __LINE__);
	}
	catch (...)
	{
		throw;
	}
}

void DcmtkBuilder::createDicomDir()
{
	try
	{
		if (filename == "not set")
		{
			throw DicomLibException("Filename not set!", __FILE__, __LINE__);
		}

		dicomDir = new DcmDicomDir(filename.c_str());
	}
	catch (...)
	{
		throw;
	}
}


void DcmtkBuilder::createDicomDirPtr()
{
	try
	{
		if (filename == "not set")
		{
			throw DicomLibException("Filename not set!", __FILE__, __LINE__);
		}

		dicomDirPtr = DcmDicomDirPtr(new DcmDicomDir(filename.c_str()));
	}
	catch (...)
	{
		throw;
	}
}

DcmFileFormatPtr DcmtkBuilder::getFileFormat()
{
	try
	{
		this -> createFileFormat();
		return fileFormat;
	}
	catch (...)
	{
		throw;
	}
}

DcmDicomDir* DcmtkBuilder::getDicomDir()
{
	try
	{
		this -> createDicomDir();
		return dicomDir;
	}
	catch (...)
	{
		throw;
	}
}

DcmDicomDirPtr DcmtkBuilder::getDicomDirPtr()
{
	try
	{
		this -> createDicomDirPtr();
		return dicomDirPtr;
	}
	catch (...)
	{
		throw;
	}
}


//void DcmtkBuilder::createDicomImage()
//{
//	try
//	{
//		if (filename == "not set")
//		{
//			throw DicomLibException("Filename not set!", __FILE__, __LINE__);
//		}
//
//		dicomImage = DicomImagePtr(new DicomImage(filename.c_str()));
//	}
//	catch (...)
//	{
//		throw;
//	}
//}
//
//DicomImagePtr DcmtkBuilder::getDicomImage()
//{
//	try
//	{
//		this -> createDicomImage();
//		return dicomImage;
//	}
//	catch (...)
//	{
//		throw;
//	}
//}