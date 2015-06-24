#pragma once

#include "WidgetUI.h"
#include "WindowDescription.h"
#include <boost/signals.hpp>
#include "InputContext.h"
#include "Graphic.h"

namespace wui{

class KeyboardDevice;
class MouseDevice;

class WIDGETUI_API WidgetHostWindow
{
public:
	WidgetHostWindow(void);
	WidgetHostWindow(const std::string &strTitle,
		int nWidth,
		int nHeight,
		bool bAllowResize = false);
	WidgetHostWindow(const WindowDescription &desc);
	~WidgetHostWindow(void);
	
	Rect GetGeometry() const;
	Rect GetViewPort() const;
	bool HasFocus() const;
	bool IsVisible() const;
	bool IsMinimized() const;
	bool IsMaximized() const;

	HWND GetHwnd() const;
	
	Point ClientToScreen(const Point &client);
	Point ScreenToClient(const Point &screen);
	void CaptureMouse(bool capture);
	void RequestRepaint(const Rect &rect);
	void SetTitle(const std::string &strTitle);
	void SetPosition(const Rect &pos, bool bClientArea);
	void SetEnabled(bool enable);
	void SetVisible(bool visible, bool activate);
	void SetSize(int width, int height, bool bClientArea);
	void SetMinimumSize(int width, int height);
	void SetMaximumSize(int width, int height);
	void Minimize();
	void Restore();
	void Maximize();
	void Show(bool activate = true);
	void Hide();
	void BringToFront();
	void ShowCursor();
	void SetCursor(const HCURSOR hCursor);
	void HideCursor();
	void SetLargeIcon(HICON hIcon);
	void SetSmallIcon(HICON hIcon);
	
	boost::signal<void ()> &sigLostFocus();
	boost::signal<void ()> &sigGotFocus();
	boost::signal<void (int, int)> &sigResize();
	boost::signal<void (const Rect &)> &sigPaint();
	boost::signal<void ()> &sigWindowClose();
	boost::signal<void ()> &sigWindowMinimized();
	boost::signal<void ()> &sigWindowMaximized();
	boost::signal<void ()> &sigWindowDestroy();
	
	InputContext& GetInputContext();
	XGraphics& GetGraphic();
	
	void Update(const Rect& rcUpdate);

private:
	void _Init();
	
	static LRESULT WINAPI static_window_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	
	void Create(const WindowDescription &desc);
	void ModifyWindow(const WindowDescription &desc);
	void CreateNewWindow(const WindowDescription &desc);
	
	void GetStylesFromDescription(const WindowDescription &desc, DWORD& dwStyle, DWORD& dwExStyle);
	RECT GetWindowGeometryFromDescription(const WindowDescription &desc, DWORD dwStyle, DWORD dwExStyle);
	
	void ReceivedKeyboardInput(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void ReceivedMouseInput(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void ReceivedMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam);
	
	void RegisterWindowClass();
	
	void SetModifierKeys(InputEvent &key);
	
private:
	HWND m_hwnd;
	bool m_bDestroyHwnd;
	HCURSOR m_hCurrCursor;
	HICON m_hLargeIcon;
	HICON m_hSmallIcon;
	bool m_bCursorSet;
	bool m_bCursorHidden;
	KeyboardDevice* m_keyboard;
	MouseDevice* m_mouse;
	Point m_ptMouse;
	std::map<int, int> m_repeatCount;
	InputContext m_ic;
	XGraphics* m_graphic;
	Size m_minSize;
	Size m_maxSize;
	std::string m_strClassName;
	bool m_minimized;
	bool m_maximized;
	HDC m_hPaintDC;
	PAINTSTRUCT m_paintstruct;
	WNDPROC m_pfnOld;
	
	boost::signal<void ()> m_sigLostFocus;
	boost::signal<void ()> m_sigGotFocus;
	boost::signal<void (int, int)> m_sigResize;
	boost::signal<void (const Rect&)> m_sigPaint;
	boost::signal<void ()> m_sigWindowClose;
	boost::signal<void ()> m_sigWindowDestroy;
	boost::signal<void ()> m_sigWindowMinimized;
	boost::signal<void ()> m_sigWindowMaximized;
	boost::signal<void ()> m_sigWindowRestored;
	boost::signal<void ()> m_sigWindowMoved;
};
}