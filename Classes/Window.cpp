#include "StdAfx.h"
#include "Window.h"
#include <boost/bind.hpp>
#include "Manager.h"
#include "WindowManager.h"
#include "WidgetHostWindow.h"
#include "InputMessage.h"
#include "ActivationChangeMessage.h"
#include "InputEvent.h"
#include "CloseMessage.h"
#include "Keys.h"
#include <xImage/ximage.h>

namespace wui{

Window::Window(Widget *pOwner, const TopLevelDescription &description)
: Widget(pOwner, description), m_bDragStart(false), m_bCloseBtnPressed(false), 
	m_bCloseBtnHot(false), m_nCaptionHeight(0), m_pImageBtnClose(NULL), m_pImageBorderTL(NULL), 
	m_pImageBorderTM(NULL), m_pImageBorderTR(NULL), m_pImageBorderBL(NULL), m_pImageBorderBM(NULL), 
	m_pImageBorderBR(NULL), m_pImageBorderL(NULL), m_pImageBorderR(NULL), m_pImageBackCenter(NULL), 
	m_bShowTitle(true), m_pFont(NULL), m_TextColor(RGB(5,5,5)), m_bDraggable(true)
{
	m_rcCloseButton.SetRectEmpty();
	m_lastMousePos.x = 0;
	m_lastMousePos.y = 0;
	
	m_strTitle = description.GetTitle();

	if (description.IsChildWindow())
		m_bDrawDecorations = false;
	else
		m_bDrawDecorations = description.GetDecorations();

	sigProcessMessage().connect(boost::bind(&Window::OnProcessMessage, this, _1));
	sigRender().connect(boost::bind(&Window::OnRender, this, _1, _2));
	sigStyleChanged().connect(boost::bind(&Window::OnStyleChanged, this));
	sigResized().connect(boost::bind(&Window::OnResized, this));
}

Window::Window(Manager *pManager, const TopLevelDescription &description)
: Widget(pManager, description), m_bDragStart(false), m_bCloseBtnPressed(false), 
m_bCloseBtnHot(false), m_nCaptionHeight(0), m_pImageBtnClose(NULL), m_pImageBorderTL(NULL), 
m_pImageBorderTM(NULL), m_pImageBorderTR(NULL), m_pImageBorderBL(NULL), m_pImageBorderBM(NULL), 
m_pImageBorderBR(NULL), m_pImageBorderL(NULL), m_pImageBorderR(NULL), m_pImageBackCenter(NULL), 
m_bShowTitle(true), m_pFont(NULL), m_TextColor(RGB(5,5,5))
{
	m_rcCloseButton.SetRectEmpty();
	m_lastMousePos.x = 0;
	m_lastMousePos.y = 0;
	
	m_strTitle = description.GetTitle();

	if (description.IsChildWindow())
		m_bDrawDecorations = false;
	else
		m_bDrawDecorations = description.GetDecorations();

	sigProcessMessage().connect(boost::bind(&Window::OnProcessMessage, this, _1));
	sigRender().connect(boost::bind(&Window::OnRender, this, _1, _2));
	sigStyleChanged().connect(boost::bind(&Window::OnStyleChanged, this));
	sigResized().connect(boost::bind(&Window::OnResized, this));
}

Window::~Window(void)
{
}

std::string Window::GetTitle() const
{
	return m_strTitle;
}

bool Window::IsMinimized() const
{
	const Widget *pRootWidget = GetTopLevelWidget();

	for (std::vector<TopLevelWindow *>::const_iterator iter = Widget::m_pManager->m_rootWidgets.begin(); iter != Widget::m_pManager->m_rootWidgets.end(); ++iter)
	{
		TopLevelWindow *cur = *iter;
		if (cur->pWidget == pRootWidget)
			return m_pManager->m_pWindowManager->IsMinimized(cur);
	}

	return false;
}

bool Window::IsMaximized() const
{
	const Widget *pRootWidget = GetTopLevelWidget();

	for (std::vector<TopLevelWindow *>::const_iterator iter = m_pManager->m_rootWidgets.begin(); iter != m_pManager->m_rootWidgets.end(); ++iter)
	{
		TopLevelWindow *cur = *iter;
		if (cur->pWidget == pRootWidget)
			return m_pManager->m_pWindowManager->IsMaximized(cur);
	}

	return false;
}

Size Window::GetPreferredSize()
{
	return Size(0,0);
}

Rect Window::GetClientArea() const
{
	Rect rect(Point(0,0), GetSize());

	return rect;
}

bool Window::GetDraggable() const
{
	return m_bDraggable;
}

void Window::SetTitle(const std::string &str)
{
	m_strTitle = str;
}

void Window::BringToFront()
{
	const Widget *pRootWidget = GetTopLevelWidget();

	for (std::vector<TopLevelWindow *>::const_iterator iter = m_pManager->m_rootWidgets.begin(); iter != m_pManager->m_rootWidgets.end(); ++iter)
	{
		TopLevelWindow *cur = *iter;
		if (cur->pWidget == pRootWidget)
			return m_pManager->m_pWindowManager->BringToFront(cur);
	}
}

void Window::SetDraggable(bool enable /*= true*/)
{
	m_bDraggable = enable;
}

void Window::CheckMoveWindow(Message &msg)
{
	if (m_bDraggable == false)
	{
		m_bDragStart = false;
		return;
	}

	if (msg.IsType(InputMessage::GetTypeName()))
	{
		InputMessage input_msg = msg;
		InputEvent e = input_msg.GetEvent();

		if (e.m_type == InputEvent::PRESSED && e.m_id == WUI_MOUSE_LEFT)
		{
			BringToFront();
			Rect rect(Point(0, 0), GetSize());
			Rect rcCaption(rect.x, rect.y, rect.GetRight(), rect.y + m_nCaptionHeight);
			if (rcCaption.Contains(e.m_ptMouse))
			{
				m_bDragStart = true;
				CaptureMouse(true);
				::GetCursorPos(&m_lastMousePos);
			}
		}
		else if (e.m_type == InputEvent::RELEASED && e.m_id == WUI_MOUSE_LEFT)
		{
			if(m_bDragStart)
			{
				m_bDragStart = false;
				CaptureMouse(false);
			}
		}
		else if (e.m_type == InputEvent::POINTER_MOVED && m_bDragStart == true)
		{
			const Widget *pRootWidget = GetTopLevelWidget();

			std::vector<TopLevelWindow>::size_type pos, size;
			size = m_pManager->m_rootWidgets.size();

			for (pos = 0; pos < size; pos++)
			{
				TopLevelWindow *cur = m_pManager->m_rootWidgets[pos];
				if (cur->pWidget == pRootWidget)
				{
					POINT ptMouse;
					::GetCursorPos(&ptMouse);
					if (ptMouse.x != m_lastMousePos.x || ptMouse.y != m_lastMousePos.y)
					{
						Rect geometry = GetWindowGeometry();
						geometry.Offset(ptMouse.x - m_lastMousePos.x, ptMouse.y - m_lastMousePos.y);
						m_lastMousePos = ptMouse;
						SetWindowGeometry(geometry);
					}
				}
			}
		}
	}
}

void Window::OnProcessMessage(Message &msg)
{
	if (m_bDrawDecorations)
		CheckMoveWindow(msg);

	if (msg.IsType(InputMessage::GetTypeName()))
	{
		InputMessage input_msg = msg;
		InputEvent e = input_msg.GetEvent();

		if (e.m_type == InputEvent::PRESSED && e.m_id == WUI_MOUSE_LEFT)
		{
			if (m_rcCloseButton.Contains(e.m_ptMouse))
				if (m_bCloseBtnPressed == false)
				{
					m_bCloseBtnPressed = true;
					RequestRepaint();
				}
		}
		else if (e.m_type == InputEvent::RELEASED && e.m_id == WUI_MOUSE_LEFT)
		{
			if(m_bDrawDecorations)
			{
				if (m_bCloseBtnPressed)
				{
					RequestRepaint();
					if (!sigClose().empty() && sigClose()())
						msg.SetConsumed();
					m_bCloseBtnPressed = false;
				}
			}
		}
		else if (e.m_type == InputEvent::POINTER_MOVED)
		{
			if(m_bDrawDecorations)
			{
				bool bInside = m_rcCloseButton.Contains(e.m_ptMouse) == TRUE;

				if (m_bCloseBtnHot != bInside)
				{
					m_bCloseBtnHot = bInside;
					RequestRepaint();
				}
			}
		}
	}
	else if (msg.IsType(CloseMessage::GetTypeName()))
	{
		if (!sigClose().empty() && sigClose()())
			msg.SetConsumed();
	}
	else if (msg.IsType(ActivationChangeMessage::GetTypeName()))
	{
		ActivationChangeMessage ac(msg);
		if (ac.GetActivationType() == ActivationChangeMessage::ACTIVATION_GAINED)
		{
			m_bActivated = true;
 			if (!sigActivated().empty() && sigActivated()())
 				msg.SetConsumed();
		}
		else if (ac.GetActivationType() == ActivationChangeMessage::ACTIVATION_LOST)
		{
			m_bActivated = false;
 			if (!sigDeactivated().empty() && sigDeactivated()())
 				msg.SetConsumed();
		}
	}
}

void Window::OnRender(XGraphics& graphic, const Rect &rcUpdate)
{
	Rect rect(Point(0,0), GetSize());

	if (m_pImageBackCenter && m_pImageBackCenter->IsValid())
		graphic.StretchImage(m_pImageBackCenter, rect);

	int x, y, m, w;

	// 绘制左边
	if (m_pImageBorderL && m_pImageBorderL->IsValid())
	{
		x = 0;
		y = m_nCaptionHeight;
		m = rect.GetBottom() - (m_pImageBorderBL ? m_pImageBorderBL->GetHeight() : 0);
		w = m_pImageBorderL->GetHeight();
		while (y < m)
		{
			graphic.BltImage(m_pImageBorderL, x, y);
			y += w;
		}
	}

	// 绘制右边
	if (m_pImageBorderR && m_pImageBorderR->IsValid())
	{
		x = rect.GetRight() - m_pImageBorderR->GetWidth();
		y = m_nCaptionHeight;
		m = rect.GetBottom() - (m_pImageBorderBR ? m_pImageBorderBR->GetHeight() : 0);
		w = m_pImageBorderR->GetHeight();
		while (y < m)
		{
			graphic.BltImage(m_pImageBorderR, x, y);
			y += w;
		}
	}

	// 绘制左下角
	if (m_pImageBorderBL && m_pImageBorderBL->IsValid())
	{
		x = 0;
		y = rect.GetBottom() - m_pImageBorderBL->GetHeight();
		graphic.DrawImage(m_pImageBorderBL, x, y, -1, -1, NULL);
	}

	// 绘制下边
	m = rect.GetRight() - (m_pImageBorderBR ? m_pImageBorderBR->GetWidth() : 0);
	if (m_pImageBorderBM && m_pImageBorderBM->IsValid())
	{
		x = m_pImageBorderBL ? m_pImageBorderBL->GetWidth() : 0;
		y = rect.GetBottom() - m_pImageBorderBM->GetHeight();
		w = m_pImageBorderBM->GetWidth();
		while (x < m)
		{
			graphic.BltImage(m_pImageBorderBM, x, y);
			x += w;
		}
	}

	// 绘制右下角
	if (m_pImageBorderBR && m_pImageBorderBR->IsValid())
	{
		x = m;
		y = rect.GetBottom() - m_pImageBorderBR->GetHeight();
		graphic.DrawImage(m_pImageBorderBR, x, y, -1, -1, NULL);
	}

	RenderCaption(graphic, rcUpdate);	
}

void Window::OnStyleChanged()
{

}

void Window::OnResized()
{

}

void Window::RenderCaption(XGraphics& graphic, const Rect &rcUpdate)
{
	int x, m, w;
	Rect rect(Point(0,0), GetSize());

	// 绘制左边
	if (m_pImageBorderTL && m_pImageBorderTL->IsValid())
		graphic.DrawImage(m_pImageBorderTL, 0, 0, -1, -1, NULL);

	// 绘制中间
	if (m_pImageBorderTM && m_pImageBorderTM->IsValid())
	{
		x = m_pImageBorderTL ? m_pImageBorderTL->GetWidth() : 0;
		m = rect.width - (m_pImageBorderTR ? m_pImageBorderTR->GetWidth() : 0);
		w = m_pImageBorderTM->GetWidth();
		while (x < m)
		{
			graphic.BltImage(m_pImageBorderTM, x, 0);
			x += w;
		}
	}

	// 绘制右边
	if (m_pImageBorderTR && m_pImageBorderTR->IsValid())
	{
		x = m;
		graphic.DrawImage(m_pImageBorderTR, x, 0, -1, -1, NULL);
	}	

	if (m_nCaptionHeight == 0)
		return;

	// 绘画标题
	if (m_bShowTitle)
	{
		Rect rcTitle;
		rcTitle.y = 0;
		rcTitle.x = 5;
		rcTitle.height = m_nCaptionHeight;
		rcTitle.width = rect.width - 5 ;
		RECT rc = rcTitle.GetRectStruct();
		graphic.SetBkMode(TRANSPARENT);
		HFONT pOldFont = NULL;
		if (m_pFont)
			graphic.SelectFont(m_pFont);
		graphic.SetTextColor(m_TextColor);
		graphic.DrawText(m_strTitle, &rc, DT_LEFT | DT_SINGLELINE | DT_VCENTER | DT_END_ELLIPSIS);
		if (pOldFont)
			graphic.SelectFont(pOldFont);
	}	

	// 绘制按钮
	if (m_bDrawDecorations && m_pImageBtnClose)
	{
		x = 0;
		if (m_bCloseBtnHot)
			x = m_pImageBtnClose->GetWidth() / 4;
		else if (m_bCloseBtnPressed)
			x = m_pImageBtnClose->GetWidth() * 2 / 4;
		RECT rc = m_rcCloseButton.GetRectStruct();
		graphic.DrawImage(m_pImageBtnClose, m_rcCloseButton.x - x, 
			m_rcCloseButton.y, m_pImageBtnClose->GetWidth(), m_pImageBtnClose->GetHeight(), &rc);
	}
}

void Window::SetBkCenterImage(CxImage* pImage)
{
	m_pImageBackCenter = pImage;
}

void Window::SetBorderTLImage(CxImage* pImage)
{
	m_pImageBorderTL = pImage;
	m_nCaptionHeight = m_bDrawDecorations && m_pImageBorderTL != NULL ? m_pImageBorderTL->GetHeight() : 0;
}

void Window::SetBorderTMImage(CxImage* pImage)
{
	m_pImageBorderTM = pImage;
}

void Window::SetBorderTRImage(CxImage* pImage)
{
	m_pImageBorderTR = pImage;
}

void Window::SetBorderLImage(CxImage* pImage)
{
	m_pImageBorderL = pImage;
}

void Window::SetBorderRImage(CxImage* pImage)
{
	m_pImageBorderR = pImage;
}

void Window::SetBorderBLImage(CxImage* pImage)
{
	m_pImageBorderBL = pImage;
}

void Window::SetBorderBMImage(CxImage* pImage)
{
	m_pImageBorderBM = pImage;
}

void Window::SetBorderBRImage(CxImage* pImage)
{
	m_pImageBorderBR = pImage;
}

void Window::SetCloseBtnImage(CxImage* pImage)
{
	m_pImageBtnClose = pImage;
}

void Window::SetCloseBtnRect( Rect rc )
{
	m_rcCloseButton = rc;
}
}