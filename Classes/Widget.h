#pragma once

#include "WidgetUI.h"
#include "WindowDescription.h"
#include "Graphic.h"
#include "Message.h"
#include "Geometry.h"
#include "InputEvent.h"
#include "InputContext.h"
#include <boost/signals.hpp>

namespace wui{

class Manager;

typedef WindowDescription TopLevelDescription;

class WIDGETUI_API Widget
{
	friend class Manager;
	
public:
	Widget(Widget* pParent);
	Widget(Manager* pManager, TopLevelDescription description);
	Widget(Widget *owner, TopLevelDescription description);
	virtual ~Widget(void);
	
	enum FocusPolicy
	{
		FOCUS_REFUSE,
		FOCUS_LOCAL,
		FOCUS_GROUP,
		FOCUS_PARENT
	};
	
	Rect GetGeometry() const;
	int GetWidth() const;
	int GetHeight() const;
	Size GetSize() const;
	Rect GetWindowGeometry() const;
	std::string GetTypeName() const;
	std::string GetClassName() const;
	std::string GetIdName() const;
	std::string GetElementName() const;
	bool HasFocus() const;
	bool GetAllowResize() const;
	bool GetClipChildren() const;
	FocusPolicy GetFocusPolicy() const;
	std::string GetWidgetGroupName() const;
	bool IsSelectedInGroup() const;
	bool GetBlocksDefaultAction() const;
	Manager* GetManager() const;

	const Widget *GetParentWidget() const;
	Widget *GetParentWidget();
	const Widget *GetOwnerWidget() const;
	Widget *GetOwnerWidget();
	const std::vector<Widget*> GetChildWidgets() const;

	const Widget *GetFirstChild() const;
	Widget *GetFirstChild();
	const Widget *GetLastChild() const;
	Widget *GetLastChild();
	Widget *GetNextWidgetInTree();
	Widget *GetPreviousWidgetInTree();
	
	std::vector<Widget*> GetChildWidgetGroup(const std::string &strGroupName) const;
	Widget *GetGroupSelectedWidget();
	
	Widget *GetNamedItem(const std::string &strId);

	bool HasChildWidgets() const;

	const Widget *GetPreviousSibling() const;
	Widget *GetPreviousSibling();
	const Widget *GetNextSibling() const;
	Widget *GetNextSibling();
	
	bool IsDescendantOf(Widget *pWidget);
	bool IsAncestorOf(Widget *pWidget);

	const Widget *GetTopLevelWidget() const;
	Widget *GetTopLevelWidget();
	
	bool IsEnabled() const;
	bool IsVisible() const;
	bool IsActive() const;

	Widget *GetWidgetAt(const Point &point);

	virtual Size GetPreferredSize();
	int GetPreferredWidth();
	int GetPreferredHeight();
	
	XGraphics& GetGraphic();
	InputContext& GetInputContext();
	WidgetHostWindow* GetHostWindow() const;

	Point WindowToWidgetCoords(const Point &point) const;
	Rect WindowToWidgetCoords(const Rect &rect) const;
	Point WidgetToWindowCoords(const Point &point) const;
	Rect WidgetToWindowCoords(const Rect &rect) const;
	Point ScreenToWidgetCoords(const Point &point) const;
	Point WidgetToScreenCoords(const Point &point) const;
	
	bool IsDefault();
	bool IsCancel();
	bool IsDoubleClickEnabled() const;
	
	void SetGeometry(Rect geometry);
	void SetWindowGeometry(Rect geometry);
	void SetTypeName(const std::string &strName); 
	void SetClassName(const std::string &strName);
	void SetIdName(const std::string &strName);
	void SetEnabled(bool bEnable = true);
	void SetClipChildren(bool bClip = true);
	void SetVisible(bool bVisible = true, bool bActivateRootWin = true);
	void SetFocus(bool bEnable = true);
	void SetFocusPolicy(FocusPolicy policy);
	void SetCursor(const HCURSOR cursor);
	void FocusNext();
	void FocusPrevious();
	void RequestRepaint();
	void RequestRepaint(Rect rect);
	void CaptureMouse(bool capture);
	
	void Render(XGraphics& graphic, const Rect &rcClip, bool bIncludeChildren = true);
	void Paint();
	void Paint(const Rect &rcUpdate);

public:
	boost::signal<void (XGraphics &, const Rect &)> &sigRender();
	boost::signal<void (Message &)> &sigProcessMessage();
	boost::signal<bool ()> &sigClose();
	boost::signal<bool ()> &sigActivated();
	boost::signal<bool ()> &sigDeactivated();
	boost::signal<bool ()> &sigFocusLost();
	boost::signal<bool ()> &sigFocusGained();
	boost::signal<bool ()> &sigPointerEnter();
	boost::signal<bool ()> &sigPointerExit();
	boost::signal<bool (const InputEvent &)> &sigInput();
	boost::signal<bool (const InputEvent &)> &sigInputPressed();
	boost::signal<bool (const InputEvent &)> &sigInputReleased();
	boost::signal<bool (const InputEvent &)> &sigInputDoubleClick();
	boost::signal<bool (const InputEvent &)> &sigInputPointerMoved();
	boost::signal<void (bool)> &sigVisiblityChange();
	boost::signal<void ()> &sigStyleChanged();
	boost::signal<void ()> &sigEnableModeChanged();
	boost::signal<void ()> &sigResized();
	
private:
	void SetGeometry(Rect newGeometry, bool bClientArea);
	void GeometryUpdated();
	
protected:
	Manager* m_pManager;

	Widget *m_pParent;
	Widget *m_pPrevSibling;
	Widget *m_pNextSibling;
	Widget *m_pFirstChild;
	Widget *m_pLastChild;
	FocusPolicy m_nFocusPolicy;
	
	std::string m_strTypeName;
	std::string m_strClassName;
	std::string m_strIdName;
	bool m_bAllowResize;
	bool m_bClipChildren;
	Rect m_rcClipChildren;
	bool m_bEnabled;
	bool m_bVisible;
	bool m_bActivated;
	bool m_bClickThrough;
	Rect m_geometry;
	mutable std::string m_strElementName;
	bool m_bDefaultHandler;
	bool m_bCancelHandler;
	bool m_bBlocksDefaultActionWhenFocused;
	bool m_bIsSelectedInGroup;
	std::string m_strGroupName;
	bool m_bDoubleClickEnabled;

private:
	boost::signal<void (XGraphics &, const Rect &)> m_sigRender;
	boost::signal<void (Message &)> m_sigProcessMessage;
	boost::signal<bool ()> m_sigClose;
	boost::signal<bool ()> m_sigActivated;
	boost::signal<bool ()> m_sigDeactivated;
	boost::signal<bool ()> m_sigFocusLost;
	boost::signal<bool ()> m_sigFocusGained;
	boost::signal<bool ()> m_sigPointerEnter;
	boost::signal<bool ()> m_sigPointerExit;
	boost::signal<bool (const InputEvent &)> m_sigInput;
	boost::signal<bool (const InputEvent &)> m_sigInputPressed;
	boost::signal<bool (const InputEvent &)> m_sigInputReleased;
	boost::signal<bool (const InputEvent &)> m_sigInputDoubleClick;
	boost::signal<bool (const InputEvent &)> m_sigInputPointerMoved;
	boost::signal<void ()> m_sigStyleChanged;
	boost::signal<void ()> m_sigEnableModeChanged;
	boost::signal<void ()> m_sigResized;
	boost::signal<void (bool)> m_sigVisibilityChange;
	
	static XGraphics m_dummyGraphic;
	static InputContext m_dummyInputContext;
};
}