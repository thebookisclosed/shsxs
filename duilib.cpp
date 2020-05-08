#include "duilib.h"

HMODULE g_twinUI;
RP_CDUIContext* g_DUIContext;

HRESULT CDUIResourceManager::_RegisterElements()
{
	HRESULT result = 0;
	for (DWORD i = 0; i < this->_cRegister; i++)
	{
		result = ((VoidCall)this->_rgRegister[i])();
		if (result < 0)
			break;
	}
	return result;
}

HRESULT CDUIResourceManager::_DebugGetDUIPath(LPWSTR* pszDUIPath)
{
	HKEY regKey;
	HRESULT result = RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Advanced", 0, KEY_QUERY_VALUE, &regKey);
	if (result)
		return result;
	DWORD pathSize;
	RegQueryValueEx(regKey, _pszMarkupOverrideRegValue, NULL, NULL, NULL, &pathSize);
	if (!pathSize)
		return E_FAIL;
	*pszDUIPath = new WCHAR[pathSize];
	result = RegQueryValueEx(regKey, _pszMarkupOverrideRegValue, NULL, NULL, (LPBYTE)*pszDUIPath, &pathSize);
	if (result)
	{
		delete[] *pszDUIPath;
		*pszDUIPath = NULL;
	}
	RegCloseKey(regKey);
	return result;
}

HRESULT CDUIResourceManager::s_DebugGetDUIFileLastWrite(LPWSTR szDUIPath, FILETIME* writeTime)
{
	(*writeTime).dwLowDateTime = 0;
	(*writeTime).dwHighDateTime = 0;
	WIN32_FILE_ATTRIBUTE_DATA attribData;
	if (GetFileAttributesEx(szDUIPath, GetFileExInfoStandard, &attribData))
	{
		*writeTime = attribData.ftLastWriteTime;
		return ERROR_SUCCESS;
	}
	return HRESULT_FROM_WIN32(GetLastError());
}

HRESULT CDUIResourceManager::GetModuleResourceData(HINSTANCE hModule, LPCWSTR pwszName, LPCWSTR pwszType, LPCSTR* prgb, DWORD* pcb)
{
	HRSRC resource;
	DWORD resSize;
	HGLOBAL resGlobal;
	LPCSTR resLockStr;
	if ((resource = FindResource(hModule, pwszName, pwszType)) &&
		(resSize = SizeofResource(hModule, resource)) &&
		(resGlobal = LoadResource(hModule, resource)) &&
		(resLockStr = (LPCSTR)LockResource(resGlobal)))
	{
		*prgb = resLockStr;
		*pcb = resSize;
		return ERROR_SUCCESS;
	}
	return HRESULT_FROM_WIN32(GetLastError());
}

HRESULT CDUIResourceManager::s_DebugPrettyPrintXMLToFile(LPCSTR pszXML, IStream* stream, DWORD resSize)
{
	ULARGE_INTEGER zero = { 0 };
	HRESULT comResult = stream->SetSize(zero);
	if (!SUCCEEDED(comResult))
		return comResult;
	DWORD indentLevel = 0;
	LPCSTR xmlEnd = pszXML + resSize - 1;
	LPCSTR pszNextElement = pszXML;
	while (true)
	{
		LPSTR pszElementStart = StrPBrkA(pszNextElement, "<");
		if (!pszElementStart)
			return comResult;
		pszNextElement = 0;
		CHAR nextChar = *(pszElementStart + 1);
		if (nextChar == '/')
			--indentLevel;
		for (DWORD i = 0; i < indentLevel; i++)
		{
			comResult = stream->Write("   ", 3, NULL);
			if (!SUCCEEDED(comResult))
				return comResult;
		}
		if (nextChar != '/' && nextChar != '!')
			++indentLevel;
		LPSTR pszElementEnd = pszElementStart;
		while (true)
		{
			if (pszNextElement || !pszElementEnd)
			{
				comResult = stream->Write(pszElementStart, (ULONG)(pszNextElement - pszElementStart), NULL);
				if (!SUCCEEDED(comResult))
					return comResult;
				if (pszNextElement >= xmlEnd)
					break;
				comResult = stream->Write("\r\n", 2, NULL);
				if (!SUCCEEDED(comResult))
					return comResult;
				break;
			}
			// Find one of the following: " > \ /
			pszElementEnd = StrPBrkA(pszElementEnd, "\">\\/");
			CHAR chElementEnd = *pszElementEnd;
			if (chElementEnd == '\\')
				pszElementEnd += 2;
			else if (chElementEnd == '>')
				pszNextElement = pszElementEnd + 1;
			else if (chElementEnd == '/' && *(pszElementEnd + 1) == '>')
			{
				--indentLevel;
				pszNextElement = pszElementEnd + 2;
			}
			else 
			{
				if (chElementEnd == '\\')
					pszElementEnd += 2;
				++pszElementEnd;
			}			
		}
		if (pszNextElement >= xmlEnd)
			break;
	}
	if (!SUCCEEDED(comResult))
		return comResult;
	return stream->Commit(STGC_DEFAULT);
}

