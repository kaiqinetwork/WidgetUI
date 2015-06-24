#include "StdAfx.h"
#include "ProgressBar.h"
#include <boost/bind.hpp>
#include <xImage/ximage.h>

namespace wui{

ProgressBar::ProgressBar(Widget* pParent)
: Widget(pParent), m_nMin(0), m_nMax(0), m_nSetpSize(0), m_nPosition(0), 
m_pBkImage(NULL), m_pBarImage(NULL)
{
	sigProcessMessage().connect(boost::bind(&ProgressBar::OnProcessMessage, this, _1));
	sigRender().connect(boost::bind(&ProgressBar::OnRender, this, _1, _2));
}

ProgressBar::~ProgressBar(void)
{
}

int ProgressBar::GetMin() const
{
	return m_nMin;
}

int ProgressBar::GetMax() const
{
	return m_nMax;
}

int ProgressBar::GetStepSize() const
{
	return m_nSetpSize;
}

int ProgressBar::GetPosition() const
{
	return m_nPosition;
}

void ProgressBar::SetMin(int nMin)
{
	m_nMin = nMin;
	CheckRange();
	RequestRepaint();
}

void ProgressBar::SetMax(int nMax)
{
	m_nMax = nMax;
	CheckRange();
	RequestRepaint();
}

void ProgressBar::SetRange(int nMin, int nMax)
{
	m_nMin = nMin;
	m_nMax = nMax;
	CheckRange();
	RequestRepaint();
}

void ProgressBar::SetStepSize(int nSize)
{
	m_nSetpSize = nSize;
	RequestRepaint();
}

void ProgressBar::SetPosition(int nPos)
{
	m_nPosition = nPos;
	CheckRange();
	RequestRepaint();
}

void ProgressBar::AdvancePosition(int nPos)
{
	m_nPosition += nPos;
	CheckRange();
	RequestRepaint();
}

void ProgressBar::StepPosition()
{
	m_nPosition += m_nSetpSize;
	CheckRange();
	RequestRepaint();
}

void ProgressBar::OnProcessMessage(Message &msg)
{

}

void ProgressBar::OnRender(XGraphics &graphic, const Rect &rcUpdate)
{
	Rect rect(Point(0,0), GetSize());
	Rect rcClip(0,0,0,0);
	int x, w, m;
	
	if (m_pBkImage && m_pBkImage->IsValid())
	{
		w = m_pBkImage->GetWidth() / 3;
		
		rcClip.SetRect(0, 0, w, m_pBkImage->GetHeight());
		RECT rc = rcClip.GetRectStruct();
		graphic.DrawImage(m_pBkImage, 0, 0, -1, -1, &rc);
		
		m = rect.GetRight() - w;
		for (x = w; x < m ; x += w)
		{
			rcClip.SetRect(x, 0, x + w, m_pBkImage->GetHeight());
			rc = rcClip.GetRectStruct();
			graphic.DrawImage(m_pBkImage, x - w, 0, -1, -1, &rc);
		}
		
		rcClip.SetRect(m, 0, m + w, m_pBkImage->GetHeight());
		rc = rcClip.GetRectStruct();
		graphic.DrawImage(m_pBkImage, m - 2 * w, 0, -1, -1, &rc);
	}
	
	if (m_pBarImage && m_pBarImage->IsValid() && m_nMax != m_nMin && m_nPosition > m_nMin)
	{
		w = m_pBarImage->GetWidth() / 3;
		int nPos = 2 * w + (rect.GetRight() - 2 * w) * (m_nPosition - m_nMin) / (m_nMax - m_nMin);

		rcClip.SetRect(0, 0, w, m_pBarImage->GetHeight());
		RECT rc = rcClip.GetRectStruct();
		graphic.DrawImage(m_pBarImage, 0, 0, -1, -1, &rc);

		m = nPos - w;
		for (x = w; x < m ; x += w)
		{
			rcClip.SetRect(x, 0, x + w, m_pBarImage->GetHeight());
			rc = rcClip.GetRectStruct();
			graphic.DrawImage(m_pBarImage, x - w, 0, -1, -1, &rc);
		}

		rcClip.SetRect(m, 0, m + w, m_pBarImage->GetHeight());
		rc = rcClip.GetRectStruct();
		graphic.DrawImage(m_pBarImage, m - 2 * w, 0, -1, -1, &rc);
	}
}

void ProgressBar::SetBkImage(CxImage* pImage)
{
	m_pBkImage = pImage;
	RequestRepaint();
}

void ProgressBar::SetBarImage(CxImage* pImage)
{
	m_pBarImage = pImage;
	RequestRepaint();
}

void ProgressBar::CheckRange()
{
	if(m_nPosition < m_nMin)
		m_nPosition = m_nMin;
	if(m_nPosition > m_nMax)
		m_nPosition = m_nMax;
}
}