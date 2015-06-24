#include "StdAfx.h"
#include "TCHAR.h"
#include "WidgetHostWindow.h"
#include "KeyboardDevice.h"
#include "MouseDevice.h"
#include "Keys.h"
#include "Geometry.h"
#include "MessageQueue.h"

namespace wui{

WidgetHostWindow::WidgetHostWindow(void)
{
	_Init();
}

WidgetHostWindow::WidgetHostWindow(const std::string &strTitle, int nWidth, int nHeight, bool bAllowResize /*= false*/)
{
	_Init();
	
	WindowDescription desc;
	desc.SetTitle(strTitle);
	desc.SetSize(Size(nWidth, nHeight), false);
	desc.SetAllowResize(bAllowResize);
	Create(desc);
}

WidgetHostWindow::WidgetHostWindow(const WindowDescription &desc)
{
	_Init();
	
	Create(desc);
}

WidgetHostWindow::~WidgetHostWindow(void)
{
	MessageQueue::m_messageQueue.RemoveClient(this);
	
	if (m_keyboard)
		delete m_keyboard;
	if (m_mouse)
		delete m_mouse;
	
	if (m_bDestroyHwnd && m_hwnd)
		DestroyWindow(m_hwnd);
	if (m_hLargeIcon)
		DestroyIcon(m_hLargeIcon);
	if (m_hSmallIcon)
		DestroyIcon(m_hSmallIcon);
	if (m_graphic)
		delete m_graphic;
		
	SetWindowLongPtr(m_hwnd, GWLP_USERDATA, (LONG_PTR)NULL);
	if (m_pfnOld)
	{
		SetWindowLongPtr(m_hwnd, GWLP_WNDPROC, (LONG_PTR)m_pfnOld);
	}
	
}

void WidgetHostWindow::_Init()
{
	m_hwnd = NULL;
	m_bDestroyHwnd = true;
	m_hCurrCursor = NULL;
	m_hLargeIcon = m_hSmallIcon = NULL;
	m_bCursorSet = m_bCursorHidden = false;
	m_minSize.SetSize(0, 0);
	m_maxSize.SetSize(0xFFFF, 0xFFFF);
	m_minimized = m_maximized = false;
	m_keyboard = new KeyboardDevice(this);
	m_mouse = new MouseDevice(this);
	m_graphic = NULL;
	m_hPaintDC = NULL;
	memset(&m_paintstruct, 0, sizeof(m_paintstruct));
	m_pfnOld = NULL;
	m_ptMouse.x = m_ptMouse.y = 0;
	
	MessageQueue::m_messageQueue.AddClient(this);
}

void WidgetHostWindow::Create(const WindowDescription &desc)
{
	if (m_hwnd)
		ModifyWindow(desc);
	else
		CreateNewWindow(desc);
}

void WidgetHostWindow::ModifyWindow(const WindowDescription &desc)
{
	DWORD dwStyle = 0, dwExStyle = 0;
	GetStylesFromDescription(desc, dwStyle, dwExStyle);
	RECT rcWindow = GetWindowGeometryFromDescription(desc, dwStyle, dwExStyle);
	
	SetWindowText(m_hwnd, (LPCTSTR)desc.GetTitle().c_str());
	SetWindowLong(m_hwnd, GWL_STYLE, dwStyle);
	SetWindowLong(m_hwnd, GWL_EXSTYLE, dwExStyle);
	
	SetWindowPos(m_hwnd, desc.IsTopmost() ? HWND_TOPMOST : HWND_NOTOPMOST, 
		rcWindow.left, rcWindow.top, 
		rcWindow.right - rcWindow.left, 
		rcWindow.bottom - rcWindow.top, 
		SWP_NOACTIVATE | SWP_FRAMECHANGED);
		
	ShowWindow(m_hwnd, desc.IsVisible() ? SW_SHOW : SW_HIDE);
	RedrawWindow(0, 0, 0, RDW_ALLCHILDREN | RDW_INVALIDATE);
	
	m_minimized = IsMinimized();
	m_maximized = IsMaximized();
}

void WidgetHostWindow::CreateNewWindow(const WindowDescription &desc)
{
	if (desc.GetHandle())
	{
		m_hwnd = desc.GetHandle();
		m_bDestroyHwnd = false;
		SetWindowLongPtr(m_hwnd, GWLP_USERDATA, (LONG_PTR)this);
		m_pfnOld = (WNDPROC)SetWindowLongPtr(m_hwnd, GWLP_WNDPROC, (LONG_PTR)WidgetHostWindow::static_window_proc);
	}
	else
	{
		RegisterWindowClass();
		
		DWORD dwStyle = 0, dwExStyle = 0;
		GetStylesFromDescription(desc, dwStyle, dwExStyle);
		RECT rcWindow = GetWindowGeometryFromDescription(desc, dwStyle, dwExStyle);
		
		HWND hParent = 0;
		if (desc.GetOwner())
		{
			hParent = desc.GetOwner()->GetHwnd();
		}
		else
			hParent = desc.GetOwnerHandle();
		
		m_hwnd = CreateWindowEx(
			dwExStyle, 
			_T("WidgetUI_Host_Window"), 
			(LPCTSTR)desc.GetTitle().c_str(), 
			dwStyle, 
			rcWindow.left, 
			rcWindow.top, 
			rcWindow.right - rcWindow.left, 
			rcWindow.bottom - rcWindow.top, 
			hParent, 
			0, 
			(HINSTANCE)GetModuleHandle(0), 
			this);
			
		if (m_hwnd == 0)
			return;
		
		if (desc.IsTopmost())
			SetWindowPos(m_hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOACTIVATE|SWP_NOMOVE|SWP_NOSIZE);
		
		if (desc.IsVisible())
			ShowWindow(m_hwnd, SW_SHOW);
		
		m_minimized = IsMinimized();
		m_maximized = IsMaximized();
	}
	
	m_ic.AddKeyboard(m_keyboard);
	m_ic.AddMouse(m_mouse);
	if (m_graphic == NULL)
		m_graphic = new XGraphics(Size(GetViewPort().width,GetViewPort().height));
	else
		m_graphic->Resize(Size(GetViewPort().width,GetViewPort().height));
}

void WidgetHostWindow::RegisterWindowClass()
{
	static bool firstCall = true;
	if (firstCall)
	{
		WNDCLASS wndclass;
		memset(&wndclass, 0, sizeof(wndclass));
		wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
		wndclass.lpfnWndProc = (WNDPROC)static_window_proc;
		wndclass.cbClsExtra = 0;
		wndclass.cbWndExtra = 0;
		wndclass.hInstance = (HINSTANCE)GetModuleHandle(0);
		wndclass.hIcon = 0;
		wndclass.hCursor = 0;
		wndclass.hbrBackground = CreateSolidBrush(RGB(255,255,255));
		wndclass.lpszMenuName = 0;
		wndclass.lpszClassName = _T("WidgetUI_Host_Window");
		RegisterClass(&wndclass);
		
		firstCall = false;
	}
}

Rect WidgetHostWindow::GetGeometry() const
{
	RECT rc;
	GetWindowRect(m_hwnd, &rc);
	Rect rect(rc.left, rc.top, rc.right- rc.left, rc.bottom- rc.top);
	return rect;
}

Rect WidgetHostWindow::GetViewPort() const
{
	RECT rc;
	GetClientRect(m_hwnd, &rc);
	Rect rect(rc.left, rc.top, rc.right- rc.left, rc.bottom- rc.top);
	return rect;
}

bool WidgetHostWindow::HasFocus() const
{
	return (GetFocus() == m_hwnd);
}

bool WidgetHostWindow::IsVisible() const
{
	return IsWindowVisible(m_hwnd) != 0;
}

bool WidgetHostWindow::IsMinimized() const
{
	return IsIconic(m_hwnd) != 0;
}

bool WidgetHostWindow::IsMaximized() const
{
	return IsZoomed(m_hwnd) != 0;
}

HWND WidgetHostWindow::GetHwnd() const
{
	return m_hwnd;
}

Point WidgetHostWindow::ClientToScreen(const Point &client)
{
	Point point(client);
	POINT pt = point.GetStructPoint();
	::ClientToScreen(m_hwnd, &pt);
	return Point(pt.x, pt.y);
}

Point WidgetHostWindow::ScreenToClient(const Point &screen)
{
	Point point(screen);
	POINT pt = point.GetStructPoint();
	::ScreenToClient(m_hwnd, &pt);
	return point;
}

void WidgetHostWindow::CaptureMouse(bool capture)
{
	if (capture)
		SetCapture(m_hwnd);
	else
		ReleaseCapture();
}

void WidgetHostWindow::RequestRepaint(const Rect &rect)
{
	RECT rc;
	rc.top = rect.y;
	rc.left = rect.x;
	rc.right = rect.x + rect.width;
	rc.bottom = rect.y + rect.height;
	InvalidateRect(m_hwnd, &rc, false);
}

void WidgetHostWindow::SetTitle(const std::string &strTitle)
{
	SetWindowText(m_hwnd, (LPCTSTR)strTitle.c_str());
}

void WidgetHostWindow::SetPosition(const Rect &pos, bool bClientArea)
{
	if (bClientArea)
	{
		Rect rect(pos);
		RECT rc = rect.GetRectStruct();
		AdjustWindowRectEx(
			&rc,
			GetWindowLongPtr(m_hwnd, GWL_STYLE),
			FALSE,
			GetWindowLongPtr(m_hwnd, GWL_EXSTYLE));

		SetWindowPos(m_hwnd, 0, pos.x, pos.y, pos.GetRight()-pos.x, pos.GetBottom()-pos.y, SWP_NOACTIVATE|SWP_NOREPOSITION|SWP_NOZORDER);
	}
	else
	{
		SetWindowPos(m_hwnd, 0, pos.x, pos.y, pos.width, pos.height, SWP_NOACTIVATE|SWP_NOREPOSITION|SWP_NOZORDER);
	}
}

void WidgetHostWindow::SetEnabled(bool enable)
{
	EnableWindow(m_hwnd, enable ? TRUE : FALSE);
}

void WidgetHostWindow::SetVisible(bool visible, bool activate)
{
	if (visible)
		Show(activate);
	else
		Hide();
}

void WidgetHostWindow::SetSize(int width, int height, bool bClientArea)
{
	if (bClientArea)
	{
		RECT rect;
		rect.left = 0;
		rect.top = 0;
		rect.right = width;
		rect.bottom = height;

		AdjustWindowRectEx(
			&rect,
			GetWindowLongPtr(m_hwnd, GWL_STYLE),
			FALSE,
			GetWindowLongPtr(m_hwnd, GWL_EXSTYLE));

		SetWindowPos(m_hwnd, 0, 0, 0, rect.right - rect.left, rect.bottom - rect.top, SWP_NOACTIVATE|SWP_NOMOVE|SWP_NOREPOSITION|SWP_NOZORDER);
	}
	else
	{
		SetWindowPos(m_hwnd, 0, 0, 0, width, height, SWP_NOACTIVATE|SWP_NOMOVE|SWP_NOREPOSITION|SWP_NOZORDER);
	}
}

void WidgetHostWindow::SetMinimumSize(int width, int height)
{
	m_minSize = Size(width,height);
}

void WidgetHostWindow::SetMaximumSize(int width, int height)
{
	m_maxSize= Size(width,height);
}

void WidgetHostWindow::Minimize()
{
	ShowWindow(m_hwnd, SW_MINIMIZE);
}

void WidgetHostWindow::Restore()
{
	ShowWindow(m_hwnd, SW_RESTORE);
}

void WidgetHostWindow::Maximize()
{
	ShowWindow(m_hwnd, SW_MAXIMIZE);
}

void WidgetHostWindow::Show(bool activate /*= true*/)
{
	ShowWindow(m_hwnd, activate ? SW_SHOW : SW_SHOWNA);
}

void WidgetHostWindow::Hide()
{
	ShowWindow(m_hwnd, SW_HIDE);
}

void WidgetHostWindow::BringToFront()
{
	BringWindowToTop(m_hwnd);
}

void WidgetHostWindow::ShowCursor()
{
	::ShowCursor(TRUE);
	m_bCursorHidden = false;
}

void WidgetHostWindow::SetCursor(const HCURSOR hCursor)
{
	::SetCursor(hCursor);
	m_hCurrCursor = hCursor;
	m_bCursorSet = true;
}

void WidgetHostWindow::HideCursor()
{
	::ShowCursor(FALSE);
	m_bCursorHidden = true;
}

void WidgetHostWindow::SetLargeIcon(HICON hIcon)
{
	if (m_hLargeIcon)
		DestroyIcon(m_hLargeIcon);
	m_hLargeIcon = 0;
	m_hLargeIcon = hIcon;

	SendMessage(m_hwnd, WM_SETICON, ICON_BIG, reinterpret_cast<LPARAM>(m_hLargeIcon));
}

void WidgetHostWindow::SetSmallIcon(HICON hIcon)
{
	if (m_hSmallIcon)
		DestroyIcon(m_hSmallIcon);
	m_hSmallIcon = 0;
	m_hSmallIcon = hIcon;

	SendMessage(m_hwnd, WM_SETICON, ICON_SMALL, reinterpret_cast<LPARAM>(m_hSmallIcon));
}

boost::signal<void ()> & WidgetHostWindow::sigLostFocus()
{
	return m_sigLostFocus;
}

boost::signal<void ()> & WidgetHostWindow::sigGotFocus()
{
	return m_sigGotFocus;
}

boost::signal<void (int, int)> & WidgetHostWindow::sigResize()
{
	return m_sigResize;
}

boost::signal<void (const Rect &)> & WidgetHostWindow::sigPaint()
{
	return m_sigPaint;
}

boost::signal<void ()> & WidgetHostWindow::sigWindowClose()
{
	return m_sigWindowClose;
}

boost::signal<void ()> & WidgetHostWindow::sigWindowMinimized()
{
	return m_sigWindowMinimized;
}

boost::signal<void ()> & WidgetHostWindow::sigWindowMaximized()
{
	return m_sigWindowMaximized;
}

LRESULT WINAPI WidgetHostWindow::static_window_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	WidgetHostWindow *self = 0;
	if (uMsg == WM_CREATE)
	{
		LPCREATESTRUCT lpcs = (LPCREATESTRUCT) lParam;
		self = (WidgetHostWindow *) lpcs->lpCreateParams;
		SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR) self);
	}
	else
	{
		self = (WidgetHostWindow *) GetWindowLongPtr(hWnd, GWLP_USERDATA);
	}

	if (self)
		return self->WindowProc(hWnd, uMsg, wParam, lParam);
	else
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

