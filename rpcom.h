MIDL_INTERFACE("137BD0F9-6676-41DE-ACC7-D37DD20272D3")
ICharmWindow : public IUnknown
{
public:
	virtual HRESULT STDMETHODCALLTYPE put_PreviousPopup(LPVOID popup) = 0;
};

MIDL_INTERFACE("C7A0FC17-C704-491B-AC4A-DF596D71BD5A")
IPopupWindow : public IUnknown
{
public:
	virtual HRESULT STDMETHODCALLTYPE get_Title(LPWSTR title) = 0;
	virtual HRESULT STDMETHODCALLTYPE put_Title(LPCWSTR title) = 0;
};

MIDL_INTERFACE("9812962F-B8B4-4B38-BB03-21F07975361A")
ICharmsWindowFactory : public IUnknown
{
public:
	virtual HRESULT STDMETHODCALLTYPE CreateCharmWindow(HWND* hwnd, CHARMS_SIZE charmSize, ICharmWindow **charm) = 0;
};

//c153e702-afcc-4f45-9a75-a70217f72db8
static const CLSID CLSID_CharmsWindowFactory = { 0xc153e702, 0xafcc, 0x4f45, { 0x9a, 0x75, 0xa7, 0x02, 0x17, 0xf7, 0x2d, 0xb8 } };

MIDL_INTERFACE("54410B83-6787-4418-9735-5AAAABE83A9A")
IScopeFactory : public IUnknown
{
public:
	virtual HRESULT STDMETHODCALLTYPE CreateScope(const IID &riid, LPVOID *ppv) = 0;
	virtual HRESULT STDMETHODCALLTYPE CreateScopeFromShellItemArray() = 0;
	virtual HRESULT STDMETHODCALLTYPE CreateScopeFromIDLists() = 0;
	virtual HRESULT STDMETHODCALLTYPE CreateScopeItemFromIDList() = 0;
	virtual HRESULT STDMETHODCALLTYPE CreateScopeItemFromKnownFolder(const IID &id1, SCOPE_ITEM_TYPE type, SCOPE_ITEM_DEPTH depth, SCOPE_ITEM_FLAGS flags, const IID &id2, LPVOID *ppv) = 0;
	virtual HRESULT STDMETHODCALLTYPE CreateScopeItemFromShellItem(IShellItem *si, SCOPE_ITEM_TYPE type, SCOPE_ITEM_DEPTH depth, SCOPE_ITEM_FLAGS flags, const IID &id2, LPVOID *ppv) = 0;
};
//6746C347-576B-4F73-9012-CDFEEA251BC4
static const CLSID CLSID_ScopeFactory = { 0x6746C347, 0x576B, 0x4F73, { 0x90, 0x12, 0xCD, 0xFE, 0xEA, 0x25, 0x1B, 0xC4 } };

MIDL_INTERFACE("DD400FF4-A119-405F-970E-A9A5E7E828C0")
IScopeItem : public IUnknown
{
};

MIDL_INTERFACE("655D1685-2BFD-4F7F-AD22-5AB61C8D8798")
IScope : public IUnknown
{
public:
	virtual HRESULT STDMETHODCALLTYPE AddScopeItem(IScopeItem *scopeItem) = 0;
};

MIDL_INTERFACE("BD59C2F9-F763-400D-A76E-028C35D047B8")
ISearchFolderItemFactoryPrivEx : public IUnknown
{
public:
	virtual HRESULT STDMETHODCALLTYPE SetScopeWithDepth(IShellItemArray* scope, SCOPE_ITEM_DEPTH depth) = 0;
	virtual HRESULT STDMETHODCALLTYPE SetScopeDirect(IScope *scope) = 0;
};

MIDL_INTERFACE("18455d05-d8f8-47f0-ba4c-c3aaf9c7035f")
ISearchFolderItemFactoryPriv : public IUnknown
{
public:
	virtual HRESULT STDMETHODCALLTYPE SetScopeWithDepth(IShellItemArray* scope, SCOPE_ITEM_DEPTH depth) = 0;
};

//1E87508D-89C2-42F0-8A7E-645A0F50CA58
static const CLSID FOLDERID_AppsFolder = { 0x1E87508D, 0x89C2, 0x42F0, { 0x8A, 0x7E, 0x64, 0x5A, 0x0F, 0x50, 0xCA, 0x58 } };

MIDL_INTERFACE("5CEC3DE3-0905-4301-955B-C871C0FFEE84")
IImmersiveApplication : public IUnknown
{
public:
	virtual HRESULT STDMETHODCALLTYPE Hide() = 0;
	virtual HRESULT STDMETHODCALLTYPE Show() = 0;
	virtual HRESULT STDMETHODCALLTYPE Activate() = 0;
	virtual HRESULT STDMETHODCALLTYPE SetPos() = 0;
	virtual HRESULT STDMETHODCALLTYPE MoveMonitor() = 0;
	virtual HRESULT STDMETHODCALLTYPE LockPresentation() = 0;
	virtual HRESULT STDMETHODCALLTYPE UnlockPresentation() = 0;
	virtual HRESULT STDMETHODCALLTYPE GetWindow() = 0;
	virtual HRESULT STDMETHODCALLTYPE GetApplicationId() = 0;
	virtual HRESULT STDMETHODCALLTYPE OpenPropertyStore(IMMAPPPROPERTYSTOREFLAGS flags, const IID &iid, LPVOID *ppv) = 0;
};

const PROPERTYKEY PKEY_ImmersiveApplication_VisibleTime = {
	{ 0xD33EA413, 0xDB44, 0x4CDC, { 0x88, 0xE6, 0xD2, 0x83, 0xAA, 0x98, 0x5A, 0xEF } },
	8
};