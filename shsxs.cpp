// shsxs.cpp : Defines the exported functions for the DLL application.
//

#include "common.h"
#include <ShlObj.h>
#include "duilib.h"
#include "rpenum.h"
#include "rpstructclass.h"
#include "rpcom.h"
#include "rpdatablob.h"

extern "C" HRESULT __stdcall RP_CFolderThumbnail_GetCoordinates(POSITION pos, DWORD *x, DWORD *y)
{
	switch (pos)
	{
	default:
		*x = 0;
		*y = 0;
		break;
	case POSITION_TOP_RIGHT:
		*x = 176;
		*y = 0;
		break;
	case POSITION_MIDDLE_RIGHT:
		*x = 176;
		*y = 88;
		break;
	case POSITION_BOTTOM_RIGHT:
		*x = 176;
		*y = 176;
		break;
	case POSITION_BOTTOM_CENTER:
		*x = 88;
		*y = 176;
		break;
	case POSITION_BOTTOM_LEFT:
		*x = 0;
		*y = 176;
		break;
	}
	return ERROR_SUCCESS;
}

extern "C" void __stdcall RP_CFolderThumbnail_GetSize(POSITION pos, SIZE *sizeOut, BYTE smallSize)
{
	DWORD result;
	if (pos)
		result = smallSize ? 32 : 80;
	else
		result = smallSize ? 64 : 168;
	(*sizeOut).cx = result;
	(*sizeOut).cy = result;
	//return result;
}

extern "C" void __stdcall RP_CFolderThumbnail_ReadSettingsFromRegistry()
{
}

extern "C" LPVOID __stdcall RP_CallScheduleGadgetTransitions(CScheduleGadgetTransitionsContext* context)
{
	return context->Invoke(context->GetTransitionContext());
}


extern "C" HRESULT __stdcall RP_CreateSettingsWindow(LPCWSTR lpTitle, HWND* hwnd, LPVOID prevPopup, IPopupWindow **popup)
{
	*popup = 0;
	if (!lpTitle)
		return E_INVALIDARG;
	CComPtr<ICharmsWindowFactory> icwf;
	HRESULT comResult = CoCreateInstance(CLSID_CharmsWindowFactory, NULL, 1, IID_PPV_ARGS(&icwf));
	if (SUCCEEDED(comResult))
	{
		CComPtr<ICharmWindow> icw;
		comResult = icwf->CreateCharmWindow(hwnd, CHARMS_SIZE_NARROW, &icw);
		if (SUCCEEDED(comResult))
		{
			IPopupWindow *ipw;
			comResult = icw->QueryInterface(IID_PPV_ARGS(&ipw));
			if (SUCCEEDED(comResult))
			{
				ipw->put_Title(lpTitle);
				icw->put_PreviousPopup(prevPopup);
				*popup = ipw;
			}
		}
	}
	return comResult;
}

extern "C" HRESULT __stdcall RP_CreateVANWindow(ICharmWindow **charm, IPopupWindow **popup, HWND* hwnd)
{
	*charm = 0;
	*popup = 0;
	*hwnd = 0;
	CComPtr<ICharmsWindowFactory> icwf;
	HRESULT comResult = CoCreateInstance(CLSID_CharmsWindowFactory, NULL, 1, IID_PPV_ARGS(&icwf));
	if (SUCCEEDED(comResult))
	{
		comResult = icwf->CreateCharmWindow(NULL, CHARMS_SIZE_NARROW, &*charm);
		if (SUCCEEDED(comResult))
		{
			CComPtr<IOleWindow> iow;
			comResult = (*charm)->QueryInterface(IID_PPV_ARGS(&iow));
			if (SUCCEEDED(comResult))
			{
				iow->GetWindow(hwnd);
				if (SUCCEEDED(comResult))
				{
					comResult = (*charm)->QueryInterface(IID_PPV_ARGS(&*popup));
				}
			}
		}
	}
	return comResult;
}

extern "C" HRESULT __stdcall RP_Enabled()
{
	return 1;
}

extern "C" HRESULT __stdcall RP_GetCharmBarItemArray(DWORD type, DWORD *&charmResources, LPVOID *charmCount)
{
	charmResources = (DWORD*)*&g_Charms;
	*charmCount = (LPVOID)5;
	return ERROR_SUCCESS;
}