HRESULT CDUIResourceManager::_DebugSaveDUIResourceToFile(LPWSTR szDUIPath)
{
	LPCSTR pszXML;
	DWORD cbXMLSize;
	HRESULT result = GetModuleResourceData(_hInst, (LPCWSTR)_uResourceID, L"UIFILE", &pszXML, &cbXMLSize);
	if (!SUCCEEDED(result))
		return result;
	CComPtr<IStream> stream;
	result = SHCreateStreamOnFileW(szDUIPath, STGM_CREATE | STGM_SHARE_EXCLUSIVE | STGM_READWRITE, &stream);
	if (SUCCEEDED(result))
		result = s_DebugPrettyPrintXMLToFile(pszXML, stream, cbXMLSize);
	return result;
}

HRESULT CDUIResourceManager::_DebugEnsureDUIFile(LPWSTR* ppszPath, FILETIME* pftLastWrite)
{
	LPWSTR filePath;
	HRESULT result = _DebugGetDUIPath(&filePath);
	if (!SUCCEEDED(result))
		return result;
	FILETIME lastWrite;
	result = s_DebugGetDUIFileLastWrite(filePath, &lastWrite);
	if (result == HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND))
	{
		result = _DebugSaveDUIResourceToFile(filePath);
		if (!SUCCEEDED(result))
		{
			delete[] filePath;
			return result;
		}
		result = s_DebugGetDUIFileLastWrite(filePath, &lastWrite);
	}
	if (!SUCCEEDED(result))
	{
		delete[] filePath;
		return result;
	}
	*ppszPath = filePath;
	*pftLastWrite = lastWrite;
	return result;
}

HRESULT CDUIResourceManager::s_DebugLoadFileToMemory(LPCWSTR pszFile, LPWSTR* ppszOutBuf)
{
	CComPtr<IStream> stream;
	HRESULT result = SHCreateStreamOnFile(pszFile, STGM_READ, &stream);
	if (!SUCCEEDED(result))
		return result;
	STATSTG stats = { 0 };
	result = stream->Stat(&stats, STATFLAG_DEFAULT);
	if (!SUCCEEDED(result))
		return result;
	if (stats.cbSize.HighPart)
		return E_FAIL;
	DWORD ansiSize = stats.cbSize.LowPart + 1;
	if (ansiSize < stats.cbSize.LowPart)
		return HRESULT_FROM_WIN32(ERROR_ARITHMETIC_OVERFLOW);
	LPSTR ansiBuf = new CHAR[ansiSize];
	DWORD readBytes = 0;
	result = stream->Read(ansiBuf, stats.cbSize.LowPart, &readBytes);
	if (stats.cbSize.LowPart != readBytes)
	{
		delete[] ansiBuf;
		return E_FAIL;
	}
	ansiBuf[stats.cbSize.LowPart] = 0;
	DWORD uniSize = ansiSize * 2;
	if (uniSize < ansiSize)
		return HRESULT_FROM_WIN32(ERROR_ARITHMETIC_OVERFLOW);
	*ppszOutBuf = new WCHAR[uniSize];
	SHAnsiToUnicode(ansiBuf, *ppszOutBuf, ansiSize);
	delete[] ansiBuf;
	return result;
}

