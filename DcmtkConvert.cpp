#include "DcmtkConvert.hpp"

#include "DcmtkHelper.hpp"
#include "DicomLibException.hpp"

#include <dcelem.h>
#include <dcdirrec.h>
#include <dcvrda.h>

#include <sstream>

DcmtkConvert::DcmtkConvert()
{}

DcmtkConvert::~DcmtkConvert()
{}

std::string DcmtkConvert::DcmElementValueToString(DcmElement* _element)
{
	try
	{
		if (_element == NULL)
		{
			throw DicomLibException("No data!", __FILE__, __LINE__);
		}

		DcmEVR devr; 
		devr = _element->ident();
		std::string value;
		std::string spacer = " -- ";

		unsigned int vm = _element->getVM();
		
		// see dcvr.h (dcmtk -> data)
		// see table 6.2-1, page 22, PS 3.5-2003
		switch (devr)
		{
			//case EVR_OF: dcmtk352
			case EVR_AE:
			case EVR_AT:
			case EVR_DT:
			case EVR_FL:
			case EVR_FD:
			case EVR_LT:
			case EVR_OW:
			case EVR_SL:
			case EVR_SQ:
			case EVR_SS:
			case EVR_UN:
			case EVR_UT: { value = "Not Implemented"; break; }
			
			case EVR_OB:
			case EVR_US:
			{
				Uint16 dvalue = 0;
				std::ostringstream tmp;
				for (unsigned int i=0; i<vm; ++i)
				{
					_element -> getUint16(dvalue, i);
					tmp << dvalue;
					if (i!=vm-1) tmp << spacer;
				}
				value = tmp.str();
				break;
			}
			
			case EVR_AS:
			case EVR_CS:
			case EVR_DA:
			case EVR_DS:
			case EVR_IS:
			case EVR_LO:
			case EVR_PN:	
			case EVR_SH:
			case EVR_ST:
			case EVR_TM:
			case EVR_UI:
			{
				OFString dvalue;
				std::ostringstream tmp;
				for (unsigned int i=0; i<vm; ++i)
				{
					_element -> getOFString(dvalue, i);
					tmp << dvalue.c_str();
					if (i!=vm-1) tmp << spacer;
				}
				value = tmp.str();
				break;			
			}
			case EVR_UL:
			{
				Uint32 dvalue = 0;
				std::ostringstream tmp;
				for (unsigned int i=0; i<vm; ++i)
				{
					_element -> getUint32(dvalue, i);
					tmp << dvalue;
					if (i!=vm-1) tmp << spacer;
				}
				value = tmp.str();
				break;
			}

			// dcmtk vr's
			case EVR_OverlayData:
			case EVR_UNKNOWN2B:
			case EVR_ox:
			case EVR_xs:
			case EVR_na:
			case EVR_up:				 { value = "Not Implemented"; break; }

			case EVR_item:       { value = "Item"; break;}
			case EVR_metainfo:   { value = "Meta Info"; break;}
			case EVR_dataset:		 { value = "Dataset"; break;}
			case EVR_fileFormat: { value = "File Format"; break;}
			case EVR_dicomDir:   { value = "DicomDir"; break;}
			case EVR_dirRecord:  { value = "DirRecord"; break;}
			case EVR_pixelSQ:		 { value = "Not Implemented"; break; }
			case EVR_pixelItem:  { value = "Not Implemented"; break; }
			case EVR_UNKNOWN:    { value = "Not Implemented"; break; }
			case EVR_PixelData:  { value = "Not Implemented"; break; }
			// you should NEVER get here!
			default:
			{
				throw DicomLibException("Unknown Value Representation!", __FILE__, __LINE__);
			}
		}
		return value;
	}
	catch (...)
	{
		throw;
	}
}

