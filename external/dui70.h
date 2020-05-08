#ifndef DUI70_H
#define DUI70_H

extern "C" __declspec(dllimport) HRESULT __stdcall UnInitThread();
extern "C" __declspec(dllimport) HRESULT __stdcall InitThread(DWORD nThreadMode);
extern "C" __declspec(dllimport) HRESULT __stdcall UnInitProcessPriv(HINSTANCE hModule);
extern "C" __declspec(dllimport) HRESULT __stdcall InitProcessPriv(DWORD dwExpectedVersion, HINSTANCE a2, CHAR fRegisterControls, CHAR fEnableUIAutomationProvider, BOOL fInitCommctl);

class DirectUI {
public:
	class Value;

	class DUIXmlParser {
		public:
			__declspec(dllimport) static long Create(DUIXmlParser** ppParserOut, Value* pfnGetSheetCallback(unsigned short const *,void *), void* pGetSheetContext, void pfnErrorCallback(unsigned short const*, unsigned short const *, int, void*), void* pErrorContext);
			__declspec(dllimport) long SetXML(unsigned short const* pBuffer, HINSTANCE hResourceInstance, HINSTANCE hControlsInstance);
			__declspec(dllimport) void Destroy();
	};
};

#endif