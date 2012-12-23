//#ifdef TW_WITH_VTK

#include "DicomToVtk.hpp"
#include "DicomLibException.hpp"
#include "DcmtkBuilder.hpp"

//#include <ofcmdln.h>
//#include <dcistrmf.h>
//#include <dcstream.h>
//#include <dcdatset.h>
//#include <dcfilefo.h>
//#include <diutils.h>
//#include <dcmimage.h>

#include <vtkImageData.h>
#include <vtkImageFlip.h>

#include <dcmimage.h>

// DEBUG
#include <iostream>

vtkImageData* DicomToVtk::convert(const std::string& _filename)
{
	try
	{
		DcmtkBuilder builder;
		builder.setFilename(_filename);

		DicomImagePtr dImage = builder.getDicomImage();

		int height( dImage->getHeight() );
		int width(  dImage->getWidth()  );
		int alloc(  dImage->getDepth()  );

		std::cout << height << " " << width << " " << alloc << std::endl;

		int depth = 0;
		if (alloc % 8 == 0)
		{
			depth = alloc/8;
		}
		else
		{
			depth = (alloc/8) + 1;
		}

		int size = height * width * depth;

		unsigned char* data = new unsigned char[size];

		int retval = 0;
		retval = dImage -> getOutputData(data, size);

		if (retval == 0)
		{
			throw DicomLibException("Unable to copy data!", __FILE__, __LINE__);
		}
		
		vtkImageData* image = vtkImageData::New();
		
		switch (depth)
		{
			case 1:
			{
				image -> SetScalarTypeToUnsignedChar();
				break;
			}
			case 2:
			{
				image -> SetScalarTypeToUnsignedShort();
				break;
			}
			case 3:
			{
				image -> SetScalarType(VTK_UNSIGNED_INT);
				break;
			}
			case 4:
			{
				image -> SetScalarType(VTK_UNSIGNED_LONG);
				break;
			}
			case 8:
			{
				image -> SetScalarType(VTK_DOUBLE);
				break;
			}
			default:
			{
				throw DicomLibException("Unable to copy data!", __FILE__, __LINE__);
			}
		}

		image -> SetDimensions(width, height, 1);
		image -> SetOrigin(0, 0, 0);
		image -> SetNumberOfScalarComponents(1);
		image -> AllocateScalars();
		image -> Update();

		unsigned char* imagePointer = (unsigned char*)image -> GetScalarPointer();
		
		for (int k=0; k<size; ++k)
		{
			*imagePointer++ = *data++;
		}
		
		vtkImageFlip* flip = vtkImageFlip::New();
		flip -> SetInput(image);
		flip -> SetFilteredAxis(0);
		flip -> SetFilteredAxis(1);

		/*for (int h=height; h>0; --h)
		{
			for (int w=0; w<width; ++w)
			{
				for (int s=0; s<depth; ++s)
				{
					int count = h*width*depth + w*depth + s;
					*imagePointer++ = data[count];
				}
			}
		}*/

		return flip->GetOutput();
	}
	catch (...)
	{
		throw;
	}
}

//#endif // TW_WITH_VTK