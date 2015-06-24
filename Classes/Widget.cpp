#include "StdAfx.h"
#include "Widget.h"
#include "Manager.h"
#include "WindowManager.h"

namespace wui{

XGraphics Widget::m_dummyGraphic(Size(0,0));
InputContext Widget::m_dummyInputContext;

Widget::Widget(Widget* pParent)
: m_pPrevSibling(0), m_pNextSibling(0), m_pFirstChild(0), m_pLastChild(0),
m_nFocusPolicy(Widget::FOCUS_REFUSE), m_bAllowResize(false), m_bClipChildren(false), m_bEnabled(true),
m_bVisible(true), m_bActivated(false), m_bDefaultHandler(false), m_bCancelHandler(false),
m_bBlocksDefaultActionWhenFocused(false), m_bIsSelectedInGroup(false)
{
	m_rcClipChildren.SetRectEmpty();
	
	if (pParent)
	{
		m_pManager = pParent->GetManager();
		m_pParent = pParent;
	}
	else
		throw std::exception("Cannot create child widget with no parent!");
		
	m_strTypeName = "widget";
	m_geometry = Rect(0,0,0,0);

	if (m_pParent->m_pLastChild)
	{
		m_pParent->m_pLastChild->m_pNextSibling = this;
		m_pPrevSibling = m_pParent->m_pLastChild;
		m_pParent->m_pLastChild = this;
	}
	else
	{
		m_pParent->m_pFirstChild = this;
		m_pParent->m_pLastChild = this;
	}
}

Widget::Widget(Manager* pManager, TopLevelDescription description)
: m_pParent(NULL), m_pPrevSibling(0), m_pNextSibling(0), m_pFirstChild(0), m_pLastChild(0),
m_nFocusPolicy(Widget::FOCUS_REFUSE), m_bAllowResize(false), m_bClipChildren(false), m_bEnabled(true),
m_bVisible(true), m_bActivated(false), m_bDefaultHandler(false), m_bCancelHandler(false),
m_bBlocksDefaultActionWhenFocused(false), m_bIsSelectedInGroup(false)
{
	m_rcClipChildren.SetRectEmpty();
	
	if (pManager)
	{
		m_pManager = pManager;
	}
	else
		throw std::exception("Cannot create widget with a null manager!");
		
	m_bAllowResize = description.GetAllowResize();
	m_bVisible = description.IsVisible();
	m_pManager->AddWidget(this, 0, description);
	m_strTypeName = "widget";
	m_geometry = m_pManager->m_pWindowManager->GetGeometry(m_pManager->GetTopLevelWindow(this), true);
	RequestRepaint();
}

Widget::Widget(Widget *owner, TopLevelDescription description)
: m_pParent(NULL), m_pPrevSibling(0), m_pNextSibling(0), m_pFirstChild(0), m_pLastChild(0),
m_nFocusPolicy(Widget::FOCUS_REFUSE), m_bClipChildren(false), m_bEnabled(true),
m_bActivated(false), m_bDefaultHandler(false), m_bCancelHandler(false),
m_bBlocksDefaultActionWhenFocused(false), m_bIsSelectedInGroup(false)
{
	m_rcClipChildren.SetRectEmpty();
	
	if (owner)
	{
		m_pManager = owner->GetManager();		
	}
	else
		throw std::exception("Cannot create child widget with a null owner!");
		
	m_pManager->AddWidget(this, owner, description);
	m_strTypeName = "widget";
	m_geometry = m_pManager->m_pWindowManager->GetGeometry(m_pManager->GetTopLevelWindow(this), true);
	RequestRepaint();
}

Widget::~Widget(void)
{
	while (m_pLastChild)
		delete m_pLastChild;

	if (m_pPrevSibling)
		m_pPrevSibling->m_pNextSibling = m_pNextSibling;
	if (m_pNextSibling)
		m_pNextSibling->m_pPrevSibling = m_pPrevSibling;

	if (m_pPrevSibling == 0 && m_pParent)
		m_pParent->m_pFirstChild = m_pNextSibling;
	if (m_pNextSibling == 0 && m_pParent)
		m_pParent->m_pLastChild = m_pPrevSibling;

	m_pManager->RemoveWidget(this);
}

Rect Widget::GetGeometry() const
{
	if (m_pParent == 0)
	{
		return m_pManager->m_pWindowManager->GetGeometry(m_pManager->GetTopLevelWindow(this), true);
	}
	else
	{
		return m_geometry;
	}
}

int Widget::GetWidth() const
{
	return m_geometry.width;
}

int Widget::GetHeight() const
{
	return m_geometry.height;
}

Size Widget::GetSize() const
{
	return Size(m_geometry.width, m_geometry.height);
}

Rect Widget::GetWindowGeometry() const
{
	if (m_pParent == 0)
	{
		return m_pManager->m_pWindowManager->GetGeometry(m_pManager->GetTopLevelWindow(this), false);
	}
	else
	{
		return m_geometry;
	}
}

std::string Widget::GetTypeName() const
{
	return m_strTypeName;
}

std::string Widget::GetClassName() const
{
	return m_strClassName;
}

std::string Widget::GetIdName() const
{
	return m_strIdName;
}

std::string Widget::GetElementName() const
{
	if (!m_strElementName.empty())
		return m_strElementName;

	std::string strElementName = m_strTypeName;
	if (!m_strClassName.empty())
		strElementName += "." + m_strClassName;
	if (!m_strIdName.empty())
		strElementName += "#" + m_strIdName;
	if (m_pParent)
	{
		std::string strPathName = m_pParent->GetElementName();
		if (!strPathName.empty())
			strElementName = strPathName + " " + strElementName;
	}

	m_strElementName = strElementName;	
	return m_strElementName;
}

bool Widget::HasFocus() const
{
	return m_pManager->HasFocus(this);
}

bool Widget::GetAllowResize() const
{
	return m_bAllowResize;
}

bool Widget::GetClipChildren() const
{
	return m_bClipChildren;
}

Widget::FocusPolicy Widget::GetFocusPolicy() const
{
	return m_nFocusPolicy;
}

std::string Widget::GetWidgetGroupName() const
{
	return m_strGroupName;
}

bool Widget::IsSelectedInGroup() const
{
	return m_bIsSelectedInGroup;
}

bool Widget::GetBlocksDefaultAction() const
{
	return m_bBlocksDefaultActionWhenFocused;
}

Manager* Widget::GetManager() const
{
	return m_pManager;
}

const Widget * Widget::GetParentWidget() const
{
	return m_pParent;
}

Widget * Widget::GetParentWidget()
{
	return m_pParent;
}

const Widget * Widget::GetOwnerWidget() const
{
	return m_pManager->GetOwnerWidget(this);
}

Widget * Widget::GetOwnerWidget()
{
	return m_pManager->GetOwnerWidget(this);
}

const std::vector<Widget*> Widget::GetChildWidgets() const
{
	std::vector<Widget*> children;
	Widget *cur_child = m_pFirstChild;
	while (cur_child)
	{
		children.push_back(cur_child);
		cur_child = cur_child->GetNextSibling();
	}
	return children;
}

const Widget * Widget::GetFirstChild() const
{
	return m_pFirstChild;
}

Widget * Widget::GetFirstChild()
{
	return m_pFirstChild;
}

const Widget * Widget::GetLastChild() const
{
	return m_pLastChild;
}

Widget * Widget::GetLastChild()
{
	return m_pLastChild;
}

Widget * Widget::GetNextWidgetInTree()
{
	if (HasChildWidgets())
		return GetFirstChild();

	if (m_pNextSibling)
		return m_pNextSibling;

	Widget *pParent = m_pParent;
	while (pParent)
	{
		if (pParent->GetNextSibling())
			return pParent->GetNextSibling();
		pParent = pParent->GetParentWidget();
	}

	// Reached end of tree. Return first item.
	return GetTopLevelWidget();
}

Widget * Widget::GetPreviousWidgetInTree()
{
	if (m_pPrevSibling)
	{
		// return last grand-child of sibling.
		if (m_pPrevSibling->HasChildWidgets())
		{
			Widget *last = m_pPrevSibling->m_pLastChild;
			while (last->HasChildWidgets())
			{
				last = last->GetLastChild();
			}
			return last;
		}
		else
		{
			// sibling has no children, return sibling.
			return m_pPrevSibling;
		}
	}

	// no previous sibling, return parent.
	if (m_pParent)
		return m_pParent;

	// No parent, must be top-level component. Find last child.
	Widget *last = m_pLastChild;

	// No child, must be a lonely top-level component.
	if(last == 0)
		return this;

	while (last->HasChildWidgets())
	{
		last = last->GetLastChild();
	}
	return last;
}

std::vector<Widget*> Widget::GetChildWidgetGroup(const std::string &strGroupName) const
{
	std::vector<Widget *> group;
	Widget *cur_child = m_pFirstChild;
	while (cur_child)
	{
		if (cur_child->GetWidgetGroupName() == strGroupName)
		{
			group.push_back(cur_child);
		}
		cur_child = cur_child->GetNextSibling();
	}
	return group;
}

Widget * Widget::GetGroupSelectedWidget()
{
	Widget *parent = GetParentWidget();

	if (parent)
	{
		std::vector<Widget*> group = parent->GetChildWidgetGroup(GetWidgetGroupName());
		std::vector<Widget*>::size_type i;
		for (i = 0; i < group.size(); i++)
		{
			if (group[i]->IsSelectedInGroup())
			{
				return group[i];
			}
		}
	}

	return 0;
}

Widget * Widget::GetNamedItem(const std::string &strId)
{
	Widget *cur = m_pFirstChild;
	while (cur && cur != this)
	{
		if (cur->GetIdName() == strId)
			return cur;

		if (cur->m_pFirstChild)
			cur = cur->m_pFirstChild;
		else if (cur->m_pNextSibling)
			cur = cur->m_pNextSibling;
		else
		{
			while (cur != this)
			{
				if (cur->m_pParent->m_pNextSibling)
				{
					cur = cur->m_pParent->m_pNextSibling;
					break;
				}
				else
					cur = cur->m_pParent;
			}
		}
	}

	return 0;
}

bool Widget::HasChildWidgets() const
{
	return (m_pFirstChild != 0);
}

const Widget * Widget::GetPreviousSibling() const
{
	return m_pPrevSibling;
}

Widget * Widget::GetPreviousSibling()
{
	return m_pPrevSibling;
}

const Widget * Widget::GetNextSibling() const
{
	return m_pNextSibling;
}

Widget * Widget::GetNextSibling()
{
	return m_pNextSibling;
}

bool Widget::IsDescendantOf(Widget *pWidget)
{
	if(m_pParent == 0)
		return false;

	if(m_pParent == pWidget)
		return true;

	return m_pParent->IsDescendantOf(pWidget);
}

bool Widget::IsAncestorOf(Widget *pWidget)
{
	Widget *cur_child = m_pFirstChild;
	while (cur_child)
	{
		if(cur_child == pWidget)
			return true;

		if(cur_child->IsAncestorOf(pWidget))
			return true;

		cur_child = cur_child->GetNextSibling();
	}

	return false;
}

const Widget * Widget::GetTopLevelWidget() const
{
	const Widget *parent = this;

	while (true)
	{
		if (parent->GetParentWidget())
			parent = parent->GetParentWidget();
		else 
			break;
	}

	return parent;
}

Widget * Widget::GetTopLevelWidget()
{
	Widget *parent = this;

	while (true)
	{
		if (parent->GetParentWidget())
			parent = parent->GetParentWidget();
		else 
			break;
	}

	return parent;
}

bool Widget::IsEnabled() const
{
	return m_bEnabled;
}

bool Widget::IsVisible() const
{
	return m_bVisible;
}

bool Widget::IsActive() const
{
	return m_bActivated;
}

Widget * Widget::GetWidgetAt(const Point &point)
{
	if (IsVisible() == false)
	{
		return 0;
	}
	else
	{
		Rect rcParent(Point(0, 0), GetSize());
		if ( rcParent.Contains(point) )  
		{
			std::vector<Widget *> children = GetChildWidgets();
			size_t pos, size = children.size();

			for( pos=size; pos>0; pos-- )
			{
				Widget *child = children[pos-1];
				if(child->IsVisible())
				{
					if (child->GetGeometry().Contains(point))
					{
						Point P = point;
						P.x -= child->GetGeometry().GetLeft();
						P.y -= child->GetGeometry().GetTop();
						Widget *comp = child->GetWidgetAt(P);
						if (comp)
							return comp;
						else
							return child;
					}
				}
			}
			return this;
		}
		else
		{
			return 0;
		}
	}
}

Size Widget::GetPreferredSize()
{
	throw std::exception("Widget::GetPreferredSize shall not be called directly! Derived classes should override this function, if needed!");
	return Size();
}

int Widget::GetPreferredWidth()
{
	return GetPreferredSize().width;
}

int Widget::GetPreferredHeight()
{
	return GetPreferredSize().height;
}

Point Widget::WindowToWidgetCoords(const Point &point) const
{
	Point P = point;

	const Widget *current = this;
	while (current->GetParentWidget())
	{
		Rect g = current->GetGeometry();
		P.x -= g.GetLeft();
		P.y -= g.GetTop();
		current = current->GetParentWidget();
	}

	return P;
}

Rect Widget::WindowToWidgetCoords(const Rect &rect) const
{
	Point tl = WindowToWidgetCoords(Point(rect.x,rect.y));
	Point br = WindowToWidgetCoords(Point(rect.GetRight(),rect.GetBottom()));
	return Rect(tl.x, tl.y, br.x, br.y);
}

Point Widget::WidgetToWindowCoords(const Point &point) const
{
	Point P = point;

	const Widget *current = this;
	while (current->GetParentWidget())
	{
		Rect g = current->GetGeometry();
		P.x += g.x;
		P.y += g.y;
		current = current->GetParentWidget();
	}

	return P;
}

Rect Widget::WidgetToWindowCoords(const Rect &rect) const
{
	Point tl = WidgetToWindowCoords(Point(rect.x,rect.y));
	Point br = WidgetToWindowCoords(Point(rect.GetRight(),rect.GetBottom()));
	return Rect(tl.x, tl.y, br.x, br.y);
}

Point Widget::ScreenToWidgetCoords(const Point &screenPoint) const
{
	TopLevelWindow* pToplevelWindow = m_pManager->GetTopLevelWindow(this);
	Point ptClient;
	return WindowToWidgetCoords(ptClient);
}

Point Widget::WidgetToScreenCoords(const Point &widgetPoint) const
{
	TopLevelWindow *pToplevelWindow = m_pManager->GetTopLevelWindow(this);
	Point ptClient = WidgetToWindowCoords(widgetPoint);
	return m_pManager->m_pWindowManager->WindowToScreen(pToplevelWindow, ptClient, true);
}

bool Widget::IsDefault()
{
	return m_bDefaultHandler;
}

bool Widget::IsCancel()
{
	return m_bCancelHandler;
}

bool Widget::IsDoubleClickEnabled() const
{
	return m_bDoubleClickEnabled;
}

void Widget::FocusNext()
{
	Widget *w = this;

	// Skip over widgets in the same group:
	if (GetFocusPolicy() == FOCUS_GROUP)
	{
		while (w->m_pNextSibling && w->GetWidgetGroupName() == this->GetWidgetGroupName())
		{
			w = w->m_pNextSibling;
		}
	}
	else
		w = w->GetNextWidgetInTree();

	while (w != this)
	{
		if ((w->GetFocusPolicy() == FOCUS_LOCAL || w->GetFocusPolicy() == FOCUS_GROUP) && w->IsVisible() && w->IsEnabled())
		{
			if (w->GetFocusPolicy() == FOCUS_GROUP)
			{
				Widget *pSelectedWidget = w->GetGroupSelectedWidget();
				if (pSelectedWidget)
				{
					pSelectedWidget->SetFocus(true);
				}
				else
				{
					w->SetFocus(true);
				}
			}
			else
			{
				w->SetFocus(true);
			}

			break;
		}

		w = w->GetNextWidgetInTree();
	}
}

void Widget::FocusPrevious()
{
	Widget *w = this;

	// Skip over widgets in the same group:
	if (GetFocusPolicy() == FOCUS_GROUP)
	{
		while (w->m_pPrevSibling && w->GetWidgetGroupName() == this->GetWidgetGroupName())
		{
			w = w->m_pPrevSibling;
		}
	}
	else
		w = w->GetPreviousWidgetInTree();

	while (w != this)
	{
		if ((w->GetFocusPolicy() == FOCUS_LOCAL || w->GetFocusPolicy() == FOCUS_GROUP) && w->IsVisible() && w->IsEnabled())
		{
			if (w->GetFocusPolicy() == FOCUS_GROUP)
			{
				Widget *pSelectedWidget = w->GetGroupSelectedWidget();
				if (pSelectedWidget)
				{
					pSelectedWidget->SetFocus(true);
				}
				else
				{
					w->SetFocus(true);
				}
			}
			else
			{
				w->SetFocus(true);
			}

			break;
		}

		w = w->GetPreviousWidgetInTree();
	}
}

void Widget::SetTypeName(const std::string &strName)
{
	m_strTypeName = strName;
}

void Widget::SetClassName(const std::string &strName)
{
	m_strClassName = strName;
}

void Widget::SetIdName(const std::string &strName)
{
	m_strIdName = strName;
}

void Widget::SetEnabled(bool bEnable /*= true*/)
{
	if (m_bEnabled != bEnable)
	{
		m_bEnabled = bEnable;
		
		if (!m_sigEnableModeChanged.empty())
			m_sigEnableModeChanged();

		if (m_pParent == NULL)
			m_pManager->SetEnabled(this, bEnable);
	}
}

void Widget::SetClipChildren(bool bClip /*= true*/)
{
	m_bClipChildren = bClip;
}

void Widget::SetVisible(bool bVisible /*= true*/, bool bActivateRootWin /*= true*/)
{
	if (bVisible != m_bVisible)
	{
		RequestRepaint();
	}

	m_bVisible = bVisible;
	if (m_pParent == NULL)
	{
		TopLevelWindow *pTopLevelWindow = m_pManager->GetTopLevelWindow(this);
		m_pManager->m_pWindowManager->SetVisible(pTopLevelWindow, bVisible, bActivateRootWin);
	}
}

void Widget::SetFocus(bool bEnable /*= true*/)
{
	if (bEnable)
	{
		m_pManager->GainFocus(this);
	}
	else
	{
		m_pManager->LooseFocus(this);
	}
}

void Widget::SetFocusPolicy(FocusPolicy policy)
{
	m_nFocusPolicy = policy;
}

void Widget::Paint()
{
	Paint(Rect(Point(0,0), GetSize()));
}

void Widget::Paint(const Rect &rcUpdate)
{
	Rect rect = WidgetToWindowCoords(rcUpdate);
	Widget *pTopLevelWidget = GetTopLevelWidget();
	TopLevelWindow *pTopLevelWindow = m_pManager->GetTopLevelWindow(this);

	XGraphics& graphic = m_pManager->m_pWindowManager->BeginPaint(pTopLevelWindow, rect);
	pTopLevelWidget->Render(graphic, rect, true);
	m_pManager->m_pWindowManager->EndPaint(pTopLevelWindow, rect);
}

void Widget::Render(XGraphics& graphic, const Rect &rcClip, bool bIncludeChildren /*= true*/)
{
	if (!m_bVisible)
		return;

	if (m_sigRender.empty() == false)
	{
		m_sigRender(graphic, rcClip);
	}

	if (bIncludeChildren)
	{
		Widget *cur = m_pFirstChild;
		while (cur)
		{
			Rect cur_geometry = cur->GetGeometry();

			Rect update_rect = WidgetToWindowCoords(rcClip);
			update_rect.Intersects(WidgetToWindowCoords(cur_geometry));
			if (!update_rect.RectIsEmpty())
			{
				Rect child_dirty_rect = cur->WindowToWidgetCoords(update_rect);
				graphic.PushModleView(cur_geometry);
				cur->Render(graphic, child_dirty_rect, true);
				graphic.PopModleView();
			}
			cur = cur->m_pNextSibling;
		}
	}
}

boost::signal<void (XGraphics &, const Rect &)> & Widget::sigRender()
{
	return m_sigRender;
}

boost::signal<void (Message &)> & Widget::sigProcessMessage()
{
	return m_sigProcessMessage;
}

boost::signal<bool ()> & Widget::sigClose()
{
	return m_sigClose;
}

boost::signal<bool ()> & Widget::sigActivated()
{
	return m_sigActivated;
}

boost::signal<bool ()> & Widget::sigDeactivated()
{
	return m_sigDeactivated;
}

boost::signal<bool ()> & Widget::sigFocusLost()
{
	return m_sigFocusLost;
}

boost::signal<bool ()> & Widget::sigFocusGained()
{
	return m_sigFocusGained;
}

boost::signal<bool ()> & Widget::sigPointerEnter()
{
	return m_sigPointerEnter;
}

boost::signal<bool ()> & Widget::sigPointerExit()
{
	return m_sigPointerExit;
}

boost::signal<bool (const InputEvent &)> & Widget::sigInput()
{
	return m_sigInput;
}

boost::signal<bool (const InputEvent &)> & Widget::sigInputPressed()
{
	return m_sigInputPressed;
}

boost::signal<bool (const InputEvent &)> & Widget::sigInputReleased()
{
	return m_sigInputReleased;
}

boost::signal<bool (const InputEvent &)> & Widget::sigInputDoubleClick()
{
	return m_sigInputDoubleClick;
}

boost::signal<bool (const InputEvent &)> & Widget::sigInputPointerMoved()
{
	return m_sigInputPointerMoved;
}

boost::signal<void (bool)> & Widget::sigVisiblityChange()
{
	return m_sigVisibilityChange;
}

boost::signal<void ()> & Widget::sigEnableModeChanged()
{
	return m_sigEnableModeChanged;
}

boost::signal<void ()> & Widget::sigResized()
{
	return m_sigResized;
}

void Widget::RequestRepaint()
{
	RequestRepaint(Rect(0, 0, m_geometry.width, m_geometry.height));	
}

void Widget::RequestRepaint(Rect rect)
{
	m_pManager->RequestRepaint(WidgetToWindowCoords(rect), GetTopLevelWidget());
}

void Widget::SetGeometry(Rect newGeometry, bool bClientArea)
{
	if (m_pParent == NULL)
	{
		TopLevelWindow* handle = m_pManager->GetTopLevelWindow(this);
		m_pManager->m_pWindowManager->SetGeometry(handle, newGeometry, bClientArea);
		newGeometry = m_pManager->m_pWindowManager->GetGeometry(handle, true);
	}
	
	if (GetParentWidget())
	{
		GetParentWidget()->RequestRepaint(m_geometry);
	}
	
	if (m_geometry.width != newGeometry.width || 
		m_geometry.height != newGeometry.height)
	{
		m_geometry = newGeometry;
		GeometryUpdated();
	}
	else
	{
		m_geometry = newGeometry;
		RequestRepaint();
	}	
}

void Widget::SetGeometry(Rect geometry)
{
	SetGeometry(geometry, true);
}

void Widget::SetWindowGeometry(Rect geometry)
{
	SetGeometry(geometry, false);
}

void Widget::CaptureMouse(bool capture)
{
	if (capture)
	{
		m_pManager->SetCaptureWidget(this, true);
	}
	else
	{
		if (m_pManager->GetCaptureWidget() == this)
			m_pManager->SetCaptureWidget(this, false);	
	}
}

WidgetHostWindow* Widget::GetHostWindow() const
{
	TopLevelWindow *toplevel = m_pManager->GetTopLevelWindow(this);
	return m_pManager->m_pWindowManager->GetHostWindow(toplevel);
}

void Widget::SetCursor(const HCURSOR cursor)
{
	TopLevelWindow *toplevel = m_pManager->GetTopLevelWindow(this);
	return m_pManager->m_pWindowManager->SetCursor(toplevel, cursor);
}

XGraphics& Widget::GetGraphic()
{
	const Widget *pRootWidget = GetTopLevelWidget();

	std::vector<TopLevelWindow*>::size_type pos, size;
	size = m_pManager->m_rootWidgets.size();
	for (pos = 0; pos < size; pos++)
	{
		TopLevelWindow *cur = m_pManager->m_rootWidgets[pos];
		if (cur->pWidget == pRootWidget)
			return m_pManager->m_pWindowManager->GetGraphic(cur);
	}
	return m_dummyGraphic;
}

InputContext& Widget::GetInputContext()
{
	const Widget *pRootWidget = GetTopLevelWidget();

	std::vector<TopLevelWindow*>::size_type pos, size;
	size = m_pManager->m_rootWidgets.size();
	for (pos = 0; pos < size; pos++)
	{
		TopLevelWindow *cur = m_pManager->m_rootWidgets[pos];
		if (cur->pWidget == pRootWidget)
			return m_pManager->m_pWindowManager->GetInputContext(cur);
	}
	return m_dummyInputContext;
}

boost::signal<void ()> & Widget::sigStyleChanged()
{
	return m_sigStyleChanged;
}

void Widget::GeometryUpdated()
{
	if (!m_sigResized.empty())
		m_sigResized();
		
	RequestRepaint();	
}
}