/****************************************************************************
** DicomDirQtWidget meta object code from reading C++ file 'DicomDirQtWidget.h'
**
** Created: Do 24. Jul 16:43:54 2003
**      by: The Qt MOC ($Id:  qt/moc_yacc.cpp   3.0.3   edited Mar 12 07:51 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "DicomDirQtWidget.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 19)
#error "This file was generated using the moc from 3.0.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *DicomDirQtWidget::className() const
{
    return "DicomDirQtWidget";
}

QMetaObject *DicomDirQtWidget::metaObj = 0;
static QMetaObjectCleanUp cleanUp_DicomDirQtWidget;

#ifndef QT_NO_TRANSLATION
QString DicomDirQtWidget::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "DicomDirQtWidget", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString DicomDirQtWidget::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "DicomDirQtWidget", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* DicomDirQtWidget::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QWidget::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ "_viewItem", &static_QUType_ptr, "QListViewItem", QUParameter::In }
    };
    static const QUMethod slot_0 = {"itemSelected", 1, param_slot_0 };
    static const QMetaData slot_tbl[] = {
	{ "itemSelected(QListViewItem*)", &slot_0, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"DicomDirQtWidget", parentObject,
	slot_tbl, 1,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_DicomDirQtWidget.setMetaObject( metaObj );
    return metaObj;
}

void* DicomDirQtWidget::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "DicomDirQtWidget" ) ) return (DicomDirQtWidget*)this;
    return QWidget::qt_cast( clname );
}

bool DicomDirQtWidget::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: itemSelected((QListViewItem*)static_QUType_ptr.get(_o+1)); break;
    default:
	return QWidget::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool DicomDirQtWidget::qt_emit( int _id, QUObject* _o )
{
    return QWidget::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool DicomDirQtWidget::qt_property( int _id, int _f, QVariant* _v)
{
    return QWidget::qt_property( _id, _f, _v);
}
#endif // QT_NO_PROPERTIES
