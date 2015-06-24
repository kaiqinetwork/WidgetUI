#include "StdAfx.h"
#include "WindowDescription.h"

namespace wui{

WindowDescription::WindowDescription(void)
{
	_Init();
}

WindowDescription::WindowDescription(const std::string &strTitle)
{
	_Init();
	m_strTitle = strTitle;
}

WindowDescription::WindowDescription(const std::string &strTitle, const Rect &position, bool bClientArea)
{
	_Init();
	m_strTitle = strTitle;
	m_position = position;
	m_bPositionClientArea = bClientArea;
}

WindowDescription::WindowDescription(const std::string &strTitle, const Size &size, bool bClientArea)
{
	_Init();
	m_strTitle = strTitle;
	m_position.SetRect(0, 0, size.width, size.height);
	m_bPositionClientArea = bClientArea;
}

WindowDescription::WindowDescription(const Rect &position, bool bClientArea)
{
	_Init();
	m_position = position;
	m_bPositionClientArea = bClientArea;
}

WindowDescription::~WindowDescription(void)
{
}

void WindowDescription::_Init()
{
	m_strTitle = "Untitled WidgetUI Window";
	m_position = Rect(-1, -1, 639, 479);
	m_bPositionClientArea = false;
	m_bDecorations = true;
	m_bCaption = true;
	m_bTopmost = false;
	m_bVisible = true;
	m_bMinimizeButton = true;
	m_bMaximizeButton = true;
	m_bChildWindow = false;
	m_bBorder = false;
	m_bSysmenu = true;
	m_bToolWindow = false;
	m_bDialogWindow = false;
	m_bAllowResize = false;
	m_handle = 0;
	m_handleOwner = 0;
	m_pOwner = NULL;
}

const std::string & WindowDescription::GetTitle() const
{
	return m_strTitle;
}

bool WindowDescription::GetDecorations() const
{
	return m_bDecorations;
}

Size WindowDescription::GetSize() const
{
	return Size(m_position.width, m_position.height);
}

Rect WindowDescription::GetPosition() const
{
	return m_position;
}

bool WindowDescription::GetPositionClientArea() const
{
	return m_bPositionClientArea;
}

bool WindowDescription::GetAllowResize() const
{
	return m_bAllowResize;
}

HWND WindowDescription::GetHandle() const
{
	return m_handle;
}

bool WindowDescription::HasCaption() const
{
	return m_bCaption;
}

bool WindowDescription::HasSysmenu() const
{
	return m_bSysmenu;
}

bool WindowDescription::HasMinimizeButton() const
{
	return m_bMinimizeButton;
}

bool WindowDescription::HasMaximizeButton() const
{
	return m_bMaximizeButton;
}

bool WindowDescription::HasBorder() const
{
	return m_bBorder;
}

bool WindowDescription::IsToolWindow() const
{
	return m_bToolWindow;
}

bool WindowDescription::IsDialog() const
{
	return m_bDialogWindow;
}

bool WindowDescription::IsVisible() const
{
	return m_bVisible;
}

bool WindowDescription::IsTopmost() const
{
	return m_bTopmost;
}

WidgetHostWindow* WindowDescription::GetOwner() const
{
	return m_pOwner;
}

void WindowDescription::ShowCaption(bool value /*= true*/)
{
	m_bCaption = value;
}

void WindowDescription::ShowSysmenu(bool value /*= true*/)
{
	m_bSysmenu = value;
}

void WindowDescription::ShowMinimizeButton(bool value /*= true*/)
{
	m_bMinimizeButton = value;
}

void WindowDescription::ShowMaximizeButton(bool value /*= true*/)
{
	m_bMaximizeButton = value;
}

void WindowDescription::ShowBorder(bool value /*= true*/)
{
	m_bBorder = value;
}

void WindowDescription::SetDecorations(bool decorations)
{
	m_bDecorations = decorations;
}

void WindowDescription::SetVisible(bool value /*= true*/)
{
	m_bVisible = value;
}

void WindowDescription::SetTopMost(bool value /*= true*/)
{
	m_bTopmost = value;
}

void WindowDescription::SetToolWindow(bool value /*= true*/)
{
	m_bToolWindow = value;
}

void WindowDescription::SetDialogWindow(bool value /*= true*/)
{
	m_bDialogWindow = value;
}

void WindowDescription::SetOwnerWindow(WidgetHostWindow* pOwner)
{
	m_pOwner = pOwner;
}

void WindowDescription::SetTitle(const std::string& strTitle)
{
	m_strTitle = strTitle;
}

void WindowDescription::SetSize(const Size& size, bool bClientArea)
{
	m_position.width = size.width;
	m_position.height = size.height;
	m_bPositionClientArea = bClientArea;
}

void WindowDescription::SetPosition(const Rect& position, bool bClientArea)
{
	m_position = position;
	m_bPositionClientArea = bClientArea;
}

void WindowDescription::SetAllowResize(bool bAllow)
{
	m_bAllowResize = bAllow;
}

void WindowDescription::SetHandle(HWND handle)
{
	m_handle = handle;
}

bool WindowDescription::IsChildWindow() const
{
	return m_bChildWindow;
}

void WindowDescription::SetChildWindow(bool value /*= true*/)
{
	m_bChildWindow = value;
}

void WindowDescription::SetOwnerHandle(HWND handle)
{
	m_handleOwner = handle;
}

HWND WindowDescription::GetOwnerHandle() const
{
	return m_handleOwner;
}
}