#pragma once

#include "WidgetUI.h"
#include "InputState.h"
#include "Widget.h"
#include "Geometry.h"

namespace wui{

class TopLevelWindow
{
public:
	TopLevelWindow() {}

	Widget *pWidget;
	Widget *pFocusedWidget;
	Widget *pOwner;
	std::vector<Rect> updateRegions;
};

class Widget;
class WindowManager;

class WIDGETUI_API Manager
{
	friend class Widget;
	friend class Window;
	friend class WindowManager;
	
public:
	Manager(void);
	virtual ~Manager(void);
	
	WindowManager* GetWindowManager() const;
	Widget *GetCaptureWidget() const;
	Widget *GetFocusedWidget();
	void SetWindowManager(WindowManager* pWindowManager);
	void SetCaptureWidget(Widget *pWidget, bool state);
	void RequestRepaint(const Rect &rect, Widget *pRootWidget);
	boost::signal<void (Message &)> &sigFilterMessage() { return m_sigFilterMessage; }
	
private:
	Widget *GetOwnerWidget(const Widget *pWidget);
	const Widget *GetOwnerWidget(const Widget *pWidget) const;
	bool HasFocus(const Widget *pWidget) const;
	TopLevelWindow *GetTopLevelWindow(const Widget * const pWidget) const;

	Widget *GetCancelWidget(Widget *pWidget=0);
	Widget *GetDefaultWidget(Widget *pWidget=0);

	void AddWidget(Widget *pWidget, Widget *pOwner, TopLevelDescription desc);
	void RemoveWidget(Widget *pWidget);
	void GainFocus(Widget *pWidget);
	void LooseFocus(Widget *pWidget);
	void SetEnabled(Widget *pWidget, bool bEnable);
	void DispatchMessage(Message &message);

	void ProcessStandardGuiKeys(Message &message);

	void OnFocusLost(TopLevelWindow *pToplevelWindow);
	void OnFocusGained(TopLevelWindow *pToplevelWindow);
	void OnResize(TopLevelWindow *pToplevelWindow, const Size &newSize);
	void OnPaint(TopLevelWindow *pToplevelWindow, const Rect &updateRect);
	void OnClose(TopLevelWindow *pToplevelWindow);
	void OnDestroy(TopLevelWindow *pToplevelWindow);
	void OnInputReceived(TopLevelWindow *pToplevelWindow, const InputEvent &inputEvent, const InputState &inputState);
	
private:
	void DeliverMessage(Message &message);

private:
	std::vector<TopLevelWindow *> m_rootWidgets;
	boost::signal<void (Message &)> m_sigFilterMessage;
	Widget *m_pMouseCaptureWidget;
	Widget *m_pMouseOverWidget;
	WindowManager* m_pWindowManager;
};

}