LRESULT WidgetHostWindow::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT result;
	bool bHandled = false;
	Rect rcUpdate(0,0,0,0);
	RECT rc = rcUpdate.GetRectStruct();

	if (uMsg == WM_PAINT)
	{

		GetUpdateRect(hWnd, &rc, TRUE);
		rcUpdate.x = rc.left;
		rcUpdate.y = rc.top;
		rcUpdate.width = rc.right - rc.left;
		rcUpdate.height = rc.bottom - rc.top;
		
		if (m_pfnOld)
		{
			result = ::CallWindowProc(m_pfnOld, hWnd, uMsg, wParam, lParam);
			bHandled = TRUE;
		}
	}
	
	switch (uMsg)
	{
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYUP:
		ReceivedKeyboardInput(uMsg, wParam, lParam);
		result = 0;
		bHandled = TRUE;
		break;
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_LBUTTONDBLCLK:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_RBUTTONDBLCLK:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_MBUTTONDBLCLK:
	case WM_MOUSEWHEEL:
	case WM_XBUTTONDOWN:
	case WM_XBUTTONUP:
	case WM_XBUTTONDBLCLK:
		ReceivedMouseInput(uMsg, wParam, lParam);
		result = 0;
		bHandled = TRUE;
		break;

	case WM_MOUSEMOVE:
		if (m_ptMouse.x == 0 && m_ptMouse.y == 0)
		{
			TRACKMOUSEEVENT tme;
			tme.cbSize = sizeof(TRACKMOUSEEVENT);         
			tme.dwFlags = TME_LEAVE;
			tme.dwHoverTime = HOVER_DEFAULT;
			tme.hwndTrack = hWnd;
			TrackMouseEvent(&tme);
		}
	
		ReceivedMouseMove(uMsg, wParam, lParam);
		result = 0;
		bHandled = TRUE;
		break;
		
	case WM_MOUSELEAVE:
		ReceivedMouseMove(WM_MOUSEMOVE, 0, 0);
		result = 0;
		bHandled = TRUE;
		break;

	case WM_SIZING:
		{

			WINDOWINFO wi;
			wi.cbSize = sizeof(WINDOWINFO);
			GetWindowInfo(hWnd, &wi);

			TITLEBARINFO ti;
			ti.cbSize = sizeof(TITLEBARINFO);
			GetTitleBarInfo(hWnd, &ti);

			RECT *rect = (RECT*)lParam;

			// enforce minimum size
			if (rect->right - rect->left < m_minSize.width)
				rect->right = rect->left + m_minSize.width;
			if (rect->bottom - rect->top < m_minSize.height)
				rect->bottom = rect->top + m_minSize.height;

			// enforce max size
			if (rect->right - rect->left > m_maxSize.width)
				rect->right = rect->left + m_maxSize.width;
			if (rect->bottom - rect->top > m_maxSize.height)
				rect->bottom = rect->top + m_maxSize.height;

			Rect rcClient(
				rect->left+wi.cxWindowBorders,
				rect->top+wi.cyWindowBorders+(ti.rcTitleBar.bottom-ti.rcTitleBar.top),
				rect->right-wi.cxWindowBorders,
				rect->bottom-wi.cyWindowBorders);

			rect->left = rcClient.x - wi.cxWindowBorders;
			rect->right = rcClient.GetRight() + wi.cxWindowBorders;
			rect->top = rcClient.y - wi.cyWindowBorders - (ti.rcTitleBar.bottom-ti.rcTitleBar.top);
			rect->bottom = rcClient.GetBottom() + wi.cyWindowBorders;
			
			result = TRUE;
			bHandled = TRUE;
			break;
		}

	case WM_SIZE:
		if (m_graphic)
			m_graphic->Resize(Size(LOWORD(lParam), HIWORD(lParam)));
		
		switch(wParam)
		{
			// Message is sent to all pop-up windows when some other window is maximized.
		case SIZE_MAXHIDE:
			break;

			// Message is sent to all pop-up windows when some other window has been restored to its former size.
		case SIZE_MAXSHOW:
			break;

			// The window has been maximized.
		case SIZE_MAXIMIZED:
			m_sigWindowMaximized();
			break;

			// The window has been minimized.
		case SIZE_MINIMIZED:
			m_sigWindowMinimized();
			break;

			// The window has been resized, but neither the SIZE_MINIMIZED nor SIZE_MAXIMIZED value applies.
		case SIZE_RESTORED:
			break;
		}

		m_sigResize(LOWORD(lParam), HIWORD(lParam));

		result = 0;
		bHandled = TRUE;
		break;
	
	case WM_ACTIVATE:
		if (LOWORD(wParam) == WA_INACTIVE)
		{
			m_sigLostFocus();
		}
		else
		{
			m_sigGotFocus();
		}
		result = 0;
		bHandled = TRUE;
		break;

	case WM_CLOSE:
		m_sigWindowClose();
		result = 0;
		bHandled = TRUE;
		break;
		
	case WM_DESTROY:
		m_sigWindowDestroy();
		result = 0;
		bHandled = TRUE;
		break;
	
	case WM_PAINT:
		{
			memset(&m_paintstruct, 0, sizeof(PAINTSTRUCT));
			m_hPaintDC = BeginPaint(hWnd, &m_paintstruct);

			if (!rcUpdate.RectIsEmpty())
			{
				Rect rect = Rect(m_paintstruct.rcPaint.left, m_paintstruct.rcPaint.top,
					m_paintstruct.rcPaint.right - m_paintstruct.rcPaint.left ,m_paintstruct.rcPaint.bottom - m_paintstruct.rcPaint.top);
				if (rect.RectIsEmpty())
					rect = rcUpdate;
				m_sigPaint(rect);
			}

			EndPaint(hWnd, &m_paintstruct);
			memset(&m_paintstruct, 0, sizeof(PAINTSTRUCT));
			m_hPaintDC = NULL;
		}
		result = 0;
		bHandled = TRUE;
		break;
		
	case WM_ERASEBKGND:
		result = TRUE;
		bHandled = TRUE;
		break;

	case WM_GETICON:
		if (wParam == ICON_BIG && m_hLargeIcon)
			result = reinterpret_cast<LRESULT>(m_hLargeIcon);
		else if ((wParam == ICON_SMALL || wParam == ICON_SMALL2) && m_hSmallIcon)
			result = reinterpret_cast<LRESULT>(m_hSmallIcon);
		break;
	}

	if (m_pfnOld && uMsg != WM_PAINT)
	{
		result = ::CallWindowProc(m_pfnOld, hWnd, uMsg, wParam, lParam);
		bHandled = TRUE;
	}
	
	// Do default window processing if our message handler didn't handle it:
	if (!bHandled)
	{
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	
	return result;
}

