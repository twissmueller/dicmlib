#ifdef TW_WITH_QT_UI

#include "DicomDirQtWidget.h"

#include "DicomDirQtListView.h"
#include "DicomDumpQtListView.h"
#include "DcmtkQtWidgetFactory.h"
#include "DcmtkConvert.h"
#include "DicomLibException.h"

#include <qlayout.h>
#include <qgroupbox.h>
#include <qwidgetstack.h>
#include <qstringlist.h>
#include <qlabel.h>

#include <iostream>
#include <vector>


////////////////////////////////////////////////////////////////////////////////

DicomDirQtWidget::DicomDirQtWidget(QWidget* parent, const char* name, WFlags fl)
	: QWidget(parent, name, fl)
{
	mainLayout = new QHBoxLayout(this);

	treeBox = new QGroupBox(this);
	treeBox -> setTitle("Dicom Directory");
	treeBox -> setColumnLayout(0, Qt::Vertical );
  treeBox -> layout() -> setSpacing( 6 );
  treeBox -> layout() -> setMargin( 11 );
  
	treeLayout = new QVBoxLayout(treeBox->layout());
	treeLayout -> setAlignment(Qt::AlignTop);

	stackBox = new QGroupBox(this);
	stackBox -> setTitle("Directory Record");
	stackBox -> setColumnLayout(0, Qt::Vertical );
  stackBox -> layout() -> setSpacing( 6 );
  stackBox -> layout() -> setMargin( 11 );
  
	stack = new QWidgetStack(stackBox);
	stack -> setSizePolicy(QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)7, 0, 0, stack->sizePolicy().hasHeightForWidth() ));

	stackLayout = new QVBoxLayout(stackBox->layout());
	stackLayout -> setAlignment(Qt::AlignTop);
	stackLayout -> addWidget(stack);

	mainLayout -> addWidget(treeBox);
	mainLayout -> addWidget(stackBox);

	dicomDir = NULL;
	listView = NULL;
	
}

////////////////////////////////////////////////////////////////////////////////

DicomDirQtWidget::~DicomDirQtWidget()
{}

////////////////////////////////////////////////////////////////////////////////

void DicomDirQtWidget::setData(DcmDicomDir* _dicomDir)
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

void DicomDirQtWidget::createView()
{
	try
	{
		if (dicomDir == NULL)
		{
			throw DicomLibException("No data!", __FILE__, __LINE__);
		}

		listView = DcmtkQtWidgetFactory::getDicomDirListView(dicomDir, treeBox);
		listView -> setSizePolicy(QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)7, 0, 0, listView->sizePolicy().hasHeightForWidth() ));
		treeLayout -> addWidget(listView);
		connect( listView, SIGNAL( clicked( QListViewItem* ) ), this, SLOT( itemSelected( QListViewItem* ) ) );
	}
	catch (...)
	{
		throw;
	}
}

////////////////////////////////////////////////////////////////////////////////

void DicomDirQtWidget::itemSelected(QListViewItem* _viewItem)
{
	try
	{
		DicomDumpQtListView* dumpView = NULL;
		if (_viewItem != NULL)
		{
			if (dumpView != NULL)
			{
				stack -> removeWidget(dumpView);
				delete dumpView;
			}

			DicomDirQtListViewItem* viewItem = reinterpret_cast<DicomDirQtListViewItem*>(_viewItem);
			dumpView = DcmtkQtWidgetFactory::getDumpWidget(reinterpret_cast<DcmObject*>(viewItem->getData()), this);
			dumpView -> setSizePolicy(QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)7, 0, 0, dumpView->sizePolicy().hasHeightForWidth() ));

			stack -> addWidget(dumpView, 1);
			stack -> raiseWidget(1);
		}

		this -> update();
	}
	catch (...)
	{
		throw;
	}
}

////////////////////////////////////////////////////////////////////////////////

QStringList DicomDirQtWidget::getSelected()
{
	try
	{
		QStringList fileNames;

		// traverse listView and get all isSelected() viewItems
		std::vector<DicomDirQtListViewItem*> items;
		items = listView->getSelectedItems();

		// extract file names from all viewItems and store them in fileNames
		int size = items.size();

		for (int i=0; i<size; ++i)
		{
			DicomDirQtListViewItem* current = items[i];
			
			fileNames << (DcmtkConvert::DcmImageRecordToFileName(current->getData())).c_str();
		}

		return fileNames;
	}
	catch (...)
	{
		throw;
	}
}

////////////////////////////////////////////////////////////////////////////////


#endif // TW_WITH_QT_UI