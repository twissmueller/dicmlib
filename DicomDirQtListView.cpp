#ifdef TW_WITH_QT_UI

#include "DicomDirQtListView.h"

#include "DcmtkConvert.h"
#include "DicomLibException.h"
#include <dcdicdir.h>

////////////////////////////////////////////////////////////////////////////////

DicomDirQtListViewItem::DicomDirQtListViewItem(DcmDirectoryRecord* _dirRec, DicomDirQtListView* parent, const char* text)
	: QListViewItem(parent, text), dirRec(_dirRec)
{}

////////////////////////////////////////////////////////////////////////////////

DicomDirQtListViewItem::DicomDirQtListViewItem(DcmDirectoryRecord* _dirRec, DicomDirQtListViewItem* parent, const char* text)
	: QListViewItem(parent, text), dirRec(_dirRec)
{}

////////////////////////////////////////////////////////////////////////////////

DicomDirQtListViewItem::~DicomDirQtListViewItem()
{}

////////////////////////////////////////////////////////////////////////////////

DicomDirQtListView::DicomDirQtListView(QWidget* parent, const char* name, WFlags fl)
	: QListView(parent, name, fl)
{
	this -> addColumn("Directory Record");
	this -> setSorting(-1);
	this -> setRootIsDecorated(true);
	this -> setSelectionMode(Extended);

	dicomDir = NULL;
}

////////////////////////////////////////////////////////////////////////////////

DicomDirQtListView::~DicomDirQtListView()
{}

////////////////////////////////////////////////////////////////////////////////

void DicomDirQtListView::setData(DcmDicomDir* _dicomDir)
{
	try
	{
		if (_dicomDir == NULL)
		{
			throw DicomLibException("No data!", __FILE__, __LINE__);
		}

		dicomDir = _dicomDir;
	}
	catch (...)
	{
		throw;
	}
}

////////////////////////////////////////////////////////////////////////////////

void DicomDirQtListView::createView()
{
	try
	{
		if (dicomDir == NULL)
		{
			throw DicomLibException("No data!", __FILE__, __LINE__);
		}

		DcmDirectoryRecord& root = dicomDir->getRootRecord();
		DicomDirQtListViewItem* viewItem = new DicomDirQtListViewItem(&root, this,
																					 DcmtkConvert::DcmDirectoryRecordToString(&root).c_str());
		this -> traverseDirRecord(&root, viewItem);
	}
	catch (...)
	{
		throw;
	}
}

////////////////////////////////////////////////////////////////////////////////

void DicomDirQtListView::traverseDirRecord(DcmDirectoryRecord* _dirRecord, DicomDirQtListViewItem* _viewItem)
{
	try
	{
		size_t card = _dirRecord->cardSub();
		for (int i=0; i<card; ++i)
		{
			DcmDirectoryRecord* tmpDcmtk = _dirRecord->getSub(i);
			DicomDirQtListViewItem* viewItem = new DicomDirQtListViewItem(tmpDcmtk, _viewItem,
																						 DcmtkConvert::DcmDirectoryRecordToString(tmpDcmtk).c_str());
			this -> traverseDirRecord(tmpDcmtk, viewItem);
		}
	}
	catch (...)
	{
		throw;
	}
}

////////////////////////////////////////////////////////////////////////////////

std::vector<DicomDirQtListViewItem*> DicomDirQtListView::getSelectedItems()
{
	try
	{
		searchResult.clear();
		DicomDirQtListViewItem* parent = reinterpret_cast<DicomDirQtListViewItem*>(this -> firstChild());

		if (parent != NULL)
		{
			if (parent->isSelected())
			{
				searchResult.push_back(parent);
			}
			this -> searchRec(parent);
		}
		return searchResult;
	}
	catch (...)
	{ 
		throw;
	}
}

////////////////////////////////////////////////////////////////////////////////

void DicomDirQtListView::searchRec(DicomDirQtListViewItem* _item)
{
	try
	{
		if (_item != NULL)
		{
			int size = _item->childCount();

			DicomDirQtListViewItem* child = reinterpret_cast<DicomDirQtListViewItem*>(_item->firstChild());

			if (child != NULL)
			{
				if (child->isSelected())
				{
					searchResult.push_back(child);
				}
				this -> searchRec(child);
			}

			for (int i=0; i<size-1; ++i)
			{
				child = reinterpret_cast<DicomDirQtListViewItem*>(child->nextSibling());
		
				if (child != NULL)
				{
					if (child->isSelected())
					{
						searchResult.push_back(child);
					}
					this -> searchRec(child);
				}
			}
		}
	}
	catch (...)
	{
		throw;
	}
}

////////////////////////////////////////////////////////////////////////////////

#endif // TW_WITH_QT_UI