void WidgetHostWindow::ReceivedKeyboardInput(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// Is message a down or up event?
	bool keydown;
	switch (uMsg)
	{
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
		keydown = true;
		break;

	case WM_KEYUP:
	case WM_SYSKEYUP:
		keydown = false;
		break;

	default:
		return;
	}

	// Check for Alt+F4 and translate it into a WM_CLOSE event.
	if (m_bDestroyHwnd && wParam == VK_F4 && (uMsg == WM_SYSKEYDOWN || uMsg == WM_SYSKEYUP))
	{
		if ((lParam & (1 << 29)) && uMsg == WM_SYSKEYDOWN) // context code. Bit 29 is set if ALT key is down.
			SendMessage(GetHwnd(), WM_CLOSE, 0, 0);
		return;
	}

	int scancode = (lParam & 0xff0000) >> 16;
	//	bool extended_key = (lparam & 0x1000000) != 0;

	// Update the ctrl/alt/shift hints:
	int nKeyId = wParam;

	// Add to repeat count
	if(keydown)
	{
		if( m_repeatCount.find(nKeyId) == m_repeatCount.end() )
			m_repeatCount[nKeyId] = 0;
		else
			m_repeatCount[nKeyId]++;
	}

	// Prepare event to be emitted:
	InputEvent key;
	if (keydown)
		key.m_type = InputEvent::PRESSED;
	else
		key.m_type = InputEvent::RELEASED;
	key.m_ptMouse = m_ptMouse;
	key.m_id = nKeyId;
	key.m_nRepeatCount = m_repeatCount[nKeyId];

	if (!keydown)
	{
		m_repeatCount[nKeyId] = -1;
	}

	if (keydown)
	{
		unsigned char keys_down[256];
		GetKeyboardState(keys_down);

		// Figure out what character sequence this maps to:
		WCHAR buf[16];
		int result = ToUnicode(
			(UINT) nKeyId,
			scancode,
			keys_down,
			buf,
			16,
			0);
		if (result > 0)
		{
			int nLen = WideCharToMultiByte(CP_ACP, 0, buf, -1, NULL, 0, NULL, NULL);
			char* pszDst = new char[nLen];
			WideCharToMultiByte(CP_ACP, 0, buf, -1, pszDst, nLen, NULL, NULL);
			pszDst[nLen -1] = 0;
			key.m_str = pszDst;

		//	key.m_str = buf;
		}
	}

	SetModifierKeys(key);

	// Emit message:
	m_keyboard->OnEvent(key);
}