extern "C" HRESULT __stdcall RP_GetCharmBarItemArray1(DWORD *&charmResources, LPVOID *charmCount)
{
	charmResources = (DWORD*)*&g_Charms1;
	*charmCount = (LPVOID)6;
	return ERROR_SUCCESS;
}

extern "C" HRESULT __stdcall RP_GetCharmBarItemArray2(DWORD type, DWORD *&charmResources, LPVOID *charmCount)
{
	if (type == 1)
		charmResources = (DWORD*)*&g_Charms2_Bottom;
	else
		charmResources = (DWORD*)*&g_Charms2_Right;
	*charmCount = (LPVOID)5;
	return ERROR_SUCCESS;
}

extern "C" HRESULT __stdcall RP_GetCharmBarItemArray3(DWORD type, DWORD *&charmResources, LPVOID *charmCount)
{
	if (type == 1)
		charmResources = (DWORD*)*&g_Charms3_Bottom;
	else
		charmResources = (DWORD*)*&g_Charms3_Right;
	*charmCount = (LPVOID)5;
	return ERROR_SUCCESS;
}

extern "C" LPCWSTR __stdcall RP_GetDDClassName()
{
	return L"Snapped Desktop";
}

extern "C" float __stdcall RP_GetKeyboardThumbKeyWidthRatio(int a1, int a2, float a3)
{
	float result = (float)a1 / ((float)a2 * a3);
	if (result > 900.0)
		return (float)900.0 / result;
	else
		return 1.0;
}

extern "C" float __stdcall RP_GetKeyboardThumbKeyWidthRatio2(int a1, float a2, int a3, float a4)
{
	int result = (int)(a1 / (a2 * a4));
	if (result > a3)
		return (float)a3 / (float)result;
	else
		return 1.0;
}

extern "C" int __stdcall RP_GetKeyboardThumbRowHeight(int a1, float a2)
{
	if ((float)a1 / a2 > 1200.0)
		return (int)(a2 * 1200.0);
	else
		return a1;
}

extern "C" int __stdcall RP_GetKeyboardThumbRowHeight2(int a1, int a2, float a3, BYTE a4)
{
	float result = a2 * a3;
	if ((int)result < a1)
		a1 = (int)result;
	if (a4)
		a1 = (int)(a1 * 1.13824);
	return a1;
}

EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)
extern "C" HRESULT __stdcall RP_InitCam()
{
	HRESULT duiCallRes = ERROR_SUCCESS;
	if (InitProcessPriv(13, HINST_THISCOMPONENT, 1, 1, 0) >= 0 || InitProcessPriv(14, HINST_THISCOMPONENT, 1, 1, 0) >= 0)
	{
		duiCallRes = InitThread(0x10002);
		if (duiCallRes < 0)
		{
			UnInitProcessPriv(HINST_THISCOMPONENT);
		}
	}
	return duiCallRes;
}

