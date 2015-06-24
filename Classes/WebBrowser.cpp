#include "StdAfx.h"
#include "WebBrowser.h"
#include <atlcomcli.h>
#include <mshtml.h>
#include <comdef.h>
#include <atlconv.h>
#include <boost/bind.hpp>
#include <boost/signal.hpp>

namespace wui{

boost::signal<bool (LPCTSTR, WebBrowser *)> WebBrowser::m_sigProcessUrl;

WebBrowser::WebBrowser(Widget* pParent) : AxWidget(pParent)
, m_dwRef(0), m_pWebBrowser2(NULL), _pHtmlWnd2(NULL)
, m_bAutoNavi(false), m_dwCookie(0), m_clrBk(RGB(255,255,255)), m_lFrame(0)
, m_pLoadingImage(NULL), m_bShowLoading(FALSE), m_bLoading(FALSE)
{
	m_clsid = CLSID_WebBrowser;
	m_Timer.sigExpired().connect(boost::bind(&WebBrowser::OnTimer, this));
}

bool WebBrowser::DoCreateControl()
{
	if (!AxWidget::DoCreateControl())
		return false;

	LPUNKNOWN lpUnk = GetUnknownControl();
	if (lpUnk == NULL)
		return false;
	lpUnk->QueryInterface(IID_IWebBrowser2, (LPVOID *)&m_pWebBrowser2);
	if (m_pWebBrowser2 == NULL)
		return false;

	RegisterEventHandler(TRUE);
	if ( m_bAutoNavi && !m_strHomePage.empty())
	{
		Navigate2(m_strHomePage.c_str());
	}

	return true;
}

void WebBrowser::OnRender( XGraphics& graphic, const Rect& rcUpdate )
{
	Rect rcGeometry = GetGeometry();
	graphic.FillSolidRect(0,0, rcGeometry.width, rcGeometry.height, m_clrBk);
	if (m_bShowLoading && m_pLoadingImage)
	{
		int x = (rcGeometry.width - m_pLoadingImage->GetWidth()) / 2;
		int y = (rcGeometry.height - m_pLoadingImage->GetHeight()) / 2;
		CxImage* pImage = m_pLoadingImage->GetFrame(m_lFrame);
		if (pImage)
		{
			pImage->Draw(graphic.GetDC(), x, y);
		}
	}

	AxWidget::OnRender(graphic, rcUpdate);
}

void WebBrowser::ReleaseControl()
{
	m_bCreated=false;
	RegisterEventHandler(FALSE);
}

WebBrowser::~WebBrowser()
{
	ReleaseControl();
}

STDMETHODIMP WebBrowser::GetTypeInfoCount( UINT *iTInfo )
{
	*iTInfo = 0;
	return E_NOTIMPL;
}

STDMETHODIMP WebBrowser::GetTypeInfo( UINT iTInfo, LCID lcid, ITypeInfo **ppTInfo )
{
	return E_NOTIMPL;
}

STDMETHODIMP WebBrowser::GetIDsOfNames( REFIID riid, OLECHAR **rgszNames, UINT cNames, LCID lcid,DISPID *rgDispId )
{
	return E_NOTIMPL;
}

STDMETHODIMP WebBrowser::Invoke( DISPID dispIdMember, REFIID riid, LCID lcid,WORD wFlags, DISPPARAMS* pDispParams,VARIANT* pVarResult, EXCEPINFO* pExcepInfo,UINT* puArgErr )
{
	if ((riid != IID_NULL))
		return E_INVALIDARG;

	switch (dispIdMember)
	{
	case DISPID_BEFORENAVIGATE2:
		BeforeNavigate2(
			pDispParams->rgvarg[6].pdispVal,
			pDispParams->rgvarg[5].pvarVal,
			pDispParams->rgvarg[4].pvarVal,
			pDispParams->rgvarg[3].pvarVal,
			pDispParams->rgvarg[2].pvarVal,
			pDispParams->rgvarg[1].pvarVal,
			pDispParams->rgvarg[0].pboolVal);
		break;
	case DISPID_COMMANDSTATECHANGE:
		CommandStateChange(
			pDispParams->rgvarg[1].lVal,
			pDispParams->rgvarg[0].boolVal);
		break;
	case DISPID_NAVIGATECOMPLETE2:
		NavigateComplete2(
			pDispParams->rgvarg[1].pdispVal,
			pDispParams->rgvarg[0].pvarVal);
		break;
	case DISPID_NAVIGATEERROR:
		NavigateError(
			pDispParams->rgvarg[4].pdispVal,
			pDispParams->rgvarg[3].pvarVal,
			pDispParams->rgvarg[2].pvarVal,
			pDispParams->rgvarg[1].pvarVal,
			pDispParams->rgvarg[0].pboolVal);
		break;
	case DISPID_STATUSTEXTCHANGE:
		break;
		//  	case DISPID_NEWWINDOW2:
		//  		break;
	case DISPID_NEWWINDOW3:
		NewWindow3(
			pDispParams->rgvarg[4].ppdispVal,
			pDispParams->rgvarg[3].pboolVal,
			pDispParams->rgvarg[2].uintVal,
			pDispParams->rgvarg[1].bstrVal,
			pDispParams->rgvarg[0].bstrVal);
		break;

	default:
		return DISP_E_MEMBERNOTFOUND;
	}
	return S_OK;
}

STDMETHODIMP WebBrowser::QueryInterface( REFIID riid, LPVOID *ppvObject )
{
	*ppvObject = NULL;

	if ( riid == IID_IDocHostUIHandler)
		*ppvObject = static_cast<IDocHostUIHandler*>(this);
	else if ( riid == IID_IDispatch)
		*ppvObject = static_cast<IDispatch*>(this);
	else if ( riid == IID_IServiceProvider)
		*ppvObject = static_cast<IServiceProvider*>(this);
	else if (riid == IID_IOleCommandTarget)
		*ppvObject = static_cast<IOleCommandTarget*>(this);

	if( *ppvObject != NULL )
		AddRef();
	return *ppvObject == NULL ? E_NOINTERFACE : S_OK;
}

STDMETHODIMP_(ULONG) WebBrowser::AddRef()
{
	InterlockedIncrement(&m_dwRef); 
	return m_dwRef;
}

STDMETHODIMP_(ULONG) WebBrowser::Release()
{
	ULONG ulRefCount = InterlockedDecrement(&m_dwRef);
	return ulRefCount; 
}

void WebBrowser::Navigate2( LPCTSTR lpszUrl )
{
	if (lpszUrl == NULL)
		return;

	if (m_pWebBrowser2)
	{
		CComVariant url(lpszUrl);
		HRESULT hr = m_pWebBrowser2->Navigate2(&url, NULL, NULL, NULL, NULL);
	}
}

void WebBrowser::Refresh()
{
	if (m_pWebBrowser2)
	{
		m_pWebBrowser2->Refresh();
	}
}
void WebBrowser::GoBack()
{
	if (m_pWebBrowser2)
	{
		m_pWebBrowser2->GoBack();
	}
}
void WebBrowser::GoForward()
{
	if (m_pWebBrowser2)
	{
		m_pWebBrowser2->GoForward();
	}
}

/// DWebBrowserEvents2
void WebBrowser::BeforeNavigate2( IDispatch *pDisp,VARIANT *&url,VARIANT *&Flags,VARIANT *&TargetFrameName,VARIANT *&PostData,VARIANT *&Headers,VARIANT_BOOL *&Cancel )
{
	*Cancel = FALSE;
	USES_CONVERSION;
	LPCTSTR lpstrUrl = W2T(url->bstrVal);
	if ((!m_sigProcessUrl.empty() && m_sigProcessUrl(lpstrUrl, this)) || 
		_tcsstr(lpstrUrl, _T("javascript:void(0);")) == lpstrUrl)
	{
		*Cancel = TRUE;
	}
	else if (m_bShowLoading && !m_bLoading)
	{
		BeginLoading();
	}
}

void WebBrowser::NavigateError( IDispatch *pDisp,VARIANT * &url,VARIANT *&TargetFrameName,VARIANT *&StatusCode,VARIANT_BOOL *&Cancel )
{
	if (m_bShowLoading)
	{
		EndLoading();
	}
}

void WebBrowser::NavigateComplete2( IDispatch *pDisp,VARIANT *&url )
{
	if (!m_sigCustomWindow.empty())
		CheckCustomWnd();

	CComPtr<IDispatch> spDoc;   
	m_pWebBrowser2->get_Document(&spDoc);   

	if (spDoc)
	{   
		CComQIPtr<ICustomDoc, &IID_ICustomDoc> spCustomDoc(spDoc);   
		if (spCustomDoc)   
			spCustomDoc->SetUIHandler(this);   
	}

	if (m_bShowLoading)
	{
		EndLoading();
	}
}

void WebBrowser::CheckCustomWnd()
{
	std::string strContent = GetMetaContent(_T("window-custom"));
	if (strContent.empty())
		return;

	Size szCustom(0, 0);
	uint32_t dwAddStyle, dwRemoveStyle;
	BOOL bHScroll = false, bVScroll = true;
	TCHAR* token;
	TCHAR* next_token;

	dwAddStyle = dwRemoveStyle = 0;
	if (_tcsrchr(strContent.c_str(), _T('&')) != NULL)
	{
		token = _tcstok_s((char *)strContent.c_str(), _T("&"), &next_token);
		while (token)
		{
			TCHAR* val = _tcsrchr(token, _T('='));
			if (val)
			{
				*val = _T('\0');
				++val;

				if (lstrcmpi(token, _T("width")) == 0)
					szCustom.width = _ttoi(val);
				else if (lstrcmpi(token, _T("height")) == 0)
					szCustom.height = _ttoi(val);
				else if (lstrcmpi(token, _T("addStyle")) == 0)
					dwAddStyle = (uint32_t)_ttol(val);
				else if (lstrcmpi(token, _T("removeStyle")) == 0)
					dwRemoveStyle = (uint32_t)_ttol(val);
				else if (lstrcmpi(token, _T("hScroll")) == 0)
					bHScroll = _ttoi(val) != 0;
				else if (lstrcmpi(token, _T("vScroll")) == 0)
					bVScroll = _ttoi(val) != 0;
			}

			token = _tcstok_s(NULL, _T("&"), &next_token);
		}
	}
	else
	{
		token = _tcstok_s((char *)strContent.c_str(), _T(","), &next_token);
		szCustom.width = token ? _ttoi(token) : 0;
		token = _tcstok_s(NULL, _T(","), &next_token);
		szCustom.height = token ? _ttoi(token) : 0;		
	}

	m_sigCustomWindow(szCustom, dwAddStyle, dwRemoveStyle, bHScroll, bVScroll);
}

std::string WebBrowser::GetMetaContent(LPCTSTR lpszName)
{
	std::string strContent = _T("");
	if (lpszName == NULL || lpszName[0] == _T('\0'))
		return strContent;

	HRESULT hr;
	do 
	{
		CComPtr<IDispatch> spDisp;   
		m_pWebBrowser2->get_Document(&spDisp);   

		if (!spDisp) break;
		CComQIPtr<IHTMLDocument2> spDoc = spDisp;
		spDisp = NULL;
		if (!spDoc) break;

		CComPtr<IHTMLElementCollection> spElements;
		hr = spDoc->get_all(&spElements);
		if (FAILED(hr)) break;

		CComQIPtr<IHTMLElementCollection> spMetaElements;
		hr = spElements->tags(CComVariant(_T("meta")), &spDisp);
		if (FAILED(hr)) break;
		spMetaElements = spDisp;
		spDisp = NULL;

		long lIndex = 0;
		long lLength = 0;
		hr = spMetaElements->get_length(&lLength);
		if (FAILED(hr)) break;

		std::string strMetaName;
		CComQIPtr<IHTMLMetaElement> spMetaElement;
		while (lIndex < lLength)
		{
			hr = spMetaElements->item(CComVariant(lIndex++), CComVariant(), &spDisp);
			if (FAILED(hr)) continue;

			spMetaElement = spDisp;
			spDisp = NULL;
			CComBSTR bstrMetaName;
			hr = spMetaElement->get_name(&bstrMetaName);
			if (FAILED(hr) || bstrMetaName == NULL) continue;

			strMetaName = _bstr_t(bstrMetaName.m_str);
			if (strMetaName.compare(lpszName) == 0)
			{
				CComBSTR bstrMetaContent;
				hr = spMetaElement->get_content(&bstrMetaContent);
				if (FAILED(hr) || bstrMetaContent == NULL) continue;

				strContent = _bstr_t(bstrMetaContent.m_str);
				break;
			}
		}
	} while (0);

	return strContent;
}

void WebBrowser::ProgressChange( LONG nProgress, LONG nProgressMax )
{
}

void WebBrowser::NewWindow3( IDispatch **pDisp, VARIANT_BOOL *&Cancel, DWORD dwFlags, BSTR bstrUrlContext, BSTR bstrUrl )
{
}
void WebBrowser::CommandStateChange(long Command,VARIANT_BOOL Enable)
{
}

// IDownloadManager
STDMETHODIMP WebBrowser::Download( /* [in] */ IMoniker *pmk, /* [in] */ IBindCtx *pbc, /* [in] */ DWORD dwBindVerb, /* [in] */ LONG grfBINDF, /* [in] */ BINDINFO *pBindInfo, /* [in] */ LPCOLESTR pszHeaders, /* [in] */ LPCOLESTR pszRedir, /* [in] */ UINT uiCP )
{
	return S_OK;
}

// IDocHostUIHandler
STDMETHODIMP WebBrowser::ShowContextMenu( DWORD dwID, POINT* pptPosition, IUnknown* pCommandTarget, IDispatch* pDispatchObjectHit )
{
	return S_FALSE;
}

STDMETHODIMP WebBrowser::GetHostInfo( DOCHOSTUIINFO* pInfo )
{
	pInfo->cbSize = sizeof(DOCHOSTUIINFO);
	pInfo->dwFlags = DOCHOSTUIFLAG_NO3DBORDER|DOCHOSTUIFLAG_SCROLL_NO;
	pInfo->dwDoubleClick = DOCHOSTUIDBLCLK_DEFAULT;
	return S_OK;
}

STDMETHODIMP WebBrowser::ShowUI( DWORD dwID, IOleInPlaceActiveObject* pActiveObject, IOleCommandTarget* pCommandTarget, IOleInPlaceFrame* pFrame, IOleInPlaceUIWindow* pDoc )
{
	return S_OK;
}

STDMETHODIMP WebBrowser::HideUI()
{
	return S_OK;
}

STDMETHODIMP WebBrowser::UpdateUI()
{
	return S_OK;
}

STDMETHODIMP WebBrowser::EnableModeless( BOOL fEnable )
{
	return E_NOTIMPL;
}

STDMETHODIMP WebBrowser::OnDocWindowActivate( BOOL fActivate )
{
	return E_NOTIMPL;
}

STDMETHODIMP WebBrowser::OnFrameWindowActivate( BOOL fActivate )
{
	return E_NOTIMPL;
}

STDMETHODIMP WebBrowser::ResizeBorder( LPCRECT prcBorder, IOleInPlaceUIWindow* pUIWindow, BOOL fFrameWindow )
{
	return E_NOTIMPL;
}

STDMETHODIMP WebBrowser::TranslateAccelerator( LPMSG lpMsg, const GUID* pguidCmdGroup, DWORD nCmdID )
{
	return S_FALSE;
}
// 
// LRESULT WuiWebBrowser::TranslateAccelerator( MSG *pMsg )
// {
//     if(pMsg->message < WM_KEYFIRST || pMsg->message > WM_KEYLAST)
//         return S_FALSE;
// 
// 	if( m_pWebBrowser2 == NULL )
//         return E_NOTIMPL;
// 
//     // 当前Web窗口不是焦点,不处理加速键
//     BOOL bIsChild = FALSE;
//     HWND hTempWnd = NULL;
//     HWND hWndFocus = ::GetFocus();
// 
//     hTempWnd = hWndFocus;
//     while(hTempWnd != NULL)
//     {
//         if(hTempWnd == m_hwndHost)
//         {
//             bIsChild = TRUE;
//             break;
//         }
//         hTempWnd = ::GetParent(hTempWnd);
//     }
//     if(!bIsChild)
//         return S_FALSE;
// 
// 	IOleInPlaceActiveObject *pObj;
// 	if (FAILED(m_pWebBrowser2->QueryInterface(IID_IOleInPlaceActiveObject, (LPVOID *)&pObj)))
// 		return S_FALSE;
// 
//     HRESULT hResult = pObj->TranslateAccelerator(pMsg);
//     pObj->Release();
//     return hResult;
// }

STDMETHODIMP WebBrowser::GetOptionKeyPath( LPOLESTR* pchKey, DWORD dwReserved )
{
	return E_NOTIMPL;
}

STDMETHODIMP WebBrowser::GetDropTarget( IDropTarget* pDropTarget, IDropTarget** ppDropTarget )
{
	return S_FALSE;	// 使用系统拖拽
}

STDMETHODIMP WebBrowser::GetExternal( IDispatch** ppDispatch )
{
	return S_FALSE;
}

STDMETHODIMP WebBrowser::TranslateUrl( DWORD dwTranslate, OLECHAR* pchURLIn, OLECHAR** ppchURLOut )
{
    *ppchURLOut = 0;
    return E_NOTIMPL;
}

STDMETHODIMP WebBrowser::FilterDataObject( IDataObject* pDO, IDataObject** ppDORet )
{
    *ppDORet = 0;
    return E_NOTIMPL;
}

void WebBrowser::Refresh2( int Level )
{
	CComVariant vLevel(Level);
	m_pWebBrowser2->Refresh2(&vLevel);
}

void WebBrowser::SetAttribute( LPCTSTR pstrName, LPCTSTR pstrValue )
{
	if (_tcscmp(pstrName, _T("homepage")) == 0)
	{
		m_strHomePage = pstrValue;
	}
	else if (_tcscmp(pstrName, _T("autonavi"))==0)
	{
		m_bAutoNavi = (_tcscmp(pstrValue, _T("true")) == 0);
	}
	else
		AxWidget::SetAttribute(pstrName, pstrValue);
}

void WebBrowser::NavigateHomePage()
{
	if (!m_strHomePage.empty())
		this->NavigateUrl(m_strHomePage.c_str());
}

void WebBrowser::NavigateUrl( LPCTSTR lpszUrl )
{
	if (m_pWebBrowser2 && lpszUrl)
	{
		m_pWebBrowser2->Navigate((BSTR)SysAllocString(T2BSTR(lpszUrl)),NULL,NULL,NULL,NULL);
	}
}

void WebBrowser::SetHomePage( LPCTSTR lpszUrl )
{
	m_strHomePage = lpszUrl;
}

LPCTSTR WebBrowser::GetHomePage()
{
	return m_strHomePage.c_str();
}

void WebBrowser::SetAutoNavigation( bool bAuto /*= TRUE*/ )
{
	if (m_bAutoNavi==bAuto)	return;

	m_bAutoNavi=bAuto;
}

bool WebBrowser::IsAutoNavigation()
{
	return m_bAutoNavi;
}

STDMETHODIMP WebBrowser::QueryService( REFGUID guidService, REFIID riid, void** ppvObject )
{
	*ppvObject = NULL;
	return E_NOINTERFACE;
}

HRESULT WebBrowser::RegisterEventHandler( BOOL inAdvise )
{
	if (m_pWebBrowser2 == NULL)
		return E_POINTER;

	CComPtr<IWebBrowser2> pWebBrowser;
	CComPtr<IConnectionPointContainer>  pCPC;
	CComPtr<IConnectionPoint> pCP;
	HRESULT hr = m_pWebBrowser2->QueryInterface(IID_IConnectionPointContainer,(void **)&pCPC);
	if (FAILED(hr))
		return hr;
	hr=pCPC->FindConnectionPoint(DIID_DWebBrowserEvents2,&pCP);
	if (FAILED(hr))
		return hr;

	if (inAdvise)
	{
		hr = pCP->Advise((IDispatch*)this, &m_dwCookie);
	}
	else
	{
		hr = pCP->Unadvise(m_dwCookie);
	}
	return hr; 
}

DISPID WebBrowser::FindId( IDispatch *pObj, LPOLESTR pName )
{
	DISPID id = 0;
	if ( FAILED(pObj->GetIDsOfNames(IID_NULL,&pName,1,LOCALE_SYSTEM_DEFAULT,&id)) ) 
		id = -1;
	return id;
}

HRESULT WebBrowser::InvokeMethod( IDispatch *pObj, LPOLESTR pMehtod, VARIANT *pVarResult, VARIANT *ps, int cArgs )
{
	DISPID dispid = FindId(pObj, pMehtod);
	if (dispid == -1)
		return E_FAIL;

	DISPPARAMS dispparams;
	dispparams.cArgs = cArgs;
	dispparams.rgvarg = ps;
	dispparams.cNamedArgs = 0;
	dispparams.rgdispidNamedArgs = NULL;

	return pObj->Invoke(dispid, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_METHOD, &dispparams, pVarResult, NULL, NULL);
}

HRESULT WebBrowser::GetProperty( IDispatch *pObj, LPOLESTR pName, VARIANT *pValue )
{
	DISPID dispid = FindId(pObj, pName);
	if (dispid == -1) 
		return E_FAIL;

	DISPPARAMS ps;
	ps.cArgs = 0;
	ps.rgvarg = NULL;
	ps.cNamedArgs = 0;
	ps.rgdispidNamedArgs = NULL;

	return pObj->Invoke(dispid, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_PROPERTYGET, &ps, pValue, NULL, NULL);
}

HRESULT WebBrowser::SetProperty( IDispatch *pObj, LPOLESTR pName, VARIANT *pValue )
{
	DISPID dispid = FindId(pObj, pName);
	if(dispid == -1) return E_FAIL;

	DISPPARAMS ps;
	ps.cArgs = 1;
	ps.rgvarg = pValue;
	ps.cNamedArgs = 0;
	ps.rgdispidNamedArgs = NULL;

	return pObj->Invoke(dispid, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_PROPERTYPUT, &ps, NULL, NULL, NULL);
}

IDispatch* WebBrowser::GetHtmlWindow()
{
	IDispatch* pDp =  NULL;
	HRESULT hr;
	if (m_pWebBrowser2)
		hr=m_pWebBrowser2->get_Document(&pDp);

	if (FAILED(hr))
		return NULL;

    CComQIPtr<IHTMLDocument2> pHtmlDoc2 = pDp;

	if (pHtmlDoc2 == NULL)
		return NULL;

	hr=pHtmlDoc2->get_parentWindow(&_pHtmlWnd2);

	if (FAILED(hr))
		return NULL;

	IDispatch *pHtmlWindown = NULL;
	hr=_pHtmlWnd2->QueryInterface(IID_IDispatch, (void**)&pHtmlWindown);
	if (FAILED(hr))
		return NULL;

	return pHtmlWindown;
}

IWebBrowser2* WebBrowser::GetWebBrowser2( void )
{
	return m_pWebBrowser2;
}

HRESULT STDMETHODCALLTYPE WebBrowser::QueryStatus( __RPC__in_opt const GUID *pguidCmdGroup, ULONG cCmds, __RPC__inout_ecount_full(cCmds ) OLECMD prgCmds[ ], __RPC__inout_opt OLECMDTEXT *pCmdText )
{
	HRESULT hr = OLECMDERR_E_NOTSUPPORTED;
	return hr;
}

HRESULT STDMETHODCALLTYPE WebBrowser::Exec( __RPC__in_opt const GUID *pguidCmdGroup, DWORD nCmdID, DWORD nCmdexecopt, __RPC__in_opt VARIANT *pvaIn, __RPC__inout_opt VARIANT *pvaOut )
{
	HRESULT hr = S_OK;

	if (pguidCmdGroup && IsEqualGUID(*pguidCmdGroup, CGID_DocHostCommandHandler))
	{

		switch (nCmdID) 
		{

		case OLECMDID_SHOWSCRIPTERROR:
			{
				IHTMLDocument2*             pDoc = NULL;
				IHTMLWindow2*               pWindow = NULL;
				IHTMLEventObj*              pEventObj = NULL;
				BSTR                        rgwszNames[5] = 
				{ 
					SysAllocString(L"errorLine"),
					SysAllocString(L"errorCharacter"),
					SysAllocString(L"errorCode"),
					SysAllocString(L"errorMessage"),
					SysAllocString(L"errorUrl")
				};
				DISPID                      rgDispIDs[5];
				VARIANT                     rgvaEventInfo[5];
				DISPPARAMS                  params;
				BOOL                        fContinueRunningScripts = true;
				int                         i;

				params.cArgs = 0;
				params.cNamedArgs = 0;

				// Get the document that is currently being viewed.
				hr = pvaIn->punkVal->QueryInterface(IID_IHTMLDocument2, (void **) &pDoc);    
				// Get document.parentWindow.
				hr = pDoc->get_parentWindow(&pWindow);
				pDoc->Release();
				// Get the window.event object.
				hr = pWindow->get_event(&pEventObj);
				// Get the error info from the window.event object.
				for (i = 0; i < 5; i++) 
				{  
					// Get the property's dispID.
					hr = pEventObj->GetIDsOfNames(IID_NULL, &rgwszNames[i], 1, 
						LOCALE_SYSTEM_DEFAULT, &rgDispIDs[i]);
					// Get the value of the property.
					hr = pEventObj->Invoke(rgDispIDs[i], IID_NULL,
						LOCALE_SYSTEM_DEFAULT,
						DISPATCH_PROPERTYGET, &params, &rgvaEventInfo[i],
						NULL, NULL);
					SysFreeString(rgwszNames[i]);
				}

				// At this point, you would normally alert the user with 
				// the information about the error, which is now contained
				// in rgvaEventInfo[]. Or, you could just exit silently.

				(*pvaOut).vt = VT_BOOL;
				if (fContinueRunningScripts)
				{
					// Continue running scripts on the page.
					(*pvaOut).boolVal = VARIANT_TRUE;
				}
				else
				{
					// Stop running scripts on the page.
					(*pvaOut).boolVal = VARIANT_FALSE;   
				} 
				break;
			}
		default:
			hr = OLECMDERR_E_NOTSUPPORTED;
			break;
		}
	}
	else
	{
		hr = OLECMDERR_E_UNKNOWNGROUP;
	}
	return (hr);
}

void WebBrowser::SetBkColor(COLORREF clr)
{
	m_clrBk = clr;
}

void WebBrowser::BeginLoading()
{
	if (m_bLoading)
		return;

	HWND hWnd = GetHostWnd();
	::ShowWindow(hWnd, SW_HIDE);

	m_lFrame = 0;
	DWORD dwDelay = m_pLoadingImage->GetFrameDelay() * 10;
	if (dwDelay == 0)
		dwDelay = 150;
	m_Timer.Start(dwDelay, true);
	m_bLoading = TRUE;
}

void WebBrowser::EndLoading()
{
	if (!m_bLoading)
		return;

	HWND hWnd = GetHostWnd();
	::ShowWindow(hWnd, SW_SHOW);
	m_Timer.Stop();

	m_bLoading = FALSE;
	m_bShowLoading = FALSE;
}

void WebBrowser::SetLoadingImage(CxImage* pImage)
{
	m_pLoadingImage = pImage;
}

void WebBrowser::SetShowLoading(BOOL bShow)
{
	if (m_bShowLoading == bShow)
		return;

	m_bShowLoading = bShow;	
	if (m_bLoading != m_bShowLoading)
	{
		if (m_bShowLoading)
			BeginLoading();
		else
			EndLoading();
	}
}

void WebBrowser::OnTimer()
{
	if (m_pLoadingImage && m_pLoadingImage->GetNumFrames() != 0)
	{
		m_lFrame = (m_lFrame + 1) % m_pLoadingImage->GetNumFrames();
		RequestRepaint();
	}
}


}// namespace wui