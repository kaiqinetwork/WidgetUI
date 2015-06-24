#include "StdAfx.h"
#include "Panel.h"
#include <boost/bind.hpp>
#include <xImage/ximage.h>

namespace wui{

Panel::Panel(Widget *pParent)
: Widget(pParent), m_pImageBorderTL(NULL), m_pImageBorderTM(NULL), m_pImageBorderTR(NULL), 
m_pImageBorderL(NULL), m_pImageBorderR(NULL), m_pImageBorderBL(NULL), m_pImageBorderBM(NULL), 
m_pImageBorderBR(NULL), m_pImageBackCenter(NULL)
{
	sigRender().connect(boost::bind(&Panel::OnRender, this, _1, _2));
}

Panel::~Panel(void)
{
}

Size Panel::GetPreferredSize()
{
	return Size(0,0);
}

void Panel::SetBkCenterImage(CxImage* pImage)
{
	m_pImageBackCenter = pImage;
}

void Panel::SetBorderTLImage(CxImage* pImage)
{
	m_pImageBorderTL = pImage;
}

void Panel::SetBorderTMImage(CxImage* pImage)
{
	m_pImageBorderTM = pImage;
}

void Panel::SetBorderTRImage(CxImage* pImage)
{
	m_pImageBorderTR = pImage;
}

void Panel::SetBorderLImage(CxImage* pImage)
{
	m_pImageBorderL = pImage;
}

void Panel::SetBorderRImage(CxImage* pImage)
{
	m_pImageBorderR = pImage;
}

void Panel::SetBorderBLImage(CxImage* pImage)
{
	m_pImageBorderBL = pImage;
}

void Panel::SetBorderBMImage(CxImage* pImage)
{
	m_pImageBorderBM = pImage;
}

void Panel::SetBorderBRImage(CxImage* pImage)
{
	m_pImageBorderBR = pImage;
}

void Panel::OnRender(XGraphics& graphic, const Rect &rcUpdate)
{
	Rect rect(Point(0,0), GetSize());

	if (m_pImageBackCenter && m_pImageBackCenter->IsValid())
		graphic.StretchImage(m_pImageBackCenter, rect);

	int x, y, m, w;

	// »æÖÆ×ó±ß
	if (m_pImageBorderL && m_pImageBorderL->IsValid())
	{
		x = 0;
		y = m_pImageBorderTL->IsValid() ? m_pImageBorderTL->GetHeight() : 0;
		m = rect.GetBottom() - (m_pImageBorderBL ? m_pImageBorderBL->GetHeight() : 0);
		w = m_pImageBorderL->GetHeight();
		while (y < m)
		{
			graphic.BltImage(m_pImageBorderL, x, y);
			y += w;
		}
	}

	// »æÖÆÓÒ±ß
	if (m_pImageBorderR && m_pImageBorderR->IsValid())
	{
		x = rect.GetRight() - m_pImageBorderR->GetWidth();
		y = m_pImageBorderTL ? m_pImageBorderTL->GetHeight() : 0;
		m = rect.GetBottom() - (m_pImageBorderBR ? m_pImageBorderBR->GetHeight() : 0);
		w = m_pImageBorderR->GetHeight();
		while (y < m)
		{
			graphic.BltImage(m_pImageBorderR, x, y);
			y += w;
		}
	}

	// »æÖÆ×óÏÂ½Ç
	if (m_pImageBorderBL && m_pImageBorderBL->IsValid())
	{
		x = 0;
		y = rect.GetBottom() - m_pImageBorderBL->GetHeight();
		graphic.DrawImage(m_pImageBorderBL, x, y, -1, -1, NULL);
	}

	// »æÖÆÏÂ±ß
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

	// »æÖÆÓÒÏÂ½Ç
	if (m_pImageBorderBR && m_pImageBorderBR->IsValid())
	{
		x = m;
		y = rect.GetBottom() - m_pImageBorderBR->GetHeight();
		graphic.DrawImage(m_pImageBorderBR, x, y, -1, -1, NULL);
	}

	// »æÖÆ×ó±ß
	if (m_pImageBorderTL && m_pImageBorderTL->IsValid())
		graphic.DrawImage(m_pImageBorderTL, 0, 0, -1, -1, NULL);

	// »æÖÆÖÐ¼ä
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

	// »æÖÆÓÒ±ß
	if (m_pImageBorderTR && m_pImageBorderTR->IsValid())
	{
		x = m;
		graphic.DrawImage(m_pImageBorderTR, x, 0, -1, -1, NULL);
	}
}
}