extern "C" HRESULT __stdcall RP_InitLauncherDataLayer(ICondition *condition, LPCWSTR lpName, IShellItem **shellItem)
{
	*shellItem = 0;
	CComPtr<ISearchFolderItemFactoryPrivEx> sfifEx;
	HRESULT comResult = CoCreateInstance(CLSID_SearchFolderItemFactory, NULL, 3, IID_PPV_ARGS(&sfifEx));
	if (SUCCEEDED(comResult))
	{
		CComPtr<IScopeFactory> sf;
		comResult = CoCreateInstance(CLSID_ScopeFactory, NULL, 3, IID_PPV_ARGS(&sf));
		if (SUCCEEDED(comResult))
		{
			CComPtr<IScope> scope;
			comResult = sf->CreateScope(IID_PPV_ARGS(&scope));
			if (SUCCEEDED(comResult))
			{
				CComPtr<IScopeItem> appsScopeItem;
				comResult = sf->CreateScopeItemFromKnownFolder(FOLDERID_AppsFolder, SI_TYPE_INCLUDE, SI_DEPTH_SHALLOW, SI_FLAG_FORCEEXHAUSTIVE, IID_PPV_ARGS(&appsScopeItem));
				if (SUCCEEDED(comResult))
					comResult = scope->AddScopeItem(appsScopeItem);
				if (SUCCEEDED(comResult))
				{
					CComPtr<IShellItem> startMenuLauncherProvider;
					if (SHCreateItemFromParsingName(L"shell:::{98F275B4-4FFF-11E0-89E2-7B86DFD72085}", NULL, IID_PPV_ARGS(&startMenuLauncherProvider)))
					{
						SFGAOF attribs;
						if (startMenuLauncherProvider->GetAttributes(SFGAO_FOLDER, &attribs) && attribs)
						{
							CComPtr<IScopeItem> launchProvScopeItem;
							comResult = sf->CreateScopeItemFromShellItem(startMenuLauncherProvider, SI_TYPE_INCLUDE, SI_DEPTH_SHALLOW, SI_FLAG_FORCEEXHAUSTIVE, IID_PPV_ARGS(&launchProvScopeItem));
							if (SUCCEEDED(comResult))
								scope->AddScopeItem(launchProvScopeItem);
						}
					}
					sfifEx->SetScopeDirect(scope);
				}
			}
		}
		if (SUCCEEDED(comResult))
		{
			CComPtr<ISearchFolderItemFactory> sfif;
			comResult = sfifEx->QueryInterface(IID_PPV_ARGS(&sfif));
			if (SUCCEEDED(comResult))
			{
				if (condition)
					comResult = sfif->SetCondition(condition);
				if (SUCCEEDED(comResult))
				{
					comResult = sfif->SetDisplayName(lpName);
					if (SUCCEEDED(comResult))
						comResult = sfif->GetShellItem(IID_PPV_ARGS(&*shellItem));
				}
			}
		}
	}
	else if (comResult == REGDB_E_CLASSNOTREG || comResult == E_NOINTERFACE)
	{
		CComPtr<ISearchFolderItemFactory> sfif;
		comResult = CoCreateInstance(CLSID_SearchFolderItemFactory, NULL, 3, IID_PPV_ARGS(&sfif));
		if (SUCCEEDED(comResult))
		{
			CComPtr<IShellItem> isi;
			comResult = SHGetKnownFolderItem(FOLDERID_AppsFolder, KF_FLAG_DONT_VERIFY, NULL, IID_PPV_ARGS(&isi));
			if (SUCCEEDED(comResult))
			{
				CComPtr<IShellItemArray> isia;
				comResult = SHCreateShellItemArrayFromShellItem(isi, IID_PPV_ARGS(&isia));
				if (SUCCEEDED(comResult))
				{
					CComPtr<ISearchFolderItemFactoryPriv> isfifp;
					comResult = sfif->QueryInterface(IID_PPV_ARGS(&isfifp));
					if (SUCCEEDED(comResult))
					{
						comResult = isfifp->SetScopeWithDepth(isia, SI_DEPTH_SHALLOW);
						if (SUCCEEDED(comResult))
						{
							if (condition)
								comResult = sfif->SetCondition(condition);
							if (SUCCEEDED(comResult))
							{
								comResult = sfif->SetDisplayName(lpName);
								if (SUCCEEDED(comResult))
									comResult = sfif->GetShellItem(IID_PPV_ARGS(&*shellItem));
							}
						}
					}
				}
			}
		}
	}
	return comResult;
}

extern "C" HRESULT __stdcall RP_InitLauncherDataLayer2(IShellItem **shellItem)
{
	*shellItem = 0;
	CComPtr<ISearchFolderItemFactoryPrivEx> sfifEx;
	HRESULT comResult = CoCreateInstance(CLSID_SearchFolderItemFactory, NULL, 3, IID_PPV_ARGS(&sfifEx));
	if (SUCCEEDED(comResult))
	{
		CComPtr<IScopeFactory> sf;
		comResult = CoCreateInstance(CLSID_ScopeFactory, NULL, 3, IID_PPV_ARGS(&sf));
		if (SUCCEEDED(comResult))
		{
			CComPtr<IScope> scope;
			comResult = sf->CreateScope(IID_PPV_ARGS(&scope));
			if (SUCCEEDED(comResult))
			{
				CComPtr<IScopeItem> appsScopeItem;
				comResult = sf->CreateScopeItemFromKnownFolder(FOLDERID_AppsFolder, SI_TYPE_INCLUDE, SI_DEPTH_SHALLOW, SI_FLAG_FORCEEXHAUSTIVE, IID_PPV_ARGS(&appsScopeItem));
				if (SUCCEEDED(comResult))
					comResult = scope->AddScopeItem(appsScopeItem);
				if (SUCCEEDED(comResult))
				{
					CComPtr<IShellItem> startMenuLauncherProvider;
					if (SHCreateItemFromParsingName(L"shell:::{98F275B4-4FFF-11E0-89E2-7B86DFD72085}", NULL, IID_PPV_ARGS(&startMenuLauncherProvider)))
					{
						SFGAOF attribs;
						if (startMenuLauncherProvider->GetAttributes(SFGAO_FOLDER, &attribs) && attribs)
						{
							CComPtr<IScopeItem> launchProvScopeItem;
							comResult = sf->CreateScopeItemFromShellItem(startMenuLauncherProvider, SI_TYPE_INCLUDE, SI_DEPTH_SHALLOW, SI_FLAG_FORCEEXHAUSTIVE, IID_PPV_ARGS(&launchProvScopeItem));
							if (SUCCEEDED(comResult))
								scope->AddScopeItem(launchProvScopeItem);
						}
					}
					sfifEx->SetScopeDirect(scope);
				}
			}
		}
		if (SUCCEEDED(comResult))
		{
			CComPtr<ISearchFolderItemFactory> sfif;
			comResult = sfifEx->QueryInterface(IID_PPV_ARGS(&sfif));
			if (SUCCEEDED(comResult))
				comResult = sfif->GetShellItem(IID_PPV_ARGS(&*shellItem));
		}
	}
	return comResult;
}