std::string DcmtkConvert::DcmDirectoryRecordToString(DcmDirectoryRecord* _dirRecord)
{
	try
	{
		if (_dirRecord == NULL)
		{
			throw DicomLibException("No data!", __FILE__, __LINE__);
		}

		int card = _dirRecord->cardSub();

		std::string value = "";
		std::ostringstream tmp;

		DcmElement* element = NULL;

		switch (_dirRecord->getRecordType())
		{
			case ERT_root:
			{
				tmp << "Root: ";
				tmp << "-";
				tmp << " (" << card << ")";
				value = tmp.str();
				break;
			}
			case ERT_Curve:
			{
				tmp << "Curve: ";
				tmp << "-";
				tmp << " (" << card << ")";
				value = tmp.str();
				break;
			}
			case ERT_FilmBox:
			{
				tmp << "Film Box: ";
				tmp << "-";
				tmp << " (" << card << ")";
				value = tmp.str();
				break;
			}
			case ERT_FilmSession:
			{
				tmp << "Film Session: ";
				tmp << "-";
				tmp << " (" << card << ")";
				value = tmp.str();
				break;
			}
			case ERT_Image:
			{
				tmp << "Image";
				element = DcmtkHelper::searchElement(dynamic_cast<DcmObject*>(_dirRecord), 0x0020, 0x0013);
				if (element != NULL)
				{
					tmp << " " << DcmtkConvert::padNum(DcmtkConvert::DcmElementValueToString(element));
				}
				element = NULL;
				element = DcmtkHelper::searchElement(dynamic_cast<DcmObject*>(_dirRecord), 0x0004, 0x1500);
				if (element != NULL)
				{
					tmp << ": " << DcmtkConvert::DcmElementValueToString(element);
				}
				value = tmp.str();
				break;
			}
			case ERT_ImageBox:
			{
				tmp << "Image Box: ";
				tmp << "-";
				tmp << " (" << card << ")";
				value = tmp.str();
				break;
			}
			case ERT_Interpretation:
			{
				tmp << "Interpretation: ";
				tmp << "-";
				tmp << " (" << card << ")";
				value = tmp.str();
				break;
			}
			case ERT_ModalityLut:
			{
				tmp << "Modality LUT: ";
				tmp << "-";
				tmp << " (" << card << ")";
				value = tmp.str();
				break;
			}
			case ERT_Mrdr:
			{
				tmp << "MRDR: ";
				tmp << "-";
				tmp << " (" << card << ")";
				value = tmp.str();
				break;
			}
			case ERT_Overlay:
			{
				tmp << "Overlay: ";
				tmp << "-";
				tmp << " (" << card << ")";
				value = tmp.str();
				break;
			}
			case ERT_Patient:
			{
				tmp << "Patient";
				element = DcmtkHelper::searchElement(dynamic_cast<DcmObject*>(_dirRecord), 0x0010, 0x0010);
				if (element != NULL)
				{	
					tmp << ": " << DcmtkConvert::DcmElementValueToString(element);
				}
				tmp << " (" << card << ")";
				value = tmp.str();
				break;
			}
			case ERT_PrintQueue:
			{
				tmp << "Print Queue: ";
				tmp << "-";
				tmp << " (" << card << ")";
				value = tmp.str();
				break;
			}
			case ERT_Private:
			{
				tmp << "Private: ";
				tmp << "-";
				tmp << " (" << card << ")";
				value = tmp.str();
				break;
			}
			case ERT_Results:
			{
				tmp << "Results: ";
				tmp << "-";
				tmp << " (" << card << ")";
				value = tmp.str();
				break;
			}
			case ERT_Series:
			{	
				tmp << "Series";
				element = DcmtkHelper::searchElement(dynamic_cast<DcmObject*>(_dirRecord), 0x0020, 0x0011);
				if (element != NULL)
				{
					tmp << " " << DcmtkConvert::padNum(DcmtkConvert::DcmElementValueToString(element));
				}
				element = NULL;
				element = DcmtkHelper::searchElement(dynamic_cast<DcmObject*>(_dirRecord), 0x0008, 0x103e);
				if (element != NULL)
				{
					tmp << ": " << DcmtkConvert::DcmElementValueToString(element);
				}
				tmp << " (" << card << ")";
				value = tmp.str();
				break;
			}
			case ERT_Study:
			{
				tmp << "Study";
				element = DcmtkHelper::searchElement(dynamic_cast<DcmObject*>(_dirRecord), 0x0020, 0x0010);
				if (element != NULL)
				{
					tmp << " " << DcmtkConvert::DcmElementValueToString(element);
					
				}
				element = NULL;
				element = DcmtkHelper::searchElement(dynamic_cast<DcmObject*>(_dirRecord), 0x0008, 0x1030);
				if (element != NULL)
				{
					tmp << ": " << DcmtkConvert::DcmElementValueToString(element);
				}
				tmp << " (" << card << ")";
				value = tmp.str();
				break;
			}
			case ERT_StudyComponent:
			{
				tmp << "Study Component: ";
				tmp << "-";
				tmp << " (" << card << ")";
				value = tmp.str();
				break;
			}
			case ERT_Topic:
			{
				tmp << "Topic: ";
				tmp << "-";
				tmp << " (" << card << ")";
				value = tmp.str();
				break;
			}
			case ERT_Visit:
			{
				tmp << "Visit: ";
				tmp << "-";
				tmp << " (" << card << ")";
				value = tmp.str();
				break;
			}
			case ERT_VoiLut:
			{
				tmp << "VOI LUT: ";
				tmp << "-";
				tmp << " (" << card << ")";
				value = tmp.str();
				break;
			}
			case ERT_StructReport:
			{
				tmp << "Struct Report: ";
				tmp << "-";
				tmp << " (" << card << ")";
				value = tmp.str();
				break;
			}
			case ERT_Presentation:
			{
				tmp << "Presentation: ";
				tmp << "-";
				tmp << " (" << card << ")";
				value = tmp.str();
				break;
			}
			case ERT_Waveform:
			{
				tmp << "Waveform: ";
				tmp << "-";
				tmp << " (" << card << ")";
				value = tmp.str();
				break;
			}
			case ERT_RTDose:
			{
				tmp << "RT Dose: ";
				tmp << "-";
				tmp << " (" << card << ")";
				value = tmp.str();
				break;
			}
			case ERT_RTStructureSet:
			{
				tmp << "RT Structure Set: ";
				tmp << "-";
				tmp << " (" << card << ")";
				value = tmp.str();
				break;
			}
			case ERT_RTPlan:
			{
				tmp << "RT Plan: ";
				tmp << "-";
				tmp << " (" << card << ")";
				value = tmp.str();
				break;
			}
			case ERT_RTTreatRecord:
			{
				tmp << "RT Treat Record: ";
				tmp << "-";
				tmp << " (" << card << ")";
				value = tmp.str();
				break;
			}
			case ERT_StoredPrint:
			{
				tmp << "Stored Print: ";
				tmp << "-";
				tmp << " (" << card << ")";
				value = tmp.str();
				break;
			}
			case ERT_KeyObjectDoc:
			{
				tmp << "Key Object Doc: ";
				tmp << "-";
				tmp << " (" << card << ")";
				value = tmp.str();
				break;
			}
			// you should never get here!
			default:
			{
				throw DicomLibException("Unknown Directory Record!", __FILE__, __LINE__);
			}
		}
		return value;
	}
	catch (...)
	{
		throw;
	}
}