void WidgetHostWindow::SetModifierKeys(InputEvent &key)
{
	key.m_alt = (GetKeyState(VK_MENU) & 0xfe) != 0;
	key.m_shift = (GetKeyState(VK_SHIFT) & 0xfe) != 0;
	key.m_ctrl = (GetKeyState(VK_CONTROL) & 0xfe) != 0;
}

void WidgetHostWindow::ReceivedMouseInput(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// Map windows events to something more interesting:
	int id;
	bool up = false;
	bool down = false;
	bool dblclk = false;

	switch (uMsg)
	{
	case WM_LBUTTONDOWN: id = WUI_MOUSE_LEFT; down = true; break;
	case WM_LBUTTONUP: id = WUI_MOUSE_LEFT; up = true; break;
	case WM_LBUTTONDBLCLK: id = WUI_MOUSE_LEFT; dblclk = true; break;
	case WM_RBUTTONDOWN: id = WUI_MOUSE_RIGHT; down = true; break;
	case WM_RBUTTONUP: id = WUI_MOUSE_RIGHT; up = true; break;
	case WM_RBUTTONDBLCLK: id = WUI_MOUSE_RIGHT; dblclk = true; break;
	case WM_MBUTTONDOWN: id = WUI_MOUSE_MIDDLE; down = true; break;
	case WM_MBUTTONUP: id = WUI_MOUSE_MIDDLE; up = true; break;
	case WM_MBUTTONDBLCLK: id = WUI_MOUSE_MIDDLE; dblclk = true; break;
	case WM_MOUSEWHEEL: id = ((short)HIWORD(wParam) > 0) ? WUI_MOUSE_WHEEL_UP : WUI_MOUSE_WHEEL_DOWN; up = true; down = true; break;
	case WM_XBUTTONDOWN: id = WUI_MOUSE_XBUTTON1 + HIWORD(wParam) - 1; down = true; break;
	case WM_XBUTTONUP: id = WUI_MOUSE_XBUTTON1 + HIWORD(wParam) - 1; up = true; break;
	case WM_XBUTTONDBLCLK: id = WUI_MOUSE_XBUTTON1 + HIWORD(wParam) - 1; dblclk = true; break;
	default:
		return;
	}
	// Prepare event to be emitted:
	InputEvent key;
	key.m_ptMouse = m_ptMouse;
	key.m_id = id;
	SetModifierKeys(key);

	if (dblclk)
	{
		key.m_type = InputEvent::DOUBLECLICK;

		// Emit message:
		if (id >= 0 && id < 32)
			m_mouse->SetKeyState(id, true);

		m_mouse->OnEvent(key);
	}

	if (down)
	{
		key.m_type = InputEvent::PRESSED;

		// Emit message:
		if (id >= 0 && id < 32)
			m_mouse->SetKeyState(id, true);

		m_mouse->OnEvent(key);
	}

	// It is possible for 2 events to be called when the wheelmouse is used
	if (up)
	{
		key.m_type = InputEvent::RELEASED;

		// Emit message:
		if (id >= 0 && id < 32)
			m_mouse->SetKeyState(id, true);

		m_mouse->OnEvent(key);
	}
}

