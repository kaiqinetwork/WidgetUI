#include "StdAfx.h"
#include "AxWidget.h"
#include <boost/bind.hpp>
#include "WidgetHostWindow.h"
#include "Keys.h"
#include "InputMessage.h"

namespace wui {

class AxContainer :
	public IOleClientSite,
	public IOleInPlaceSiteWindowless,
	public IOleControlSite,
	public IObjectWithSite,
	public IOleContainer
{
	friend class AxWnd;

public:
	AxContainer();
	~AxContainer();

	// IUnknown
	STDMETHOD_(ULONG,AddRef)();
	STDMETHOD_(ULONG,Release)();
	STDMETHOD(QueryInterface)(REFIID riid, LPVOID *ppvObject);

	// IObjectWithSite
	STDMETHOD(SetSite)(IUnknown *pUnkSite);
	STDMETHOD(GetSite)(REFIID riid, LPVOID* ppvSite);

	// IOleClientSite
	STDMETHOD(SaveObject)(void);       
	STDMETHOD(GetMoniker)(DWORD dwAssign, DWORD dwWhichMoniker, IMoniker** ppmk);
	STDMETHOD(GetContainer)(IOleContainer** ppContainer);        
	STDMETHOD(ShowObject)(void);        
	STDMETHOD(OnShowWindow)(BOOL fShow);        
	STDMETHOD(RequestNewObjectLayout)(void);

	// IOleInPlaceSiteWindowless
	STDMETHOD(CanWindowlessActivate)(void);
	STDMETHOD(GetCapture)(void);
	STDMETHOD(SetCapture)(BOOL fCapture);
	STDMETHOD(GetFocus)(void);
	STDMETHOD(SetFocus)(BOOL fFocus);
	STDMETHOD(GetDC)(LPCRECT pRect, DWORD grfFlags, HDC* phDC);
	STDMETHOD(ReleaseDC)(HDC hDC);
	STDMETHOD(InvalidateRect)(LPCRECT pRect, BOOL fErase);
	STDMETHOD(InvalidateRgn)(HRGN hRGN, BOOL fErase);
	STDMETHOD(ScrollRect)(INT dx, INT dy, LPCRECT pRectScroll, LPCRECT pRectClip);
	STDMETHOD(AdjustRect)(LPRECT prc);
	STDMETHOD(OnDefWindowMessage)(UINT msg, WPARAM wParam, LPARAM lParam, LRESULT* plResult);

	// IOleInPlaceSiteEx
	STDMETHOD(OnInPlaceActivateEx)(BOOL *pfNoRedraw, DWORD dwFlags);        
	STDMETHOD(OnInPlaceDeactivateEx)(BOOL fNoRedraw);       
	STDMETHOD(RequestUIActivate)(void);

	// IOleInPlaceSite
	STDMETHOD(CanInPlaceActivate)(void);       
	STDMETHOD(OnInPlaceActivate)(void);        
	STDMETHOD(OnUIActivate)(void);
	STDMETHOD(GetWindowContext)(IOleInPlaceFrame** ppFrame, IOleInPlaceUIWindow** ppDoc, LPRECT lprcPosRect, LPRECT lprcClipRect, LPOLEINPLACEFRAMEINFO lpFrameInfo);
	STDMETHOD(Scroll)(SIZE scrollExtant);
	STDMETHOD(OnUIDeactivate)(BOOL fUndoable);
	STDMETHOD(OnInPlaceDeactivate)(void);
	STDMETHOD(DiscardUndoState)( void);
	STDMETHOD(DeactivateAndUndo)( void);
	STDMETHOD(OnPosRectChange)(LPCRECT lprcPosRect);

	// IOleWindow
	STDMETHOD(GetWindow)(HWND* phwnd);
	STDMETHOD(ContextSensitiveHelp)(BOOL fEnterMode);

	// IOleControlSite
	STDMETHOD(OnControlInfoChanged)(void);      
	STDMETHOD(LockInPlaceActive)(BOOL fLock);       
	STDMETHOD(GetExtendedControl)(IDispatch** ppDisp);        
	STDMETHOD(TransformCoords)(POINTL* pPtlHimetric, POINTF* pPtfContainer, DWORD dwFlags);       
	STDMETHOD(TranslateAccelerator)(MSG* pMsg, DWORD grfModifiers);
	STDMETHOD(OnFocus)(BOOL fGotFocus);
	STDMETHOD(ShowPropertyFrame)(void);

	// IOleContainer
	STDMETHOD(EnumObjects)(DWORD grfFlags, IEnumUnknown** ppenum);
	STDMETHOD(LockContainer)(BOOL fLock);

	// IParseDisplayName
	STDMETHOD(ParseDisplayName)(IBindCtx* pbc, LPOLESTR pszDisplayName, ULONG* pchEaten, IMoniker** ppmkOut);

public:
	void Init(AxWidget *pAxWidge);
	bool CreateControl(const std::string &strModule, REFCLSID rclsid);
	IUnknown * GetControlUnknown(void);
	HRESULT DoVerb(LONG iVerb);
	HWND GetHostWnd(void);
	void SetGeometry(const RECT &rcWnd);
	void OnRender(HDC hdc, const RECT &rcDraw);
	HRESULT OnProcessMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	HRESULT OnTranslateAccelerator(MSG &msg);
	bool IsWindowless() const { return m_bWindowless; }

	HRESULT CreateActiveXWnd();

protected:
	AxWidget * m_pAxWidge;
	IOleObject *m_pOleObject;
	AxWnd* m_pWindow;
	IUnknown* m_pUnkSite;				// this ???
	IViewObject* m_pViewObject;
	IOleInPlaceObjectWindowless* m_pInPlaceObject;

	LONG m_dwRef;
	bool m_bLocked;
	bool m_bFocused;
	bool m_bCaptured;
	bool m_bUIActivated;
	bool m_bInPlaceActive;
	bool m_bWindowless;
	HWND m_hwndHost;
};


class AxEnum : public IEnumUnknown
{
public:
	AxEnum(IUnknown* pUnk);
	~AxEnum();
	STDMETHOD_(ULONG,AddRef)();
	STDMETHOD_(ULONG,Release)();
	STDMETHOD(QueryInterface)(REFIID riid, LPVOID *ppvObject);
	STDMETHOD(Next)(ULONG celt, IUnknown **rgelt, ULONG *pceltFetched);
	STDMETHOD(Skip)(ULONG celt);
	STDMETHOD(Reset)(void);
	STDMETHOD(Clone)(IEnumUnknown **ppenum);

private:
	LONG m_iPos;
	ULONG m_dwRef;
	IUnknown* m_pUnk;
};

class AxFrameWnd : public IOleInPlaceFrame
{
public:
	AxFrameWnd(HWND hContainer);
	~AxFrameWnd();