extern "C" HRESULT __stdcall RP_InitLauncherDataLaye0(IShellItem **shellItem)
{
	*shellItem = 0;
	CComPtr<ISearchFolderItemFactory> sfif;
	HRESULT comResult = CoCreateInstance(CLSID_SearchFolderItemFactory, NULL, 3, IID_PPV_ARGS(&sfif));
	if (SUCCEEDED(comResult))
	{
		CComPtr<IShellItem> isi;
		comResult = SHGetKnownFolderItem(FOLDERID_AppsFolder, KF_FLAG_DONT_VERIFY, NULL, IID_PPV_ARGS(&isi));
		if (SUCCEEDED(comResult))
		{
			CComPtr<IShellItemArray> isia;
			comResult = SHCreateShellItemArrayFromShellItem(isi, IID_PPV_ARGS(&isia));
			if (SUCCEEDED(comResult))
			{
				CComPtr<ISearchFolderItemFactoryPriv> isfifp;
				comResult = sfif->QueryInterface(IID_PPV_ARGS(&isfifp));
				if (SUCCEEDED(comResult))
				{
					comResult = isfifp->SetScopeWithDepth(isia, SI_DEPTH_SHALLOW);
					if (SUCCEEDED(comResult))
						comResult = sfif->GetShellItem(IID_PPV_ARGS(&*shellItem));
				}
			}
		}
	}
	return comResult;
}

extern "C" HRESULT __stdcall RP_InitSystemSetting()
{
	HMODULE dui70 = GetModuleHandle(L"dui70.dll");
	if (!dui70)
		return HRESULT_FROM_WIN32(GetLastError());
	VoidCall RegisterPVLBehaviorFactory = (VoidCall)GetProcAddress(dui70, "RegisterPVLBehaviorFactory");
	if (!RegisterPVLBehaviorFactory)
		return HRESULT_FROM_WIN32(GetLastError());
	HRESULT duiCallRes = ERROR_SUCCESS;
	if (InitProcessPriv(13, HINST_THISCOMPONENT, 1, 1, 0) >= 0 || InitProcessPriv(14, HINST_THISCOMPONENT, 1, 1, 0) >= 0)
	{
		duiCallRes = InitThread(0x10002);
		if (duiCallRes < 0)
		{
			UnInitProcessPriv(HINST_THISCOMPONENT);
			return duiCallRes;
		}
		duiCallRes = RegisterPVLBehaviorFactory();
		if (duiCallRes < 0)
		{
			UnInitThread();
			UnInitProcessPriv(HINST_THISCOMPONENT);
		}
	}
	return duiCallRes;
}

extern "C" int __stdcall RP_InitializeFeedbackRegion(HDC hDC, const RECT* lprc, COLORREF cRef)
{
	BITMAPINFO bmi;
	memset(&bmi, 0, sizeof(BITMAPINFO));
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth = 1;
	bmi.bmiHeader.biHeight = 1;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 32;

	BYTE alpha = (cRef >> 24) & 0xFF;
	RGBQUAD color;
	color.rgbRed = alpha * GetRValue(cRef) / 0xFF;
	color.rgbGreen = alpha * GetGValue(cRef) / 0xFF;
	color.rgbBlue = alpha * GetBValue(cRef) / 0xFF;
	color.rgbReserved = alpha;
	return StretchDIBits(hDC, lprc->left, lprc->top, lprc->right - lprc->left, lprc->bottom - lprc->top, 0, 0, 1, 1, &color, &bmi, 0, SRCCOPY);
}

