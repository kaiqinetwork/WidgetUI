#pragma once

#include "WidgetUI.h"
#include "Widget.h"
#include "InputContext.h"
#include "Graphic.h"
#include "Geometry.h"

namespace wui{

class TopLevelWindow;
class Manager;

class WIDGETUI_API WindowManager
{
public:
	WindowManager(void);
	~WindowManager(void);
	
	void SetManager(Manager* pManager);
	void CreateWindowHack(TopLevelWindow* handle, TopLevelWindow* owner, 
		Widget* pWidget, TopLevelDescription desc);
	void DestroyWindow(TopLevelWindow* handle);
	void EnableWindow(TopLevelWindow* handle, bool bEnable);
	bool HasFocus(TopLevelWindow* handle) const;
	void SetVisible(TopLevelWindow* handle, bool bVisible, bool bActiveRootWin);
	void SetGeometry(TopLevelWindow* handle, const Rect& geometry, bool bClientArea);
	Rect GetGeometry(TopLevelWindow* handle, bool bClientArea) const;
	Point ScreenToWindow(TopLevelWindow* handle, const Point& ptScreen, bool bClientArea) const;
	Point WindowToScreen(TopLevelWindow* handle, const Point& ptWindow, bool bClientArea) const;
	XGraphics& GetGraphic(TopLevelWindow* handle);
	InputContext& GetInputContext(TopLevelWindow* handle);
	XGraphics& BeginPaint(TopLevelWindow *handle, const Rect &rcUpdate);
	bool IsMinimized(TopLevelWindow *handle) const;
	bool IsMaximized(TopLevelWindow *handle) const;
	void CaptureMouse(TopLevelWindow *handle, bool state);
	void BringToFront(TopLevelWindow *handle);
	void RequestRepaint(TopLevelWindow *handle, const Rect &rcUpdate);
	void EndPaint(TopLevelWindow *handle, const Rect &rcUpdate);
	void SetCursor(TopLevelWindow *handle, const HCURSOR cursor);
	WidgetHostWindow* GetHostWindow(TopLevelWindow *handle);

private:
	void OnHostWindowLostFocus(TopLevelWindow *handle);
	void OnHostWindowGotFocus(TopLevelWindow *handle);
	void OnHostWindowResize(int width, int height, TopLevelWindow *handle);
	void OnHostWindowPaint(const Rect &rect, TopLevelWindow *handle);
	void OnHostWindowClose(TopLevelWindow *handle);
	void OnHostWindowDestroy(TopLevelWindow *handle);

	void OnInput(const InputEvent &event, const InputState &inputState, TopLevelWindow *handle);

private:
	TopLevelWindow *m_pCaptureMouseWindow;
	std::map<TopLevelWindow *, WidgetHostWindow *> m_windows;
	Manager* m_pManager;
};
}