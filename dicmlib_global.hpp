// uncomment this for QT GUI
// #define TW_WITH_QT_UI

// uncomment this for wxWindows GUI
#define TW_WITH_WX_UI

// uncomment this for VTK support
#define TW_WITH_VTK

// uncomment this for DLL compilation
//#define TW_AS_DLL

#ifdef TW_AS_DLL
	#ifdef _MSC_VER
		#define TW_DLL_EXPORT __declspec(dllexport)
	#endif // _MSC_VER
#else
	#define TW_DLL_EXPORT
#endif // TW_AS_DLL