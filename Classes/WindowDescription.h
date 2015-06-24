#pragma once
#include "WidgetUI.h"
#include "Geometry.h"


namespace wui{

class WidgetHostWindow;

class WIDGETUI_API WindowDescription
{
public:
	WindowDescription(void);
	WindowDescription(const std::string &strTitle);
	WindowDescription(const std::string &strTitle, const Rect &position, bool bClientArea);
	WindowDescription(const std::string &strTitle, const Size &size, bool bClientArea);
	WindowDescription(const Rect &position, bool bClientArea);
	virtual ~WindowDescription(void);
	
	const std::string &GetTitle() const;
	bool GetDecorations() const;
	Size GetSize() const;
	Rect GetPosition() const;
	bool GetPositionClientArea() const;
	bool GetAllowResize() const;
	HWND GetHandle() const;
	bool HasCaption() const;
	bool HasSysmenu() const;
	bool HasMinimizeButton() const;
	bool HasMaximizeButton() const;
	bool HasBorder() const;
	bool IsChildWindow() const;
	bool IsToolWindow() const;
	bool IsDialog() const;
	bool IsVisible() const;
	bool IsTopmost() const;
	WidgetHostWindow* GetOwner() const;
	HWND GetOwnerHandle() const;
	
	void ShowCaption(bool value = true);
	void ShowSysmenu(bool value = true);
	void ShowMinimizeButton(bool value = true);
	void ShowMaximizeButton(bool value = true);
	void ShowBorder(bool value = true);
	void SetDecorations(bool decorations);
	void SetVisible(bool value = true);
	void SetTopMost(bool value = true);
	void SetToolWindow(bool value = true);
	void SetDialogWindow(bool value = true);
	void SetChildWindow(bool value = true);
	void SetOwnerWindow(WidgetHostWindow* pOwner);
	void SetOwnerHandle(HWND handle);
	void SetTitle(const std::string& strTitle);
	void SetSize(const Size& size, bool bClientArea);
	void SetPosition(const Rect& position, bool bClientArea);
	void SetAllowResize(bool bAllow);
	void SetHandle(HWND handle);

private:
	void _Init();

private:
	std::string m_strTitle;
	Rect m_position;
	bool m_bPositionClientArea;
	bool m_bDecorations;
	bool m_bCaption;
	bool m_bMinimizeButton;
	bool m_bMaximizeButton;
	bool m_bSysmenu;
	bool m_bBorder;
	bool m_bTopmost;
	bool m_bVisible;
	bool m_bToolWindow;
	bool m_bDialogWindow;
	bool m_bChildWindow;
	bool m_bAllowResize;
	WidgetHostWindow* m_pOwner;
	HWND m_handle;
	HWND m_handleOwner;
};
}