	// IUnknown
	STDMETHOD_(ULONG,AddRef)();
	STDMETHOD_(ULONG,Release)();
	STDMETHOD(QueryInterface)(REFIID riid, LPVOID *ppvObject);

	// IOleInPlaceFrameWindow
	STDMETHOD(InsertMenus)(HMENU /*hmenuShared*/, LPOLEMENUGROUPWIDTHS /*lpMenuWidths*/);
	STDMETHOD(SetMenu)(HMENU /*hmenuShared*/, HOLEMENU /*holemenu*/, HWND /*hwndActiveObject*/);
	STDMETHOD(RemoveMenus)(HMENU /*hmenuShared*/);
	STDMETHOD(SetStatusText)(LPCOLESTR /*pszStatusText*/);
	STDMETHOD(EnableModeless)(BOOL /*fEnable*/);
	STDMETHOD(TranslateAccelerator)(LPMSG /*lpMsg*/, WORD /*wID*/);
	// IOleWindow
	STDMETHOD(GetWindow)(HWND* phwnd);
	STDMETHOD(ContextSensitiveHelp)(BOOL /*fEnterMode*/);
	// IOleInPlaceUIWindow
	STDMETHOD(GetBorder)(LPRECT /*lprectBorder*/);
	STDMETHOD(RequestBorderSpace)(LPCBORDERWIDTHS /*pborderwidths*/);
	STDMETHOD(SetBorderSpace)(LPCBORDERWIDTHS /*pborderwidths*/);
	STDMETHOD(SetActiveObject)(IOleInPlaceActiveObject* pActiveObject, LPCOLESTR /*pszObjName*/);

private:
	ULONG m_dwRef;
	HWND m_hContainer;
	IOleInPlaceActiveObject* m_pActiveObject;
};

class AxWnd
{
public:
	AxWnd(AxContainer* pContainer);
	~AxWnd();
	HWND Create(HWND hWndParent);
	HWND Create(HWND hwndParent, LPCTSTR pstrName, DWORD dwStyle, DWORD dwExStyle, const RECT rc, HMENU hMenu = NULL);
	HWND Create(HWND hwndParent, LPCTSTR pstrName, DWORD dwStyle, DWORD dwExStyle, int x = CW_USEDEFAULT, int y = CW_USEDEFAULT, int cx = CW_USEDEFAULT, int cy = CW_USEDEFAULT, HMENU hMenu = NULL);

	LPCTSTR GetWindowClassName() const;
	HWND GetHWND() const;
	operator HWND() const;
	bool RegisterWindowClass();
	static LRESULT CALLBACK __WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	void DoVerb(LONG iVerb);

