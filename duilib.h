#ifndef DUILIB_H
#define DUILIB_H

#include "common.h"
#include <Uxtheme.h>

struct ExclusiveSRW
{
   ExclusiveSRW(SRWLOCK* lock) : lock(lock)
   {
       AcquireSRWLockExclusive(lock);
   }

    ~ExclusiveSRW()
    {
        ReleaseSRWLockExclusive(lock);
    }

private:
    SRWLOCK* lock;
};

class RP_CDUIContext
{
public:
	virtual HRESULT InitProcess() = 0;
	virtual HRESULT UnInitProcess() = 0;
	virtual HRESULT InitThread() = 0;
	virtual HRESULT UnInitThread() = 0;
	virtual HRESULT CreateParser(LPVOID** ppParser) = 0;
	virtual HRESULT Parser_SetXML(LPVOID* pParser, LPCWSTR xml, HINSTANCE inst) = 0;
	virtual HRESULT Parser_Destroy(LPVOID* pParser) = 0;
};

extern HMODULE g_twinUI;
extern RP_CDUIContext* g_DUIContext;

class CDUIResourceManager
{
	RTL_SRWLOCK _srwlock;
	HINSTANCE _hInst;
	DWORD _uInitCount;
	DWORD _tlsParser;
	DWORD _uResourceID;
	LPCVOID* _rgRegister;
	DWORD _cRegister;
	LPCWSTR _pszMarkupOverrideRegValue;
	BOOL _fDebug;

	struct REFCOUNTEDPARSER
	{
		LPVOID* pParser;
		DWORD cRefCount;
		LPWSTR spszDUIPath;
		FILETIME ftDUILastWrite;
	};

	HRESULT _RegisterElements();
	HRESULT _DebugGetDUIPath(LPWSTR* pszDUIPath);
	HRESULT s_DebugGetDUIFileLastWrite(LPWSTR szDUIPath, FILETIME* writeTime);
	HRESULT GetModuleResourceData(HINSTANCE hModule, LPCWSTR pwszName, LPCWSTR pwszType, LPCSTR* prgb, DWORD* pcb);
	HRESULT s_DebugPrettyPrintXMLToFile(LPCSTR pszXML, IStream* stream, DWORD resSize);
	HRESULT _DebugSaveDUIResourceToFile(LPWSTR szDUIPath);
	HRESULT _DebugEnsureDUIFile(LPWSTR* ppszPath, FILETIME* pftLastWrite);
	HRESULT s_DebugLoadFileToMemory(LPCWSTR pszFile, LPWSTR* ppszOutBuf);
	HMODULE GetShellStyleHInstance();
	HRESULT _DebugLoadParser(LPCWSTR pszPath, LPVOID** parser);
	HRESULT DUI_LoadUIFileFromResources(HINSTANCE hModuleInstance, DWORD uFileID, LPWSTR* ppszUIFile);
	HRESULT DUI_CreateParserFromResource(HINSTANCE hinst, DWORD uID, LPVOID** ppParser);
	HRESULT _DebugCreateParser(REFCOUNTEDPARSER* prcp, LPVOID** ppParser);
	HRESULT _InitParserForThread();
	void _UninitParserForThread();
public:
	virtual void Initialize(HINSTANCE hInst, DWORD uResourceID, LPCVOID *rgRegister, DWORD cRegister, LPCWSTR pszMarkupOverrideRegValue, BOOL fDebug);
	virtual HRESULT InitThread();
	virtual void UnInitThread();
	virtual LPVOID GetParserForThread();
	virtual HRESULT ReloadParser(LPVOID* ppParser);
	virtual BOOL DebugReloadParser();
};
#endif