HMODULE CDUIResourceManager::GetShellStyleHInstance()
{
	HMODULE hInst = NULL;
	LPWSTR pszThemeFileName = new WCHAR[MAX_PATH];
	LPWSTR pszColorBuff = new WCHAR[100];
	if (!GetCurrentThemeName(pszThemeFileName, MAX_PATH, pszColorBuff, 100, NULL, 0))
	{
		PathAppendW(pszThemeFileName, L"Shell");
		PathAppendW(pszThemeFileName, pszColorBuff);
		PathAppendW(pszThemeFileName, L"ShellStyle.dll");
		hInst = LoadLibraryExW(pszThemeFileName, 0i64, 2u);
	}
	if (!hInst)
	{
		ExpandEnvironmentStringsW(L"%SystemRoot%\\System32\\ShellStyle.dll", pszThemeFileName, 0x104u);
		hInst = LoadLibraryExW(pszThemeFileName, 0i64, 2u);
	}
	delete[] pszThemeFileName;
	delete[] pszColorBuff;
	return hInst;
}

HRESULT CDUIResourceManager::_DebugLoadParser(LPCWSTR pszPath, LPVOID** parser)
{
	LPWSTR uiFile;
	HRESULT result = s_DebugLoadFileToMemory(pszPath, &uiFile);
	if (SUCCEEDED(result))
	{
		if (g_DUIContext)
			result = g_DUIContext->CreateParser(parser);
		else
			result = DirectUI::DUIXmlParser::Create((DirectUI::DUIXmlParser**)parser, NULL, NULL, NULL, NULL);
		if (SUCCEEDED(result))
		{
			HMODULE shellStyle = GetShellStyleHInstance();
			BOOL unloadLater = TRUE;
			if (!shellStyle)
			{
				shellStyle = _hInst;
				unloadLater = FALSE;
			}
			if (g_DUIContext)
				result = g_DUIContext->Parser_SetXML(*parser, uiFile, shellStyle);
			else
				result = ((DirectUI::DUIXmlParser*)*parser)->SetXML((unsigned const short*)uiFile, shellStyle, g_twinUI);
			if (!SUCCEEDED(result))
			{
				if (g_DUIContext)
					g_DUIContext->Parser_Destroy(*parser);
				else
					((DirectUI::DUIXmlParser*)*parser)->Destroy();
				*parser = 0;
			}
			if (unloadLater)
				FreeLibrary(shellStyle);
		}
		delete[] uiFile;
	}
	return result;
}

HRESULT CDUIResourceManager::DUI_LoadUIFileFromResources(HINSTANCE hModuleInstance, DWORD uFileID, LPWSTR* ppszUIFile)
{
	LPCSTR ansiBuf;
	DWORD ansiSize;
	HRESULT result = GetModuleResourceData(hModuleInstance, (LPCWSTR)uFileID, L"UIFILE", &ansiBuf, &ansiSize);
	if (!SUCCEEDED(result))
		return result;
	DWORD uniSize = (ansiSize + 1) * 2;
	if (uniSize < ansiSize)
		return HRESULT_FROM_WIN32(ERROR_ARITHMETIC_OVERFLOW);
	*ppszUIFile = (LPWSTR)LocalAlloc(LMEM_ZEROINIT, uniSize);
	if (!*ppszUIFile)
		return HRESULT_FROM_WIN32(GetLastError());
	SHAnsiToUnicode(ansiBuf, *ppszUIFile, uniSize);
	return result;
}

HRESULT CDUIResourceManager::DUI_CreateParserFromResource(HINSTANCE hinst, DWORD uID, LPVOID** ppParser)
{
	HMODULE shellStyle = GetShellStyleHInstance();
	BOOL unloadLater = TRUE;
	if (!shellStyle)
	{
		shellStyle = _hInst;
		unloadLater = FALSE;
	}
	LPWSTR uiFile;
	HRESULT result = DUI_LoadUIFileFromResources(hinst, uID, &uiFile);
	if (SUCCEEDED(result))
	{
		if (g_DUIContext)
			result = g_DUIContext->CreateParser(ppParser);
		else
			result = DirectUI::DUIXmlParser::Create((DirectUI::DUIXmlParser**)ppParser, NULL, NULL, NULL, NULL);
		if (SUCCEEDED(result))
		{
			if (g_DUIContext)
				result = g_DUIContext->Parser_SetXML(*ppParser, uiFile, shellStyle);
			else
				result = ((DirectUI::DUIXmlParser*)*ppParser)->SetXML((unsigned const short*)uiFile, shellStyle, g_twinUI);
		}
	}
	if (uiFile)
		LocalFree((HLOCAL)uiFile);
	if (unloadLater)
		FreeLibrary(shellStyle);
	if (!SUCCEEDED(result) && *ppParser)
	{
		if (g_DUIContext)
			g_DUIContext->Parser_Destroy(*ppParser);
		else
			((DirectUI::DUIXmlParser*)*ppParser)->Destroy();
		*ppParser = 0;
	}
	return result;
}

