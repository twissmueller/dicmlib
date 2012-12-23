#include "DcmtkHelper.hpp"

#include "DicomLibException.hpp"

#include "dctagkey.h"
#include "dcstack.h"
#include "dcobject.h"

DcmElement* DcmtkHelper::searchElement(DcmObject* _object, unsigned int _group, unsigned int _element)
{
	try
	{
		if (_object == NULL)
		{
			throw DicomLibException("No data!", __FILE__, __LINE__);
		}

		DcmTagKey searchKey;
		searchKey.set(_group, _element);

		DcmStack stack;

		_object -> search(searchKey, stack, ESM_fromHere, OFTrue);

		DcmElement* element = NULL;
		element = reinterpret_cast<DcmElement*>(stack.elem(0));

		return element;
	}
	catch (...)
	{
		throw;
	}
}

DcmtkHelper::DcmtkHelper()
{}

DcmtkHelper::~DcmtkHelper()
{}

/*
int twtools::DcmLibInterface::getInstNum(const char* filename)
{
  int tagval = -1;
  
	
	dataset = fileformat -> getDataset();  

  unsigned int group = 0x0020;
  unsigned int elem = 0x0013;
  DcmTagKey searchKey;
  searchKey.set(group, elem);
  
  DcmStack stack;
 
  dataset->search(searchKey, stack, ESM_fromHere, OFTrue);

  DcmElement* dobj = (DcmElement*)stack.elem(0);
  OFString myString;
  dobj -> getOFString(myString, 0);

  tagval = atoi(myString.c_str());

  return tagval;
}
*/