extern "C" UINT __stdcall RP_PrivateExtractIconsW(LPCWSTR szFileName, int nIconIndex, int cxIcon, int cyIcon, HICON *phicon, UINT *piconid, UINT nIcons, int flags)
{
	if (nIconIndex != -162 && (nIconIndex <= -7 || nIconIndex > -3))
		return PrivateExtractIconsW(szFileName, nIconIndex, cxIcon, cyIcon, phicon, piconid, nIcons, flags);
	if (StrCmpIW(PathFindFileNameW(szFileName), L"imageres.dll"))
		return PrivateExtractIconsW(szFileName, nIconIndex, cxIcon, cyIcon, phicon, piconid, nIcons, flags);
	HMODULE phModule;
	if (!GetModuleHandleExW(6, (LPCWSTR)RP_PrivateExtractIconsW, &phModule))
		return PrivateExtractIconsW(szFileName, nIconIndex, cxIcon, cyIcon, phicon, piconid, nIcons, flags);
	WCHAR Filename[MAX_PATH];
	if (GetModuleFileNameW(phModule, Filename, MAX_PATH))
	{
		UINT localExtractResult = PrivateExtractIconsW(Filename, nIconIndex, cxIcon, cyIcon, phicon, piconid, nIcons, flags);
		if (localExtractResult > 0)
			return localExtractResult;
	}
	return PrivateExtractIconsW(szFileName, nIconIndex, cxIcon, cyIcon, phicon, piconid, nIcons, flags);
}

extern "C" HRESULT __stdcall RP_SetTimestampForApp(IImmersiveApplication *app, FILETIME *time)
{
	CComPtr<IPropertyStore> ps;
	HRESULT comResult = app->OpenPropertyStore(IAGPS_DEFAULT, IID_PPV_ARGS(&ps));
	if ((comResult & 0x80000000) == 0)
	{
		PROPVARIANT pv = {};
		pv.vt = VT_FILETIME;
		pv.filetime = *time;
		ps->SetValue(PKEY_ImmersiveApplication_VisibleTime, pv);
	}
	return comResult;
}

extern "C" DWORD __stdcall RP_VersionCheck()
{
	DWORD version = 26;
	HKEY optKey;
	if (!RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer", 0, KEY_QUERY_VALUE, &optKey))
	{
		DWORD dwData;
		DWORD cbData = sizeof(DWORD);
		if (!RegQueryValueEx(optKey, L"RPVersion", NULL, NULL, (LPBYTE)&dwData, &cbData))
			version = dwData;
		RegCloseKey(optKey);
	}
	return version;
}

extern "C" void __stdcall RP_PickerSetDUIContext(RP_CDUIContext *pDUIContext)
{
	g_DUIContext = pDUIContext;
}

CDUIResourceManager* PickerDUIResourceManager;

extern "C" HRESULT __stdcall RP_InitializePickerDUIResourceManager(HINSTANCE hInstance, DWORD uResourceID, LPCVOID* rgRegister, DWORD cRegister, LPCWSTR pszMarkupOverrideRegValue, BOOL fDebug)
{
	if (!g_DUIContext)
		g_twinUI = GetModuleHandleW(L"twinui.dll");
	static CDUIResourceManager pickerMgr;
	pickerMgr.Initialize(hInstance, uResourceID, rgRegister, cRegister, pszMarkupOverrideRegValue, fDebug);
	PickerDUIResourceManager = &pickerMgr;
	return ERROR_SUCCESS;
}

extern "C" HRESULT __stdcall RP_GetPickerDUIResourceManager(CDUIResourceManager *&resMgr)
{
	resMgr = PickerDUIResourceManager;
	return ERROR_SUCCESS;
}

extern "C" DWORD __stdcall RP_GetLayoutManagerBandDependencies(BAND_DEPENDENCY *&pprgBandDeps, int *pcDeps)
{
	HLOCAL mem = LocalAlloc(0, 10*sizeof(BAND_DEPENDENCY));
	memcpy(mem, (DWORD*)*&g_BandDeps, 10*sizeof(BAND_DEPENDENCY));
	pprgBandDeps = (BAND_DEPENDENCY*)*&mem;
	*pcDeps = 10;
	return ERROR_SUCCESS;
}
