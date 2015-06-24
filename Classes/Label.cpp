#include "StdAfx.h"
#include "Label.h"
#include <boost/bind.hpp>
#include <xImage/ximage.h>

namespace wui{

Label::Label(Widget* pParent)
: Widget(pParent), m_pFont(NULL), m_clrText(0), m_pBkImage(NULL), m_alignment(ALIGN_LEFT)
{
	sigProcessMessage().connect(boost::bind(&Label::OnProcessMessage, this, _1));
	sigRender().connect(boost::bind(&Label::OnRender, this, _1, _2));
}

Label::~Label(void)
{
}

Label::Alignment Label::GetAlignment() const
{
	return m_alignment;
}

std::string Label::GetText() const
{
	return m_strText;
}

void Label::SetText(const std::string& text)
{
	m_strText = text;
	RequestRepaint();
}

void Label::SetAlignment(Alignment align)
{
	if (m_alignment != align)
	{
		m_alignment = align;
		RequestRepaint();
	}
}

void Label::OnProcessMessage(Message &msg)
{

}

void Label::OnRender(XGraphics &graphic, const Rect &rcUpdate)
{
	Rect rect(Point(0,0), GetSize());
	
	if (m_pBkImage && m_pBkImage->IsValid())
	{
		graphic.DrawImage(m_pBkImage, 0, 0, m_pBkImage->GetWidth(), m_pBkImage->GetHeight(), NULL);
	}
	
	int nFormat = DT_WORDBREAK | DT_EDITCONTROL;
	switch (m_alignment)
	{
	case ALIGN_LEFT:
		nFormat |= DT_LEFT;
		break;
	case ALIGN_CENTER:
		nFormat |= DT_CENTER;
		break;
	case ALIGN_RIGHT:
		nFormat |= DT_RIGHT;
		break;
	}
	HFONT pOldFont = NULL;
	if (m_pFont)
		pOldFont = graphic.SelectFont(m_pFont);
	graphic.SetBkMode(TRANSPARENT);
	graphic.SetTextColor(m_clrText);
	RECT rc = rect.GetRectStruct();
	graphic.DrawText(m_strText, &rc, nFormat);
	if (pOldFont)
		graphic.SelectFont(pOldFont);
}

Size Label::GetPreferredSize()
{
	XGraphics& graphic = GetGraphic();
	Rect rect(0,0,0,0);
	
	int nFormat = 0;
	switch (m_alignment)
	{
	case ALIGN_LEFT:
		nFormat |= DT_LEFT;
		break;
	case ALIGN_CENTER:
		nFormat |= DT_CENTER;
		break;
	case ALIGN_RIGHT:
		nFormat |= DT_RIGHT;
		break;
	}
	HFONT pOldFont = NULL;
	if (m_pFont)
		pOldFont = graphic.SelectFont(m_pFont);
	RECT rc = rect.GetRectStruct();
	graphic.DrawText(m_strText, &rc, nFormat | DT_CALCRECT);
	if (pOldFont)
		graphic.SelectFont(pOldFont);
	return Size(rect.width, rect.height);
}

void Label::SetFont(HFONT pFont)
{
	m_pFont = pFont;
	RequestRepaint();
}

void Label::SetTextColor(COLORREF clrText)
{
	m_clrText = clrText;
	RequestRepaint();
}

void Label::SetBkImage(CxImage* pImage)
{
	m_pBkImage = pImage;
	RequestRepaint();
}
}