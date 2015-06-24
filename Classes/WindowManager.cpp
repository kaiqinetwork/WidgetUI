#include "StdAfx.h"
#include "WindowManager.h"
#include <boost/bind.hpp>
#include "WidgetHostWindow.h"
#include "Manager.h"
#include "InputDevice.h"

namespace wui{

WindowManager::WindowManager(void)
: m_pCaptureMouseWindow(NULL), m_pManager(NULL)
{
}

WindowManager::~WindowManager(void)
{
	for (std::map<TopLevelWindow *, WidgetHostWindow *>::const_iterator iter = m_windows.begin(); 
		iter != m_windows.end(); ++iter)
	{
		delete iter->second;
	}
}

void WindowManager::CreateWindowHack(TopLevelWindow* handle, TopLevelWindow* owner, 
	Widget* pWidget, TopLevelDescription desc)
{
	WidgetHostWindow *pOwnerHostWindow = 0;
	if (owner)
	{
		pOwnerHostWindow = m_windows[owner];
		desc.SetOwnerWindow(pOwnerHostWindow);
	}

	WidgetHostWindow *pTopLevelHostWindow = new WidgetHostWindow(desc);
	
	pTopLevelHostWindow->sigLostFocus().connect(boost::bind(&WindowManager::OnHostWindowLostFocus, this, handle));
	pTopLevelHostWindow->sigGotFocus().connect(boost::bind(&WindowManager::OnHostWindowGotFocus, this, handle));
	pTopLevelHostWindow->sigResize().connect(boost::bind(&WindowManager::OnHostWindowResize, this, _1, _2, handle));
	pTopLevelHostWindow->sigPaint().connect(boost::bind(&WindowManager::OnHostWindowPaint, this, _1, handle));
	pTopLevelHostWindow->sigWindowClose().connect(boost::bind(&WindowManager::OnHostWindowClose, this, handle));
	pTopLevelHostWindow->sigWindowDestroy().connect(boost::bind(&WindowManager::OnHostWindowDestroy, this, handle));

	InputContext& ic = pTopLevelHostWindow->GetInputContext();
	ic.GetMouse()->sigKeyUp().connect(boost::bind(&WindowManager::OnInput, this, _1, _2, handle));
	ic.GetMouse()->sigKeyDown().connect(boost::bind(&WindowManager::OnInput, this, _1, _2, handle));
	ic.GetMouse()->sigPointerMove().connect(boost::bind(&WindowManager::OnInput, this, _1, _2, handle));
	ic.GetKeyboard()->sigKeyUp().connect(boost::bind(&WindowManager::OnInput, this, _1, _2, handle));
	ic.GetKeyboard()->sigKeyDown().connect(boost::bind(&WindowManager::OnInput, this, _1, _2, handle));

	m_windows[handle] = pTopLevelHostWindow;
}

void WindowManager::DestroyWindow(TopLevelWindow* handle)
{
	CaptureMouse(handle, false);	// Ensure the destroyed window has not captured the mouse

	std::map<TopLevelWindow *, WidgetHostWindow *>::iterator it = m_windows.find(handle);
	delete m_windows[handle];
	m_windows.erase(it);
}

void WindowManager::EnableWindow(TopLevelWindow* handle, bool bEnable)
{
	m_windows[handle]->SetEnabled(bEnable);
}

bool WindowManager::HasFocus(TopLevelWindow* handle) const
{
	std::map<TopLevelWindow *, WidgetHostWindow *>::const_iterator it = m_windows.find(handle);
	if (it != m_windows.end())
		return it->second->HasFocus();
	else
		return false;
}

void WindowManager::SetVisible(TopLevelWindow* handle, bool bVisible, bool bActiveRootWin)
{
	m_windows[handle]->SetVisible(bVisible, bActiveRootWin);
}

void WindowManager::SetGeometry(TopLevelWindow* handle, const Rect& geometry, bool bClientArea)
{
	m_windows[handle]->SetPosition(geometry, bClientArea);
}

Rect WindowManager::GetGeometry(TopLevelWindow* handle, bool bClientArea) const
{
	std::map<TopLevelWindow *, WidgetHostWindow *>::const_iterator it = m_windows.find(handle);
	if (it != m_windows.end())
	{
		if (bClientArea)
		{
			Rect r = it->second->GetViewPort();
			Point tl = it->second->ClientToScreen(Point(r.x,r.y));
			r.x += tl.x;
			r.y += tl.y;
			return r;
		}
		else
			return it->second->GetGeometry();
	}
	else
	{
		return Rect(0, 0, 0, 0);
	}
}

Point WindowManager::ScreenToWindow(TopLevelWindow* handle, const Point& ptScreen, bool bClientArea) const
{
	std::map<TopLevelWindow *, WidgetHostWindow *>::const_iterator it = m_windows.find(handle);
	if (it != m_windows.end())
	{
		if (bClientArea)
		{
			return it->second->ScreenToClient(ptScreen);
		}
		else
		{
			Rect geometry = it->second->GetGeometry();
			return Point(ptScreen.x - geometry.x, ptScreen.y - geometry.y);
		}
	}
	else
	{
		return Point(0, 0);
	}
}

Point WindowManager::WindowToScreen(TopLevelWindow* handle, const Point& ptWindow, bool bClientArea) const
{
	std::map<TopLevelWindow *, WidgetHostWindow *>::const_iterator it = m_windows.find(handle);
	if (it != m_windows.end())
	{
		if (bClientArea)
		{
			return it->second->ClientToScreen(ptWindow);
		}
		else
		{
			Rect geometry = it->second->GetGeometry();
			return Point(geometry.x + ptWindow.x, geometry.y + ptWindow.y);
		}
	}
	else
	{
		return Point(0, 0);
	}
}

XGraphics& WindowManager::GetGraphic(TopLevelWindow* handle)
{
	std::map<TopLevelWindow *, WidgetHostWindow *>::const_iterator iter = m_windows.find(handle);
	if (iter == m_windows.end())
		throw std::exception("Cannot find window handle");

	return iter->second->GetGraphic();
}

InputContext& WindowManager::GetInputContext(TopLevelWindow* handle)
{
	std::map<TopLevelWindow *, WidgetHostWindow *>::const_iterator iter = m_windows.find(handle);
	if (iter == m_windows.end())
		throw std::exception("Cannot find window handle");

	return iter->second->GetInputContext();
}

void WindowManager::OnHostWindowLostFocus(TopLevelWindow *handle)
{
	m_pManager->OnFocusLost(handle);
}

void WindowManager::OnHostWindowGotFocus(TopLevelWindow *handle)
{
	m_pManager->OnFocusGained(handle);
}

void WindowManager::OnHostWindowResize(int width, int height, TopLevelWindow *handle)
{
	m_pManager->OnResize(handle, Size(width, height));
}

void WindowManager::OnHostWindowPaint(const Rect &rect, TopLevelWindow *handle)
{
	m_pManager->OnPaint(handle, rect);
}

void WindowManager::OnHostWindowClose(TopLevelWindow *handle)
{
	m_pManager->OnClose(handle);
}

void WindowManager::OnInput(const InputEvent &event, const InputState &inputState, TopLevelWindow *handle)
{
	m_pManager->OnInputReceived(handle, event, inputState);
}

void WindowManager::SetManager(Manager* pManager)
{
	m_pManager = pManager;
}

XGraphics& WindowManager::BeginPaint(TopLevelWindow *handle, const Rect &rcUpdate)
{
	XGraphics &graphic = m_windows[handle]->GetGraphic();
// 	HWND hwnd = m_windows[handle]->GetHwnd();
// 	HDC hdc = GetDC(hwnd);
// 	::BitBlt(graphic.GetDC()->GetSafeHdc(), 0, 0, graphic.GetWidth(), graphic.GetHeight(), 
// 		hdc, 0, 0, SRCCOPY);
// 	ReleaseDC(hwnd, hdc);
	
	return graphic;
}

void WindowManager::EndPaint(TopLevelWindow *handle, const Rect &rcUpdate)
{
	m_windows[handle]->Update(rcUpdate);
}

bool WindowManager::IsMinimized(TopLevelWindow *handle) const
{
	if (handle != 0)
	{
		std::map<TopLevelWindow *, WidgetHostWindow *>::const_iterator iter = m_windows.find(handle);
		if (iter == m_windows.end())
			return false;

		return iter->second->IsMinimized();
	}

	return false;
}

bool WindowManager::IsMaximized(TopLevelWindow *handle) const
{
	if (handle != 0)
	{
		std::map<TopLevelWindow *, WidgetHostWindow *>::const_iterator iter = m_windows.find(handle);
		if (iter == m_windows.end())
			return false;

		return iter->second->IsMaximized();
	}

	return false;
}

void WindowManager::CaptureMouse(TopLevelWindow *handle, bool state)
{
	if (state)
	{
		m_windows[handle]->CaptureMouse(true);
		m_pCaptureMouseWindow = handle;
	}
	else
	{
		// Don't remove capture to the window, if handle does not control the capture
		if (m_pCaptureMouseWindow == handle)
		{
			m_windows[m_pCaptureMouseWindow]->CaptureMouse(false);
			m_pCaptureMouseWindow = NULL;
		}
	}
}

void WindowManager::BringToFront(TopLevelWindow *handle)
{
	if (handle != 0)
	{
		m_windows[handle]->BringToFront();
	}
}

void WindowManager::RequestRepaint(TopLevelWindow *handle, const Rect &rcUpdate)
{
	m_windows[handle]->RequestRepaint(rcUpdate);
}

WidgetHostWindow* WindowManager::GetHostWindow(TopLevelWindow *handle)
{
	return m_windows[handle];
}

void WindowManager::OnHostWindowDestroy(TopLevelWindow *handle)
{
	m_pManager->OnDestroy(handle);
}

void WindowManager::SetCursor(TopLevelWindow *handle, const HCURSOR cursor)
{
	m_windows[handle]->SetCursor(cursor);
}
}