	LRESULT OnMouseActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSetFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnEraseBkgnd(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

private:
	HWND m_hWnd;
	AxContainer* m_pContainer;
};

//AxWidget
//////////////////////////////////////////////////////////////////////////
AxWidget::AxWidget(Widget* pParent) 
: Widget(pParent), m_dwRef(0), m_clsid(IID_NULL), m_pContainer(NULL), m_bCreated(false), m_bDelayCreate(false)
{
	sigProcessMessage().connect(boost::bind(&AxWidget::OnProcessMessage, this, _1));
	sigRender().connect(boost::bind(&AxWidget::OnRender, this, _1, _2));
}

AxWidget::~AxWidget()
{
	if ( m_pContainer != NULL ) 
	{
		delete m_pContainer;
		m_pContainer->Release();
		m_pContainer = NULL;
	}
}

RECT AxWidget::GetControlRect()
{
	return m_geometry.GetRectStruct();
}

HWND AxWidget::GetParentWnd()
{
	return Widget::GetHostWindow()->GetHwnd();
}

HWND AxWidget::GetHostWnd()
{
	return m_pContainer == NULL ? NULL : m_pContainer->GetHostWnd();
}

void AxWidget::SetFocus(bool bFocus)
{
	Widget::SetFocus(bFocus);
}

void AxWidget::SetCapture( bool bCapture )
{
	Widget::CaptureMouse(bCapture);
}

void AxWidget::SetDelayCreate(bool bDelayCreate)
{
	if( m_bDelayCreate == bDelayCreate ) 
		return;
	if( bDelayCreate == false && m_bCreated == false && m_clsid != IID_NULL) 
		DoCreateControl();

	m_bDelayCreate = bDelayCreate;
}

void AxWidget::SetModuleName(LPCTSTR pstrText)
{
	m_strModuleName = pstrText;
}

bool AxWidget::CreateControl(LPCTSTR pstrCLSID)
{
	CLSID clsid = { 0 };
	OLECHAR szCLSID[100] = { 0 };
#ifndef _UNICODE
	::MultiByteToWideChar(::GetACP(), 0, pstrCLSID, -1, szCLSID, _countof(szCLSID) - 1);
#else
	_tcsncpy(szCLSID, pstrCLSID, _countof(szCLSID) - 1);
#endif
	if ( pstrCLSID[0] == '{' ) 
		::CLSIDFromString(szCLSID, &clsid);
	else 
		::CLSIDFromProgID(szCLSID, &clsid);
	return CreateControl(clsid);
}

bool AxWidget::CreateControl(const CLSID clsid)
{
	if ( clsid == IID_NULL ) 
		return false;
	m_bCreated = false;
	m_clsid = clsid;
	if ( !m_bDelayCreate ) 
		DoCreateControl();
	return true;
}

void AxWidget::SetVisible( bool bVisible /*= true*/, bool bActivateRootWin /*= true*/ )
{
	Widget::SetVisible(bVisible);

	if( m_pContainer != NULL && m_pContainer->GetHostWnd() != NULL) 
		::ShowWindow(m_pContainer->GetHostWnd(), bVisible ? SW_SHOW : SW_HIDE);
}

void AxWidget::SetGeometry(Rect geometry)
{
	Widget::SetGeometry(geometry);

	if ( !m_bCreated ) 
		DoCreateControl();

	if ( m_pContainer == NULL)
		return;
	m_pContainer->SetGeometry(m_geometry.GetRectStruct());
}

void AxWidget::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
	//     if( _tcscmp(pstrName, _T("clsid")) == 0 ) CreateControl(pstrValue);
	//     else if( _tcscmp(pstrName, _T("modulename")) == 0 ) SetModuleName(pstrValue);
	//     else if( _tcscmp(pstrName, _T("delaycreate")) == 0 ) SetDelayCreate(_tcscmp(pstrValue, _T("true")) == 0);
	//     else CControlUI::SetAttribute(pstrName, pstrValue);
}

CLSID AxWidget::GetClisd() const
{
	return m_clsid;
}

bool AxWidget::IsDelayCreate() const
{
	return m_bDelayCreate;
}

const std::string & AxWidget::GetModuleName() const
{
	return m_strModuleName;
}

void AxWidget::OnRender( XGraphics& graphic, const Rect& rcUpdate )
{
	if (!IsVisible() || m_pContainer == NULL) 
		return;

	RECT rcControl = Rect(rcUpdate).GetRectStruct();
	m_pContainer->OnRender(graphic.GetDC(), rcControl);
}

void AxWidget::OnProcessMessage( Message& wuiMsg )
{
	if (m_pContainer == NULL || !m_pContainer->IsWindowless())
		return;
	MSG winMsg;
	winMsg.hwnd = GetParentWnd();
	if (!wuiMessageToWinMsg(wuiMsg, winMsg))
		return;
	m_pContainer->OnProcessMessage(winMsg.message, winMsg.wParam, winMsg.lParam);
}

bool AxWidget::OnTranslateAccelerator( MSG &msg )
{
	if(msg.message < WM_KEYFIRST || msg.message > WM_KEYLAST)
		return false;

	// 当前Web窗口不是焦点,不处理快捷键
	BOOL bIsChild = FALSE;
	HWND hTempWnd = NULL;
	HWND hWndFocus = ::GetFocus();

	hTempWnd = hWndFocus;
	while(hTempWnd != NULL)
	{
		if(hTempWnd == m_pContainer->GetHostWnd())
		{
			bIsChild = TRUE;
			break;
		}
		hTempWnd = ::GetParent(hTempWnd);
	}
	if(!bIsChild)
		return false;

	return S_OK == m_pContainer->OnTranslateAccelerator(msg);
}

bool AxWidget::DoCreateControl()
{
	if (m_bCreated || m_pContainer != NULL)
		return false;
	m_pContainer = new AxContainer();
	if (m_pContainer == NULL)
		return false;

	m_pContainer->Init(this);
	if (!m_pContainer->CreateControl(m_strModuleName, m_clsid))
		return false;

	m_bCreated = true;

	return true;
}

bool AxWidget::wuiMessageToWinMsg( Message &wuiMsg, MSG &winMsg )
{
	if (wuiMsg.IsType(InputMessage::GetTypeName()))
	{
		InputMessage input_msg = wuiMsg;
		InputEvent e = input_msg.GetEvent();

		if(e.m_type = InputEvent::POINTER_MOVED)
		{
			winMsg.message = WM_MOUSEMOVE;
			winMsg.wParam = 0;
			winMsg.lParam = MAKELONG(e.m_ptMouse.x, e.m_ptMouse.y);
		}
		// 无WM_SYSKEYDOWN和WM_SYSKEYUP消息
		else if(e.m_type = InputEvent::PRESSED)
		{
			winMsg.wParam = 0;
			winMsg.lParam = 0;
			if (e.m_id == WUI_MOUSE_LEFT)	winMsg.message = WM_LBUTTONDOWN;
			else if (e.m_id == WUI_MOUSE_RIGHT)	winMsg.message = WM_RBUTTONDOWN;
			else if (e.m_id == WUI_MOUSE_MIDDLE)	winMsg.message = WM_MBUTTONDOWN;
			else if (e.m_id == WUI_MOUSE_XBUTTON1)	winMsg.message = WM_XBUTTONDOWN;
			else if (e.m_id == WUI_MOUSE_XBUTTON2)	winMsg.message = WM_XBUTTONDOWN;
			else
			{
				winMsg.message = WM_KEYDOWN;
				winMsg.wParam = e.m_id;
				winMsg.lParam = 1<<16;
			}
		}
		else if(e.m_type = InputEvent::RELEASED)
		{
			winMsg.wParam = 0;
			winMsg.lParam = 0;
			if (e.m_id == WUI_MOUSE_LEFT)	winMsg.message = WM_LBUTTONUP;
			else if (e.m_id == WUI_MOUSE_RIGHT)	winMsg.message = WM_RBUTTONUP;
			else if (e.m_id == WUI_MOUSE_MIDDLE)	winMsg.message = WM_MBUTTONUP;
			else if (e.m_id == WUI_MOUSE_XBUTTON1)	winMsg.message = WM_XBUTTONUP;
			else if (e.m_id == WUI_MOUSE_XBUTTON2)	winMsg.message = WM_XBUTTONUP;
			else
			{
				winMsg.message = WM_KEYUP;
				winMsg.wParam = e.m_id;
				winMsg.lParam = (1<<16) + (1<<2) + 1;
			}
		}
		else if(e.m_type = InputEvent::DOUBLECLICK)
		{
			if (e.m_id == WUI_MOUSE_LEFT)	winMsg.message = WM_LBUTTONDBLCLK;
			else if (e.m_id == WUI_MOUSE_RIGHT)	winMsg.message = WM_RBUTTONDBLCLK;
			else if (e.m_id == WUI_MOUSE_MIDDLE)	winMsg.message = WM_MBUTTONDBLCLK;
			else if (e.m_id == WUI_MOUSE_XBUTTON1)	winMsg.message = WM_XBUTTONDBLCLK;
			else if (e.m_id == WUI_MOUSE_XBUTTON2)	winMsg.message = WM_XBUTTONDBLCLK;
			winMsg.wParam = 0;
			winMsg.lParam = 0;
		}
		else
			return false;
	}
	else
		return false;
// 	else if (wuiMsg.IsType(FocusChangeMessage::GetTypeName()))
// 	{
// 	}
// 	else if (wuiMsg.IsType(ActivationChangeMessage::GetTypeName()))
// 	{
// 	}
// 	else if (wuiMsg.IsType(CloseMessage::GetTypeName()))
// 	{
// 	}
// 	else if (wuiMsg.IsType(ResizeMessage::GetTypeName()))
// 	{
// 	}
	return true;
}

LPUNKNOWN AxWidget::GetUnknownControl()
{
	return m_pContainer->GetControlUnknown();
}

//////////////////////////////////////////////////////////////////////////
//AxContainer

AxContainer::AxContainer() 
	: m_pAxWidge(NULL), m_pWindow(NULL), m_pOleObject(NULL), m_pUnkSite(NULL), m_pViewObject(NULL)
	, m_pInPlaceObject(NULL),m_dwRef(0),  m_bLocked(false), m_bFocused(false), m_bCaptured(false)
	, m_bUIActivated(false), m_bInPlaceActive(false), m_bWindowless(true), m_hwndHost(NULL)
{
}

AxContainer::~AxContainer()
{
	if ( m_pWindow != NULL ) 
	{
		::DestroyWindow(*m_pWindow);
		delete m_pWindow;
	}
	m_hwndHost = NULL;	
	if (m_pAxWidge != NULL)
	{
		m_pAxWidge = NULL;
	}

	if ( m_pOleObject != NULL ) 
	{
		SetSite(NULL);
		m_pOleObject->Close(OLECLOSE_NOSAVE);
		m_pOleObject->SetClientSite(NULL);
		m_pOleObject->Release(); 
		m_pOleObject = NULL;
	}

	if ( m_pViewObject != NULL ) m_pViewObject->Release();
	if ( m_pInPlaceObject != NULL ) m_pInPlaceObject->Release();
	if ( m_pUnkSite != NULL ) m_pUnkSite->Release();
}

STDMETHODIMP_(ULONG) AxContainer::AddRef()
{
	return ++m_dwRef;
}

STDMETHODIMP_(ULONG) AxContainer::Release()
{
	if (--m_dwRef == 0)
	{
		delete this;
		return 0;
	}
	return m_dwRef;
}

STDMETHODIMP AxContainer::QueryInterface(REFIID riid, LPVOID *ppvObject)
{
	*ppvObject = NULL;
	if ( riid == IID_IUnknown )                       *ppvObject = static_cast<IOleWindow*>(this);
	else if ( riid == IID_IOleClientSite )            *ppvObject = static_cast<IOleClientSite*>(this);
	else if ( riid == IID_IOleInPlaceSiteWindowless ) *ppvObject = static_cast<IOleInPlaceSiteWindowless*>(this);
	else if ( riid == IID_IOleInPlaceSiteEx )         *ppvObject = static_cast<IOleInPlaceSiteEx*>(this);
	else if ( riid == IID_IOleInPlaceSite )           *ppvObject = static_cast<IOleInPlaceSite*>(this);
	else if ( riid == IID_IOleWindow )                *ppvObject = static_cast<IOleWindow*>(this);
	else if ( riid == IID_IOleControlSite )           *ppvObject = static_cast<IOleControlSite*>(this);
	else if ( riid == IID_IOleContainer )             *ppvObject = static_cast<IOleContainer*>(this);
	else if ( riid == IID_IObjectWithSite )           *ppvObject = static_cast<IObjectWithSite*>(this);
	if ( *ppvObject != NULL ) 
		AddRef();
	return *ppvObject == NULL ? E_NOINTERFACE : S_OK;
}

STDMETHODIMP AxContainer::SetSite(IUnknown *pUnkSite)
{
	if ( m_pUnkSite != NULL ) 
		m_pUnkSite->Release();
	if (pUnkSite)
		pUnkSite->QueryInterface(IID_IUnknown, (LPVOID*)&m_pUnkSite);
	return S_OK;
}

STDMETHODIMP AxContainer::GetSite(REFIID riid, LPVOID* ppvSite)
{
	if ( ppvSite == NULL ) 
		return E_POINTER;
	*ppvSite = NULL;
	if ( m_pUnkSite == NULL ) 
		return E_FAIL;
	return m_pUnkSite->QueryInterface(riid, ppvSite);
}

STDMETHODIMP AxContainer::SaveObject(void)
{
	return E_NOTIMPL;
}

STDMETHODIMP AxContainer::GetMoniker(DWORD dwAssign, DWORD dwWhichMoniker, IMoniker** ppmk)
{
	if( ppmk != NULL ) 
		*ppmk = NULL;
	return E_NOTIMPL;
}

STDMETHODIMP AxContainer::GetContainer(IOleContainer** ppContainer)
{
	if ( ppContainer == NULL ) 
		return E_POINTER;
	*ppContainer = NULL;
	HRESULT Hr = E_NOTIMPL;
	if ( m_pUnkSite != NULL ) 
		Hr = m_pUnkSite->QueryInterface(IID_IOleContainer, (LPVOID*) ppContainer);
	if ( FAILED(Hr) ) 
		Hr = QueryInterface(IID_IOleContainer, (LPVOID*) ppContainer);
	return Hr;
}

STDMETHODIMP AxContainer::ShowObject(void)
{
	if ( m_pAxWidge == NULL ) 
		return E_UNEXPECTED;
	HDC hDC = ::GetDC(m_hwndHost);
	if ( hDC == NULL ) 
		return E_FAIL;
	if ( m_pViewObject != NULL ) 
	{
		RECT rcItem = m_pAxWidge->GetControlRect();
		m_pViewObject->Draw(DVASPECT_CONTENT, -1, NULL, NULL, NULL, hDC, (RECTL*) &rcItem, (RECTL*) &rcItem, NULL, NULL);
	}
	::ReleaseDC(m_hwndHost, hDC);
	return S_OK;
}

STDMETHODIMP AxContainer::OnShowWindow(BOOL fShow)
{
	return E_NOTIMPL;
}

STDMETHODIMP AxContainer::RequestNewObjectLayout(void)
{
	return E_NOTIMPL;
}

STDMETHODIMP AxContainer::CanWindowlessActivate(void)
{
	return S_OK;  // Yes, we can!!
}

STDMETHODIMP AxContainer::GetCapture(void)
{
	if ( m_pAxWidge == NULL )
		return E_UNEXPECTED;
	return m_bCaptured ? S_OK : S_FALSE;
}

STDMETHODIMP AxContainer::SetCapture(BOOL fCapture)
{
	if ( m_pAxWidge == NULL ) 
		return E_UNEXPECTED;
	m_bCaptured = (fCapture == TRUE);
	if ( fCapture ) 
		::SetCapture(m_hwndHost); 
	else 
		::ReleaseCapture();
	if (m_pAxWidge)
		m_pAxWidge->SetCapture((bool)fCapture);
	return S_OK;
}

STDMETHODIMP AxContainer::GetFocus(void)
{
	if ( m_pAxWidge == NULL ) 
		return E_UNEXPECTED;
	return m_bFocused ? S_OK : S_FALSE;
}

STDMETHODIMP AxContainer::SetFocus(BOOL fFocus)
{
	if ( m_pAxWidge == NULL ) 
		return E_UNEXPECTED;

	if (m_pAxWidge) 
		m_pAxWidge->SetFocus(fFocus == TRUE);
	m_bFocused = (fFocus == TRUE);
	return S_OK;
}

STDMETHODIMP AxContainer::GetDC(LPCRECT pRect, DWORD grfFlags, HDC* phDC)
{
	if ( phDC == NULL ) 
		return E_POINTER;
	if ( m_pAxWidge == NULL ) 
		return E_UNEXPECTED;
	*phDC = ::GetDC(m_hwndHost);
	if ( (grfFlags & OLEDC_PAINTBKGND) != 0 ) 
	{
		RECT rcItem = m_pAxWidge->GetControlRect();
		if ( !m_bWindowless ) 
			::OffsetRect(&rcItem, -rcItem.left, -rcItem.top);
		::FillRect(*phDC, &rcItem, (HBRUSH) (COLOR_WINDOW + 1));
	}
	return S_OK;
}

STDMETHODIMP AxContainer::ReleaseDC(HDC hDC)
{
	if ( m_pAxWidge == NULL ) 
		return E_UNEXPECTED;
	::ReleaseDC(m_hwndHost, hDC);
	return S_OK;
}

STDMETHODIMP AxContainer::InvalidateRect(LPCRECT pRect, BOOL fErase)
{
	if ( m_pAxWidge == NULL ) 
		return E_UNEXPECTED;
	if ( m_hwndHost == NULL ) 
		return E_FAIL;
	return ::InvalidateRect(m_hwndHost, pRect, fErase) ? S_OK : E_FAIL;
}

STDMETHODIMP AxContainer::InvalidateRgn(HRGN hRGN, BOOL fErase)
{
	if ( m_pAxWidge == NULL )
		return E_UNEXPECTED;
	return ::InvalidateRgn(m_hwndHost, hRGN, fErase) ? S_OK : E_FAIL;
}

STDMETHODIMP AxContainer::ScrollRect(INT dx, INT dy, LPCRECT pRectScroll, LPCRECT pRectClip)
{
	return S_OK;
}

STDMETHODIMP AxContainer::AdjustRect(LPRECT prc)
{
	return S_OK;
}

STDMETHODIMP AxContainer::OnDefWindowMessage(UINT msg, WPARAM wParam, LPARAM lParam, LRESULT* plResult)
{
	if ( m_pAxWidge == NULL ) 
		return E_UNEXPECTED;
	*plResult = ::DefWindowProc(m_hwndHost, msg, wParam, lParam);
	return S_OK;
}

STDMETHODIMP AxContainer::OnInPlaceActivateEx(BOOL* pfNoRedraw, DWORD dwFlags)        
{
	if ( m_pAxWidge == NULL ) 
		return E_UNEXPECTED;
	if ( m_pOleObject == NULL ) 
		return E_UNEXPECTED;
	::OleLockRunning(m_pOleObject, TRUE, FALSE);
	HWND hWndFrame = m_pAxWidge->GetParentWnd();
	HRESULT hr = E_FAIL;
	if ( (dwFlags & ACTIVATE_WINDOWLESS) != 0 ) 
	{
		m_bWindowless = true;
		hr = m_pOleObject->QueryInterface(IID_IOleInPlaceObjectWindowless, (LPVOID*) &m_pInPlaceObject);
		m_hwndHost = hWndFrame;
	}
	if ( FAILED(hr) ) 
	{
		m_bWindowless = false;
		hr = CreateActiveXWnd();
		if ( FAILED(hr) ) 
			return hr;
		hr = m_pOleObject->QueryInterface(IID_IOleInPlaceObject, (LPVOID*) &m_pInPlaceObject);
	}
	if ( m_pInPlaceObject != NULL ) 
	{
		RECT rcItem = m_pAxWidge->GetControlRect();
		if ( !m_bWindowless ) 
			::OffsetRect(&rcItem, -rcItem.left, -rcItem.top);
		m_pInPlaceObject->SetObjectRects(&rcItem, &rcItem);
	}
	m_bInPlaceActive = SUCCEEDED(hr);
	return hr;
}

STDMETHODIMP AxContainer::OnInPlaceDeactivateEx(BOOL fNoRedraw)       
{
	m_bInPlaceActive = false;
	if ( m_pInPlaceObject != NULL ) 
	{
		m_pInPlaceObject->Release();
		m_pInPlaceObject = NULL;
	}
	if( m_pWindow != NULL ) 
	{
		::DestroyWindow(*m_pWindow);
		delete m_pWindow;
		m_pWindow = NULL;
	}
	return S_OK;
}

STDMETHODIMP AxContainer::RequestUIActivate(void)
{
	return S_OK;
}

STDMETHODIMP AxContainer::CanInPlaceActivate(void)       
{
	return S_OK;
}

STDMETHODIMP AxContainer::OnInPlaceActivate(void)
{
	BOOL bDummy = FALSE;
	return OnInPlaceActivateEx(&bDummy, 0);
}

STDMETHODIMP AxContainer::OnUIActivate(void)
{
	m_bUIActivated = true;
	return S_OK;
}

STDMETHODIMP AxContainer::GetWindowContext(IOleInPlaceFrame** ppFrame, IOleInPlaceUIWindow** ppDoc, LPRECT lprcPosRect, LPRECT lprcClipRect, LPOLEINPLACEFRAMEINFO lpFrameInfo)
{
	if ( ppDoc == NULL ) 
		return E_POINTER;
	if ( ppFrame == NULL ) 
		return E_POINTER;
	if ( lprcPosRect == NULL ) 
		return E_POINTER;
	if ( lprcClipRect == NULL ) 
		return E_POINTER;
	if (m_pWindow != NULL)
	{
		::GetClientRect(m_pWindow->GetHWND(),lprcPosRect);
		::GetClientRect(m_pWindow->GetHWND(),lprcClipRect);
	}
	*ppFrame = new AxFrameWnd(m_hwndHost);
	*ppDoc = NULL;
	ACCEL ac = { 0 };
	HACCEL hac = ::CreateAcceleratorTable(&ac, 1);
	lpFrameInfo->cb = sizeof(OLEINPLACEFRAMEINFO);
	lpFrameInfo->fMDIApp = FALSE;
	lpFrameInfo->hwndFrame = m_hwndHost;
	lpFrameInfo->haccel = hac;
	lpFrameInfo->cAccelEntries = 1;
	return S_OK;
}

STDMETHODIMP AxContainer::Scroll(SIZE scrollExtant)
{
	return E_NOTIMPL;
}

STDMETHODIMP AxContainer::OnUIDeactivate(BOOL fUndoable)
{
	m_bUIActivated = false;
	return S_OK;
}

STDMETHODIMP AxContainer::OnInPlaceDeactivate(void)
{
	return OnInPlaceDeactivateEx(TRUE);
}

STDMETHODIMP AxContainer::DiscardUndoState(void)
{
	return E_NOTIMPL;
}

STDMETHODIMP AxContainer::DeactivateAndUndo(void)
{
	return E_NOTIMPL;
}

STDMETHODIMP AxContainer::OnPosRectChange(LPCRECT lprcPosRect)
{
	return E_NOTIMPL;
}

STDMETHODIMP AxContainer::GetWindow(HWND* phwnd)
{
	if ( m_pAxWidge == NULL ) 
		return E_UNEXPECTED;
	if ( m_hwndHost == NULL ) 
		CreateActiveXWnd();
	if ( m_hwndHost == NULL ) 
		return E_FAIL;
	*phwnd = m_hwndHost;
	return S_OK;
}

STDMETHODIMP AxContainer::ContextSensitiveHelp(BOOL fEnterMode)
{
	return S_OK;
}

STDMETHODIMP AxContainer::OnControlInfoChanged(void)      
{
	return S_OK;
}

STDMETHODIMP AxContainer::LockInPlaceActive(BOOL fLock)       
{
	return S_OK;
}

STDMETHODIMP AxContainer::GetExtendedControl(IDispatch** ppDisp)        
{
	if ( ppDisp == NULL )
		return E_POINTER;   
	if ( m_pAxWidge == NULL ) 
		return E_UNEXPECTED;
	if ( m_pOleObject == NULL ) 
		return E_UNEXPECTED;
	return m_pOleObject->QueryInterface(IID_IDispatch, (LPVOID*) ppDisp);
}

STDMETHODIMP AxContainer::TransformCoords(POINTL* pPtlHimetric, POINTF* pPtfContainer, DWORD dwFlags)       
{
	return S_OK;
}

STDMETHODIMP AxContainer::TranslateAccelerator(MSG *pMsg, DWORD grfModifiers)
{
	return S_FALSE;
}

STDMETHODIMP AxContainer::OnFocus(BOOL fGotFocus)
{
	m_bFocused = (fGotFocus == TRUE);
	return S_OK;
}

STDMETHODIMP AxContainer::ShowPropertyFrame(void)
{
	return E_NOTIMPL;
}

STDMETHODIMP AxContainer::EnumObjects(DWORD grfFlags, IEnumUnknown** ppenum)
{
	if ( ppenum == NULL ) 
		return E_POINTER;
	if ( m_pAxWidge == NULL ) 
		return E_UNEXPECTED;
	*ppenum = new AxEnum(m_pOleObject);
	return S_OK;
}

STDMETHODIMP AxContainer::LockContainer(BOOL fLock)
{
	m_bLocked = (fLock != FALSE);
	return S_OK;
}

STDMETHODIMP AxContainer::ParseDisplayName(IBindCtx *pbc, LPOLESTR pszDisplayName, ULONG* pchEaten, IMoniker** ppmkOut)
{
	return E_NOTIMPL;
}

HRESULT AxContainer::CreateActiveXWnd()
{
	if ( m_pWindow != NULL ) 
		return S_OK;
	HWND hParentWnd = m_pAxWidge->GetParentWnd();
	if (hParentWnd == NULL)
		return S_FALSE;
	m_pWindow = new AxWnd(this);
	if( m_pWindow == NULL ) 
		return E_OUTOFMEMORY;
	m_hwndHost = m_pWindow->Create(hParentWnd);
	return S_OK;
}

void AxContainer::Init( AxWidget *pAxWidge )
{
	m_pAxWidge = pAxWidge;	 
}

typedef HRESULT (__stdcall *DllGetClassObjectFunc)(REFCLSID rclsid, REFIID riid, LPVOID* ppv); 

bool AxContainer::CreateControl( const std::string &strModule, REFCLSID rclsid )
{
	IOleControl* pOleControl = NULL;

	HRESULT hr = -1;
	if( !strModule.empty() ) 
	{
		HMODULE hModule = ::LoadLibrary((LPCTSTR)strModule.c_str());
		if( hModule != NULL ) 
		{
			IClassFactory* aClassFactory = NULL;
			DllGetClassObjectFunc aDllGetClassObjectFunc = (DllGetClassObjectFunc)::GetProcAddress(hModule, "DllGetClassObject");
			hr = aDllGetClassObjectFunc(rclsid, IID_IClassFactory, (LPVOID*)&aClassFactory);
			if( SUCCEEDED(hr) ) 
			{
				hr = aClassFactory->CreateInstance(NULL, IID_IOleObject, (LPVOID*)&pOleControl);
			}
			aClassFactory->Release();
		}
	}
	if( FAILED(hr) ) 
		hr = ::CoCreateInstance(rclsid, NULL, CLSCTX_ALL, IID_IOleControl, (LPVOID*)&pOleControl);

	if( FAILED(hr) ) 
		return false;
	pOleControl->QueryInterface(IID_IOleObject, (LPVOID*) &m_pOleObject);
	pOleControl->Release();
	if( m_pOleObject == NULL ) 
		return false;

	hr = m_pOleObject->QueryInterface(IID_IOleInPlaceObjectWindowless, (LPVOID*) &m_pInPlaceObject);

	// More control creation stuff
	DWORD dwMiscStatus = 0;
	m_pOleObject->GetMiscStatus(DVASPECT_CONTENT, &dwMiscStatus);
	// Initialize control
	if( (dwMiscStatus & OLEMISC_SETCLIENTSITEFIRST) != 0 ) 
		m_pOleObject->SetClientSite(this);
	IPersistStreamInit* pPersistStreamInit = NULL;
	m_pOleObject->QueryInterface(IID_IPersistStreamInit, (LPVOID*) &pPersistStreamInit);
	if( pPersistStreamInit != NULL )
	{
		hr = pPersistStreamInit->InitNew();
		pPersistStreamInit->Release();
	}
	if( FAILED(hr) ) 
		return false;
	if( (dwMiscStatus & OLEMISC_SETCLIENTSITEFIRST) == 0 ) 
		m_pOleObject->SetClientSite(this);
	// Grab the view...
	hr = m_pOleObject->QueryInterface(IID_IViewObjectEx, (LPVOID*) &m_pViewObject);
	if( FAILED(hr) ) 
		hr = m_pOleObject->QueryInterface(IID_IViewObject2, (LPVOID*) &m_pViewObject);
	if( FAILED(hr) ) 
		hr = m_pOleObject->QueryInterface(IID_IViewObject, (LPVOID*) &m_pViewObject);

	// Activate and done...
	m_pOleObject->SetHostNames(OLESTR("UIActiveX"), NULL);
	if( (dwMiscStatus & OLEMISC_INVISIBLEATRUNTIME) == 0 ) 
		DoVerb(OLEIVERB_INPLACEACTIVATE);

	IObjectWithSite* pSite = NULL;
	m_pOleObject->QueryInterface(IID_IObjectWithSite, (LPVOID*) &pSite);
	if( pSite != NULL ) 
	{
		pSite->SetSite(static_cast<IOleWindow*>(this));
		pSite->Release();
	}

	return SUCCEEDED(hr);
}

LPUNKNOWN AxContainer::GetControlUnknown()
{
	return m_pOleObject;
}

HRESULT AxContainer::DoVerb( LONG iVerb )
{
	if (m_pOleObject == NULL || m_pAxWidge == NULL)
		return E_POINTER;

	RECT rcControl = m_pAxWidge->GetControlRect();
	m_pOleObject->DoVerb(iVerb, NULL, this, 0, m_hwndHost, &rcControl);
	return S_OK;
}

HWND AxContainer::GetHostWnd()
{
	return m_hwndHost;
}

static void PixelToHiMetric(const SIZEL* lpSizeInPix, LPSIZEL lpSizeInHiMetric)
{
#define HIMETRIC_PER_INCH   2540
#define MAP_PIX_TO_LOGHIM(x,ppli)   MulDiv(HIMETRIC_PER_INCH, (x), (ppli))
#define MAP_LOGHIM_TO_PIX(x,ppli)   MulDiv((ppli), (x), HIMETRIC_PER_INCH)
	int nPixelsPerInchX;    // Pixels per logical inch along width
	int nPixelsPerInchY;    // Pixels per logical inch along height
	HDC hDCScreen = ::GetDC(NULL);
	nPixelsPerInchX = ::GetDeviceCaps(hDCScreen, LOGPIXELSX);
	nPixelsPerInchY = ::GetDeviceCaps(hDCScreen, LOGPIXELSY);
	::ReleaseDC(NULL, hDCScreen);
	lpSizeInHiMetric->cx = MAP_PIX_TO_LOGHIM(lpSizeInPix->cx, nPixelsPerInchX);
	lpSizeInHiMetric->cy = MAP_PIX_TO_LOGHIM(lpSizeInPix->cy, nPixelsPerInchY);
}

void AxContainer::SetGeometry( const RECT &rcWnd )
{
	SIZEL hmSize = { 0 };
	SIZEL pxSize = { 0 };
	pxSize.cx = rcWnd.right-rcWnd.left;
	pxSize.cy = rcWnd.bottom-rcWnd.top;
	PixelToHiMetric(&pxSize, &hmSize);

	if ( m_pOleObject != NULL ) 
	{
		m_pOleObject->SetExtent(DVASPECT_CONTENT, &hmSize);
	}

	if ( m_pInPlaceObject != NULL ) 
	{
		RECT rcItem = rcWnd;
		if( !m_bWindowless ) 
			::OffsetRect(&rcItem, -rcItem.left, -rcItem.top);
		m_pInPlaceObject->SetObjectRects(&rcItem, &rcItem);
	}
	if ( !m_bWindowless && m_pWindow) 
	{
		::MoveWindow(*m_pWindow, rcWnd.left, rcWnd.top, rcWnd.right-rcWnd.left, rcWnd.bottom-rcWnd.top, TRUE);
	}
}

void AxContainer::OnRender( HDC hdc, const RECT &rcDraw )
{
	if ( m_bWindowless && m_pViewObject != NULL )
	{
		m_pViewObject->Draw(DVASPECT_CONTENT, -1, NULL, NULL, NULL, 
			hdc, (RECTL*) &rcDraw, (RECTL*) &rcDraw, NULL, NULL); 
	}
}

HRESULT AxContainer::OnProcessMessage( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if (m_pInPlaceObject == NULL)
		return E_POINTER;
	LRESULT lResult = 0;
	return m_pInPlaceObject->OnWindowMessage(uMsg, wParam, lParam, &lResult);;
}

HRESULT AxContainer::OnTranslateAccelerator( MSG &msg )
{
	if (m_pOleObject == NULL)
		return S_FALSE;
	IOleInPlaceActiveObject *pObj;
	if (FAILED(m_pOleObject->QueryInterface(IID_IOleInPlaceActiveObject, (LPVOID *)&pObj)))
		return S_FALSE;

	HRESULT hResult = pObj->TranslateAccelerator(&msg);
	pObj->Release();
	return hResult;
}

//////////////////////////////////////////////////////////////////////////
//AxEnum

AxEnum::AxEnum(IUnknown* pUnk) : m_pUnk(pUnk), m_dwRef(1), m_iPos(0)
{
	m_pUnk->AddRef();
}
AxEnum::~AxEnum()
{
	m_pUnk->Release();
}

STDMETHODIMP_(ULONG) AxEnum::AddRef()
{
	return ++m_dwRef;
}

STDMETHODIMP_(ULONG) AxEnum::Release()
{
	LONG lRef = --m_dwRef;
	if( lRef == 0 ) delete this;
	return lRef;
}

STDMETHODIMP AxEnum::QueryInterface(REFIID riid, LPVOID *ppvObject)
{
	*ppvObject = NULL;
	if( riid == IID_IUnknown ) *ppvObject = static_cast<IEnumUnknown*>(this);
	else if( riid == IID_IEnumUnknown ) *ppvObject = static_cast<IEnumUnknown*>(this);
	if( *ppvObject != NULL ) AddRef();
	return *ppvObject == NULL ? E_NOINTERFACE : S_OK;
}

STDMETHODIMP AxEnum::Next(ULONG celt, IUnknown **rgelt, ULONG *pceltFetched)
{
	if( pceltFetched != NULL ) *pceltFetched = 0;
	if( ++m_iPos > 1 ) return S_FALSE;
	*rgelt = m_pUnk;
	(*rgelt)->AddRef();
	if( pceltFetched != NULL ) *pceltFetched = 1;
	return S_OK;
}

STDMETHODIMP AxEnum::Skip(ULONG celt)
{
	m_iPos += celt;
	return S_OK;
}

STDMETHODIMP AxEnum::Reset(void)
{
	m_iPos = 0;
	return S_OK;
}

STDMETHODIMP AxEnum::Clone(IEnumUnknown **ppenum)
{
	return E_NOTIMPL;
}

//////////////////////////////////////////////////////////////////////////
//AxFrameWnd
AxFrameWnd::AxFrameWnd( HWND hContainer )	
	: m_dwRef(1), m_hContainer(hContainer), m_pActiveObject(NULL)
{
}
AxFrameWnd::~AxFrameWnd()
{
	if( m_pActiveObject != NULL ) 
		m_pActiveObject->Release();
}

STDMETHODIMP_(ULONG) AxFrameWnd::AddRef()
{
	return ++m_dwRef;
}

STDMETHODIMP_(ULONG) AxFrameWnd::Release()
{
	if( --m_dwRef == 0 ) 
	{
		delete this;
		return 0;
	}
	return m_dwRef;
}

STDMETHODIMP AxFrameWnd::QueryInterface( REFIID riid, LPVOID *ppvObject )
{
	*ppvObject = NULL;
	if( riid == IID_IUnknown ) 
		*ppvObject = static_cast<IOleInPlaceFrame*>(this);
	else if( riid == IID_IOleWindow ) 
		*ppvObject = static_cast<IOleWindow*>(this);
	else if( riid == IID_IOleInPlaceFrame ) 
		*ppvObject = static_cast<IOleInPlaceFrame*>(this);
	else if( riid == IID_IOleInPlaceUIWindow ) 
		*ppvObject = static_cast<IOleInPlaceUIWindow*>(this);
	else
		return E_NOINTERFACE;

	AddRef();
	return S_OK;
}

STDMETHODIMP AxFrameWnd::InsertMenus( HMENU /*hmenuShared*/, LPOLEMENUGROUPWIDTHS /*lpMenuWidths*/ )
{
	return S_OK;
}

STDMETHODIMP AxFrameWnd::SetMenu( HMENU /*hmenuShared*/, HOLEMENU /*holemenu*/, HWND /*hwndActiveObject*/ )
{
	return S_OK;
}

STDMETHODIMP AxFrameWnd::RemoveMenus( HMENU /*hmenuShared*/ )
{
	return S_OK;
}

STDMETHODIMP AxFrameWnd::SetStatusText( LPCOLESTR /*pszStatusText*/ )
{
	return S_OK;
}

STDMETHODIMP AxFrameWnd::EnableModeless( BOOL /*fEnable*/ )
{
	return S_OK;
}

STDMETHODIMP AxFrameWnd::TranslateAccelerator( LPMSG /*lpMsg*/, WORD /*wID*/ )
{
	return S_FALSE;
}

STDMETHODIMP AxFrameWnd::GetWindow( HWND* phwnd )
{
	if( m_hContainer == NULL ) return E_UNEXPECTED;
	*phwnd = m_hContainer;
	return S_OK;
}

STDMETHODIMP AxFrameWnd::ContextSensitiveHelp( BOOL /*fEnterMode*/ )
{
	return S_OK;
}

STDMETHODIMP AxFrameWnd::GetBorder( LPRECT /*lprectBorder*/ )
{
	return S_OK;
}

STDMETHODIMP AxFrameWnd::RequestBorderSpace( LPCBORDERWIDTHS /*pborderwidths*/ )
{
	return INPLACE_E_NOTOOLSPACE;
}

STDMETHODIMP AxFrameWnd::SetBorderSpace( LPCBORDERWIDTHS /*pborderwidths*/ )
{
	return S_OK;
}

STDMETHODIMP AxFrameWnd::SetActiveObject( IOleInPlaceActiveObject* pActiveObject, LPCOLESTR /*pszObjName*/ )
{
	if( m_pActiveObject != NULL ) 
		m_pActiveObject->Release();

	pActiveObject->QueryInterface(IID_IOleInPlaceActiveObject, (VOID **)&m_pActiveObject);
	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
//AxWnd

AxWnd::AxWnd( AxContainer* pContainer)
	: m_pContainer(pContainer)
{
}

AxWnd::~AxWnd()
{

}

HWND AxWnd::Create(HWND hWndParent)
{	
	UINT uStyle = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	Create(hWndParent, "ActiveXWnd", uStyle, 0L, 0,0,0,0, NULL);
	return m_hWnd;
}

HWND AxWnd::Create(HWND hwndParent, LPCTSTR pstrName, DWORD dwStyle, DWORD dwExStyle, const RECT rc, HMENU hMenu)
{
	return Create(hwndParent, pstrName, dwStyle, dwExStyle, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, hMenu);
}

HWND AxWnd::Create(HWND hwndParent, LPCTSTR pstrName, DWORD dwStyle, DWORD dwExStyle, int x, int y, int cx, int cy, HMENU hMenu)
{
	if (!RegisterWindowClass() ) return NULL;
	m_hWnd = ::CreateWindowEx(dwExStyle, GetWindowClassName(), pstrName, dwStyle, x, y, cx, cy, hwndParent, hMenu, (HINSTANCE)GetModuleHandle(0), this);
	return m_hWnd;
}

LPCTSTR AxWnd::GetWindowClassName() const
{
	return "ActiveXWnd";
}

HWND AxWnd::GetHWND() const
{
	return m_hWnd;
}

AxWnd::operator HWND() const
{
	return m_hWnd;
}

bool AxWnd::RegisterWindowClass()
{
	WNDCLASS wc = { 0 };
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hIcon = NULL;
	wc.lpfnWndProc = AxWnd::__WndProc;
	wc.hInstance = (HINSTANCE)GetModuleHandle(0);
	wc.hCursor = ::LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;
	wc.lpszMenuName  = NULL;
	wc.lpszClassName = GetWindowClassName();
	ATOM ret = ::RegisterClass(&wc);
	return ret != NULL || ::GetLastError() == ERROR_CLASS_ALREADY_EXISTS;
}

void AxWnd::DoVerb(LONG iVerb)
{
	if( m_pContainer == NULL ) 
		return;

	m_pContainer->DoVerb(iVerb);
}

LRESULT AxWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes=0;
	BOOL bHandled = TRUE;
	switch( uMsg ) {
	case WM_PAINT:         lRes = OnPaint(uMsg, wParam, lParam, bHandled); break;
	case WM_SETFOCUS:      lRes = OnSetFocus(uMsg, wParam, lParam, bHandled); break;
	case WM_KILLFOCUS:     lRes = OnKillFocus(uMsg, wParam, lParam, bHandled); break;
	case WM_ERASEBKGND:    lRes = OnEraseBkgnd(uMsg, wParam, lParam, bHandled); break;
	case WM_MOUSEACTIVATE: lRes = OnMouseActivate(uMsg, wParam, lParam, bHandled); break;
	case WM_MOUSEWHEEL: break;
	default:
		bHandled = FALSE;
	}
	if( !bHandled ) 
		return ::DefWindowProc(m_hWnd, uMsg, wParam, lParam);
	return lRes;
}

LRESULT AxWnd::OnEraseBkgnd(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if( m_pContainer->m_pViewObject == NULL ) 
		bHandled = FALSE;

	return S_FALSE;
}

LRESULT AxWnd::OnMouseActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if ( m_pContainer->m_pOleObject == NULL ) 
		return 0;
	DWORD dwMiscStatus = 0;
	m_pContainer->m_pOleObject->GetMiscStatus(DVASPECT_CONTENT, &dwMiscStatus);
	if ( (dwMiscStatus & OLEMISC_NOUIACTIVATE) != 0 )
		return 0;
	if ( !m_pContainer->m_bInPlaceActive )
		DoVerb(OLEIVERB_INPLACEACTIVATE);
	bHandled = FALSE;
	return 0;
}

LRESULT AxWnd::OnSetFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	m_pContainer->m_bFocused = true;
	if( !m_pContainer->m_bUIActivated ) 
		DoVerb(OLEIVERB_UIACTIVATE);
	return 0;
}

LRESULT AxWnd::OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	m_pContainer->m_bFocused = false;
	return 0;
}

LRESULT AxWnd::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	PAINTSTRUCT ps = { 0 };
	::BeginPaint(m_hWnd, &ps);
	::LineTo(ps.hdc, 0,0);
	::LineTo(ps.hdc, 110,0);
	::LineTo(ps.hdc, 0,110);
	::LineTo(ps.hdc, 110,110);
	::EndPaint(m_hWnd, &ps);
	return 1;
}

LRESULT CALLBACK AxWnd::__WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	AxWnd *self = 0;
	if (uMsg == WM_CREATE)
	{
		LPCREATESTRUCT lpcs = (LPCREATESTRUCT) lParam;
		self = (AxWnd *) lpcs->lpCreateParams;
		SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR) self);
	}
	else
	{
		self = (AxWnd *) GetWindowLongPtr(hWnd, GWLP_USERDATA);
	}

	if (self)
		return self->HandleMessage(uMsg, wParam, lParam);
	else
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
} // namespace wui