HRESULT CDUIResourceManager::_DebugCreateParser(REFCOUNTEDPARSER* prcp, LPVOID** ppParser)
{
	LPWSTR debugUiFilePath;
	FILETIME debugUiFileLastWrite;
	if (SUCCEEDED(_DebugEnsureDUIFile(&debugUiFilePath, &debugUiFileLastWrite)))
	{
		HRESULT result = _DebugLoadParser(debugUiFilePath, ppParser);
		if (SUCCEEDED(result))
		{
			if (prcp->spszDUIPath)
				delete[] prcp->spszDUIPath;
			prcp->spszDUIPath = debugUiFilePath;
			prcp->ftDUILastWrite = debugUiFileLastWrite;
			return result;
		}
	}
	return DUI_CreateParserFromResource(_hInst, _uResourceID, ppParser);
}

HRESULT CDUIResourceManager::_InitParserForThread()
{
	REFCOUNTEDPARSER* rcp = (REFCOUNTEDPARSER*)TlsGetValue(this->_tlsParser);
	if (rcp)
	{
		++_uInitCount;
		return ERROR_SUCCESS;
	}
	rcp = new REFCOUNTEDPARSER();
	LPVOID* pParser = 0;
	HRESULT result = _DebugCreateParser(rcp, &pParser);
	if (SUCCEEDED(result))
	{
		rcp->cRefCount = 1;
		rcp->pParser = pParser;
		result = TlsSetValue(_tlsParser, rcp) == 0 ? E_FAIL : 0;
		if (SUCCEEDED(result))
			return result;
		if (g_DUIContext)
			g_DUIContext->Parser_Destroy(pParser);
		else
			((DirectUI::DUIXmlParser*)pParser)->Destroy();
	}
	if (rcp)
	{
		if (rcp->spszDUIPath)
			delete[] rcp->spszDUIPath;
		delete rcp;
	}
	TlsSetValue(_tlsParser, NULL);
	return result;
}

void CDUIResourceManager::_UninitParserForThread()
{
	REFCOUNTEDPARSER* rcp = ((REFCOUNTEDPARSER*)TlsGetValue(_tlsParser));
	if (--rcp->cRefCount == 1)
	{
		if (g_DUIContext)
			g_DUIContext->Parser_Destroy(rcp->pParser);
		else
			((DirectUI::DUIXmlParser*)rcp->pParser)->Destroy();
		if (rcp->spszDUIPath)
			delete[] rcp->spszDUIPath;
		delete rcp;
		TlsSetValue(_tlsParser, NULL);
	}
}

void CDUIResourceManager::Initialize(HINSTANCE hInst, DWORD uResourceID, LPCVOID *rgRegister, DWORD cRegister, LPCWSTR pszMarkupOverrideRegValue, BOOL fDebug)
{
	_hInst = hInst;
	_uInitCount = 0;
	_tlsParser = -1;
	_uResourceID = uResourceID;
	_rgRegister = rgRegister;
	_cRegister = cRegister;
	_pszMarkupOverrideRegValue = pszMarkupOverrideRegValue;
	_fDebug = fDebug;
	InitializeSRWLock(&this->_srwlock);
}