void WidgetHostWindow::ReceivedMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	m_bCursorSet = false;

	// Fetch coordinates
	short x = LOWORD(lParam);
	short y = HIWORD(lParam);

	if (m_ptMouse.x != x || m_ptMouse.y != y)
	{
		m_ptMouse.x = x;
		m_ptMouse.y = y;

		// Prepare event to be emitted:
		InputEvent key;
		key.m_type = InputEvent::POINTER_MOVED;
		key.m_ptMouse = m_ptMouse;
		SetModifierKeys(key);

		// Fire off signal
		m_mouse->OnEvent(key);
	}

	if (!m_bCursorSet && !m_bCursorHidden)
	{
		if (m_hCurrCursor == 0)
			SetCursor(LoadCursor(0, IDC_ARROW));
		else
			SetCursor(m_hCurrCursor);
	}
}

InputContext& WidgetHostWindow::GetInputContext()
{
	return m_ic;
}

XGraphics& WidgetHostWindow::GetGraphic()
{
	return *m_graphic;
}

void WidgetHostWindow::GetStylesFromDescription(const WindowDescription &desc, DWORD& dwStyle, DWORD& dwExStyle)
{
	dwStyle = WS_CLIPCHILDREN;
	dwExStyle = 0;

	if (desc.IsChildWindow())
		dwStyle |= WS_CHILD;
	else if (!desc.GetDecorations() || !desc.HasCaption())
		dwStyle |= WS_POPUP;

	if (desc.GetAllowResize())
		dwStyle |= WS_SIZEBOX;

	if (!desc.IsChildWindow() && desc.HasCaption() && desc.GetDecorations())
	{
		dwStyle |= WS_CAPTION;
		if (desc.HasSysmenu())
			dwStyle |= WS_SYSMENU;
		if (desc.HasMinimizeButton())
			dwStyle |= WS_MINIMIZEBOX;
		if (desc.HasMaximizeButton())
			dwStyle |= WS_MAXIMIZEBOX;
	}
	
	if (desc.IsVisible())
		dwStyle |= WS_VISIBLE;

	if (desc.IsToolWindow())
		dwExStyle |= WS_EX_TOOLWINDOW;
}