std::string DcmtkConvert::DcmImageRecordToFileName(DcmDirectoryRecord* _dirRecord)
{
	try
	{
		if (_dirRecord == NULL)
		{
			throw DicomLibException("No data!", __FILE__, __LINE__);
		}

		if (_dirRecord->getRecordType() == ERT_Image)
		{
			std::string fileName;

			DcmElement* delem = NULL;
			delem = DcmtkHelper::searchElement(_dirRecord, 0x0004, 0x1500);

			if (delem != NULL)
			{
				unsigned int vm = delem->getVM();
				OFString dvalue;
				std::ostringstream tmp;
				for (unsigned int i=0; i<vm; ++i)
				{
					delem -> getOFString(dvalue, i);
					tmp << dvalue.c_str();
					if (i!=vm-1) tmp << "\\\\";
				}
				fileName = tmp.str();
			}
			return fileName;
		}
		else
		{
			throw DicomLibException("This is not an Image Record!", __FILE__, __LINE__);
		}
	}
	catch (...)
	{
		throw;
	}
}

std::string DcmtkConvert::padNum(const std::string& _num)
{
	if (_num.length() == 1)
	{
		std::ostringstream tmp;
		tmp << 0 << _num;
		return tmp.str();
	}
	else
	{
		return _num;
	}
}