HRESULT CDUIResourceManager::InitThread()
{
	ExclusiveSRW scopeLock(&this->_srwlock);
	HRESULT result = 0;
	if (!this->_uInitCount)
	{
		if (g_DUIContext)
			result = g_DUIContext->InitProcess();
		else
			result = InitProcessPriv(9, g_twinUI, 1, 1, FALSE);
		if (!SUCCEEDED(result))
			return result;
		result = _RegisterElements();
		if (!SUCCEEDED(result))
			return result;
		_tlsParser = TlsAlloc();
		if (_tlsParser == TLS_OUT_OF_INDEXES)
		{
			if (g_DUIContext)
				g_DUIContext->UnInitProcess();
			else
				UnInitProcessPriv(g_twinUI);
			return E_FAIL;
		}
	}
	if (g_DUIContext)
		result = g_DUIContext->InitThread();
	else
		result = ::InitThread(2);
	if (!SUCCEEDED(result))
		return result;
	result = _InitParserForThread();
	if (SUCCEEDED(result))
	{
		++_uInitCount;
		return result;
	}
	if (g_DUIContext)
		g_DUIContext->UnInitThread();
	else
		::UnInitThread();
	if (!_uInitCount)
	{
		TlsFree(_tlsParser);
		_tlsParser = -1;
		if (g_DUIContext)
			g_DUIContext->UnInitProcess();
		else
			UnInitProcessPriv(g_twinUI);
	}
	return result;
}

void CDUIResourceManager::UnInitThread()
{
	ExclusiveSRW scopeLock(&this->_srwlock);
	--_uInitCount;
	_UninitParserForThread();
	if (g_DUIContext)
		g_DUIContext->UnInitThread();
	else
		::UnInitThread();
	if (!_uInitCount)
	{
		TlsFree(_tlsParser);
		_tlsParser = -1;
		if (g_DUIContext)
			g_DUIContext->UnInitProcess();
		else
			UnInitProcessPriv(g_twinUI);
	}
}

LPVOID CDUIResourceManager::GetParserForThread()
{
	return ((REFCOUNTEDPARSER*)TlsGetValue(_tlsParser))->pParser;
}

HRESULT CDUIResourceManager::ReloadParser(LPVOID* ppParser)
{
	REFCOUNTEDPARSER* rcp = (REFCOUNTEDPARSER*)TlsGetValue(_tlsParser);
	if (!rcp)
		return E_FAIL;
	LPVOID* pParser;
	HRESULT result = _DebugCreateParser(rcp, &pParser);
	if (SUCCEEDED(result))
	{
		if (rcp->pParser)
		{
			if (g_DUIContext)
				g_DUIContext->Parser_Destroy(rcp->pParser);
			else
				((DirectUI::DUIXmlParser*)rcp->pParser)->Destroy();
		}
		rcp->pParser = pParser;
		ppParser = pParser;
	}
	return result;
}

BOOL CDUIResourceManager::DebugReloadParser()
{
	REFCOUNTEDPARSER* rcp = (REFCOUNTEDPARSER*)TlsGetValue(_tlsParser);
	if (!rcp)
		return FALSE;
	LPWSTR debugUiFilePath;
	FILETIME debugUiFileLastWrite;
	LPVOID* pParser;
	if (SUCCEEDED(_DebugEnsureDUIFile(&debugUiFilePath, &debugUiFileLastWrite)))
	{
		// Debug file is the same, or we failed loading using a newer one
		if (rcp->spszDUIPath && !StrCmpIW(rcp->spszDUIPath, debugUiFilePath) && !CompareFileTime(&rcp->ftDUILastWrite, &debugUiFileLastWrite) ||
			!SUCCEEDED(_DebugLoadParser(debugUiFilePath, &pParser)))
		{
			delete[] debugUiFilePath;
			return FALSE;
		}
		// Loaded using a newer debug file
		if (rcp->spszDUIPath)
			delete[] rcp->spszDUIPath;
		rcp->spszDUIPath = debugUiFilePath;
	}
	else
	{
		// Debug file wasn't used or regular DLL resource failed
		if (!rcp->spszDUIPath || !SUCCEEDED(DUI_CreateParserFromResource(_hInst, _uResourceID, &pParser)))
			return FALSE;
		// Loaded using regular DLL resource after a debug one was used
		delete[] rcp->spszDUIPath;
		debugUiFileLastWrite.dwLowDateTime = 0;
		debugUiFileLastWrite.dwHighDateTime = 0;
	}
	rcp->ftDUILastWrite = debugUiFileLastWrite;
	if (g_DUIContext)
		g_DUIContext->Parser_Destroy(rcp->pParser);
	else
		((DirectUI::DUIXmlParser*)rcp->pParser)->Destroy();
	rcp->pParser = pParser;
	return TRUE;
}