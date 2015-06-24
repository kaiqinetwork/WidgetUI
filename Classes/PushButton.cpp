#include "StdAfx.h"
#include "PushButton.h"
#include "InputMessage.h"
#include "PointerMessage.h"
#include "Keys.h"
#include <boost/bind.hpp>
#include <xImage/ximage.h>
//#include <SkinCtrls/PPTooltip.h>

namespace wui{

PushButton::PushButton(Widget* pParent)
: Widget(pParent), m_pBkImage(NULL), m_bPressed(false), m_bHot(false), m_nBkImageType(BTN_IMAGE_1STATE), 
m_bUseHandCursor(false), m_pFont(NULL), m_clrText(RGB(0,0,0)), /*m_pTooltip(NULL),*/ m_bTooltipShowed(false)
{
	SetFocusPolicy(FOCUS_LOCAL);
	
	sigProcessMessage().connect(boost::bind(&PushButton::OnProcessMessage, this, _1));
	sigRender().connect(boost::bind(&PushButton::OnRender, this, _1, _2));
	sigEnableModeChanged().connect(boost::bind(&PushButton::OnEnableModeChanged, this));
}

PushButton::~PushButton(void)
{
// 	if (m_bTooltipShowed && m_pTooltip)
// 	{
// 		m_pTooltip->HideTooltip();
// 	}
}

bool PushButton::IsPushed() const
{
	return m_bPressed;
}

const std::string & PushButton::GetText() const
{
	return m_strText;
}

CxImage* PushButton::GetBkImage() const
{
	return m_pBkImage;
}

void PushButton::SetPushed(bool enable)
{
	if (m_bPressed != enable)
	{
		m_bPressed = enable;
		RequestRepaint();
	}
}

void PushButton::SetText(const char* lpszText)
{
	m_strText = lpszText;
	RequestRepaint();
}

void PushButton::SetBkImage(CxImage* pImage, int nImageType /* = BTN_IMAGE_1STATE */)
{
	m_pBkImage = pImage;
	m_nBkImageType = min(nImageType, BTN_IMAGE_5STATE);
	m_nBkImageType = max(nImageType, BTN_IMAGE_1STATE);
	RequestRepaint();
}

boost::signal<void ()>& PushButton::sigClicked()
{
	return m_sigClicked;
}

void PushButton::OnRender(XGraphics& graphic, const Rect& rcUpdate)
{
	Size size = GetSize();
	if (m_pBkImage)
	{
		Size sizePerferred = GetPreferredSize();
		Rect rcClip(0, 0, sizePerferred.width, sizePerferred.height);
		int nPos = 0;
		if (m_nBkImageType != BTN_IMAGE_1STATE)
		{
			switch (m_nBkImageType)
			{
			case BTN_IMAGE_3STATE:
				if (m_bPressed) nPos = 2;
				else if (m_bHot) nPos = 1;
				break;
			case BTN_IMAGE_4STATE:
				if (!IsEnabled()) nPos = 3;
				else if (m_bPressed) nPos = 2;
				else if (m_bHot) nPos = 1;
				break;
			case BTN_IMAGE_5STATE:
				if (!IsEnabled()) nPos = 4;
				else if (HasFocus()) nPos = 2;
				else if (m_bPressed) nPos = 3;
				else if (m_bHot) nPos = 1;
				break;
			default:
				break;
			}			
		}
		LPRECT rect;
		rect = &rcClip.GetRectStruct();
		graphic.DrawImage(m_pBkImage, -nPos * sizePerferred.width, 0, m_pBkImage->GetWidth(), m_pBkImage->GetHeight(), rect);
	}
	
	if (!m_strText.empty())
	{
		HFONT pOldFont = NULL;
		if (m_pFont)
			pOldFont = graphic.SelectFont(m_pFont);
		graphic.SetBkMode(TRANSPARENT);
		graphic.SetTextColor(m_clrText);
		RECT rc = Rect(Point(0,0),size).GetRectStruct();
		graphic.DrawText(m_strText, &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		if (pOldFont)
			graphic.SelectFont(pOldFont);
	}
}

void PushButton::OnProcessMessage(Message& msg)
{
	if (!IsEnabled())
		return;

	if (msg.IsType(InputMessage::GetTypeName()))
	{
		InputMessage input_msg = msg;
		InputEvent e = input_msg.GetEvent();

		if (e.m_type == InputEvent::PRESSED && 
			(e.m_id == WUI_MOUSE_LEFT || e.m_id == WUI_KEY_RETURN || e.m_id == WUI_KEY_SPACE || e.m_id == WUI_KEY_NUMPAD_ENTER))
		{
			m_bPressed = true;
			RequestRepaint();
			msg.SetConsumed();
		}
		else if( e.m_type == InputEvent::RELEASED &&
			(e.m_id == WUI_MOUSE_LEFT || e.m_id == WUI_KEY_RETURN || e.m_id == WUI_KEY_SPACE || e.m_id == WUI_KEY_NUMPAD_ENTER) &&
			m_bPressed )
		{
			m_bPressed = false;
			RequestRepaint();
			msg.SetConsumed();

			if (!m_sigClicked.empty())
				m_sigClicked();
		}
		if (e.m_type == InputEvent::PRESSED && (e.m_id == WUI_KEY_LEFT || e.m_id == WUI_KEY_UP))
		{
			FocusPrevious();
			msg.SetConsumed();
		}
		else if (e.m_type == InputEvent::PRESSED && (e.m_id == WUI_KEY_RIGHT || e.m_id == WUI_KEY_DOWN))
		{
			FocusNext();
			msg.SetConsumed();
		}
	}
	else if (msg.IsType(PointerMessage::GetTypeName()))
	{
		PointerMessage pointer = msg;
		if (pointer.GetPointerType() == PointerMessage::POINTER_ENTER)
		{
// 			if (m_pTooltip && !m_strTooltipText.empty())
// 			{
// 				m_bTooltipShowed = true;
// 				
// 				POINT pt;
// 				GetCursorPos(&pt);
// 				pt.x += 6;
// 				pt.y += 6;
// 				m_pTooltip->ShowHelpTooltip(&pt, m_strTooltipText.c_str(), NULL, TRUE);
// 			}
		
			if (m_bUseHandCursor && IsEnabled())
				SetCursor(::LoadCursor(NULL, IDC_HAND));
			
			m_bHot = true;
			RequestRepaint();
			msg.SetConsumed();
		}
		else if (pointer.GetPointerType() == PointerMessage::POINTER_LEAVE)
		{
// 			if (m_bTooltipShowed && m_pTooltip)
// 			{
// 				m_bTooltipShowed = false;
// 				m_pTooltip->HideTooltip();
// 			}
			
			if (m_bUseHandCursor)
				SetCursor(::LoadCursor(NULL, IDC_ARROW));
			
			m_bHot = false;
			m_bPressed = false;
			RequestRepaint();
			msg.SetConsumed();
		}
	}
}

Size PushButton::GetPreferredSize()
{
	Size size = GetSize();
	if (m_pBkImage)
	{
		int nPart = 1;
		if (m_nBkImageType == BTN_IMAGE_3STATE)
			nPart = 3;
		else if (m_nBkImageType == BTN_IMAGE_4STATE)
			nPart = 4;
		else if (m_nBkImageType == BTN_IMAGE_5STATE)
			nPart = 5;
		size.width = m_pBkImage->GetWidth() / nPart;
		size.height = m_pBkImage->GetHeight();
	}
	return size;
}

void PushButton::SetUseHandCursor(bool bUse)
{
	m_bUseHandCursor = bUse;
}

bool PushButton::IsHot() const
{
	return m_bHot;
}

void PushButton::OnEnableModeChanged()
{
	if (m_bHot && m_bUseHandCursor && !IsEnabled())
		SetCursor(::LoadCursor(NULL, IDC_ARROW));
	m_bHot = false;
	m_bPressed = false;
	RequestRepaint();
}

void PushButton::SetFont(HFONT pFont)
{
	m_pFont = pFont;
	RequestRepaint();
}

void PushButton::SetTextColor(COLORREF clrText)
{
	m_clrText = clrText;
	RequestRepaint();
}

void PushButton::SetTooltipText(LPCTSTR lpszText)
{
	m_strTooltipText = lpszText;
}

// void PushButton::SetTooltip(CPPToolTip* pTooltip)
// {
// 	m_pTooltip = pTooltip;
// }
}