RECT WidgetHostWindow::GetWindowGeometryFromDescription(const WindowDescription &desc, DWORD dwStyle, DWORD dwExStyle)
{
	int x = desc.GetPosition().GetLeft();
	int y = desc.GetPosition().GetTop();
	int width = desc.GetSize().width;
	int height = desc.GetSize().height;

	bool clientSize = desc.GetPositionClientArea();	// false = Size includes the window frame. true = Size is the drawable size.

	if (desc.GetPosition().GetLeft() == -1 && desc.GetPosition().GetTop() == -1)
	{
		int nScreenWidth = GetSystemMetrics(SM_CXSCREEN);
		int nScreenHeight = GetSystemMetrics(SM_CYSCREEN);

		x = nScreenWidth/2 - width/2;
		y = nScreenHeight/2 - height/2;
	}

	// get size of window with decorations to pass to CreateWindow
	RECT rcWindow = { x, y, x+width, y+height };
	if (clientSize)
		AdjustWindowRectEx(&rcWindow, dwStyle, FALSE, dwExStyle);
	return rcWindow;
}

void WidgetHostWindow::Update(const Rect& rcUpdate)
{
	HDC hdc = m_hPaintDC != NULL ? m_hPaintDC : GetDC(m_hwnd);
	::BitBlt(hdc, rcUpdate.x, rcUpdate.y, rcUpdate.width, rcUpdate.height, 
		m_graphic->GetDC(), rcUpdate.x, rcUpdate.y, SRCCOPY);
	if (m_hPaintDC == NULL)
		ReleaseDC(m_hwnd, hdc);
}

boost::signal<void ()> & WidgetHostWindow::sigWindowDestroy()
{
	return m_sigWindowDestroy;
}
}