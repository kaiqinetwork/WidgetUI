#include "stdafx.h"
#include "Graphic.h"
#include <xImage/ximage.h>

namespace wui{

XGraphics::XGraphics(const Size& size)
: m_pOldBitmap(NULL)
{	
	Resize(size);
}

XGraphics::~XGraphics()
{
// 	if (m_pOldBitmap)
// 		m_dc->SelectObject(m_pOldBitmap);
}

void XGraphics::FillSolidRect(int x, int y, int cx, int cy, COLORREF clr)
{
	RECT rc = Rect(x,y,x+cx,y+cy).GetRectStruct();
	FillSolidRect(m_dc,&rc, clr);
}

void XGraphics::FillSolidRect(HDC hdc, LPCRECT lpRect, COLORREF clr)
{
	Rect rcDest(lpRect->left, lpRect->top, 
			lpRect->right- lpRect->left, lpRect->bottom- lpRect->top);
	if (!m_modelviews.empty())
	{
		Rect rcModleView = m_modelviews.back();
		rcDest.Offset(rcModleView.x,rcModleView.y);
		rcDest.Intersects(rcModleView);
	}
	RECT rc = rcDest.GetRectStruct();
	HBRUSH hbru = CreateSolidBrush(clr);
	::FillRect(hdc,&rc, hbru);
	::DeleteObject(hbru);
}

void XGraphics::DrawText(LPCTSTR lpszString, int nCount, LPRECT lpRect, UINT nFormat)
{
	Rect rcDest(lpRect->left, lpRect->top, 
		lpRect->right- lpRect->left, lpRect->bottom- lpRect->top);
	if (!m_modelviews.empty())
	{
		Rect rcModleView = m_modelviews.back();
		rcDest.Offset(rcModleView.x,rcModleView.y);
	}
	RECT rc = rcDest.GetRectStruct();
	::DrawText(m_dc, lpszString, nCount, &rc, nFormat);
	if (nFormat & DT_CALCRECT)
	{
		if (!m_modelviews.empty())
		{
			Rect rcModleView = m_modelviews.back();
			rcDest.Offset(-rcModleView.GetLeft(), -rcModleView.GetTop());
		}
		RECT rc = rcDest.GetRectStruct();
		*lpRect = rc;
	}
}

void XGraphics::DrawText(const std::string& str, LPRECT lpRect, UINT nFormat)
{
	DrawText((LPCTSTR)str.c_str(), str.size(), lpRect, nFormat);
}

void XGraphics::DrawImage(CxImage* pImage, int x, int y, int cx, int cy, LPRECT lpClipRect, bool bSmooth /* = false */)
{
	if (cx == -1)
		cx = pImage->GetWidth();
	if (cy == -1)
		cy = pImage->GetHeight();
	RECT rc = Rect(x, y, cx, cy).GetRectStruct();
	DrawImage(pImage, &rc, lpClipRect, bSmooth);
}

void XGraphics::DrawImage(CxImage* pImage, LPRECT lpRect, LPRECT lpClipRect, bool bSmooth /* = false */)
{
	if (!pImage->IsValid())
		return;
		
	Rect rcDest(lpRect->left, lpRect->top, 
		lpRect->right- lpRect->left, lpRect->bottom- lpRect->top);
	Rect rcClip(rcDest);
	if (lpClipRect)
		rcClip = Rect(lpClipRect->left, lpClipRect->top,
			lpClipRect->right - lpClipRect->left, lpClipRect->bottom - lpClipRect->top);
	if (!m_modelviews.empty())
	{
		Rect rcModleView = m_modelviews.back();
		rcDest.Offset(rcModleView.x,rcModleView.y);
		rcClip.Offset(rcModleView.x,rcModleView.y);
		rcClip.Intersects(rcModleView);
	}
	RECT rcCl = rcClip.GetRectStruct();
	RECT rcDe = rcDest.GetRectStruct();
	pImage->Draw(m_dc , rcDe, &rcCl, bSmooth);
}

void XGraphics::BltImage(CxImage* pImage, int x, int y)
{
	if (!pImage->IsValid())
		return;
		
	Point pt(x, y);
	if (!m_modelviews.empty())
	{
		Rect rcModleView = m_modelviews.back();
		pt.Offset(rcModleView.x,rcModleView.y);
	}
	pImage->Blt(m_dc , pt.x, pt.y);
}

HFONT XGraphics::SelectFont(HFONT pFont)
{
	return (HFONT)SelectObject(m_dc,pFont);
}

void XGraphics::Resize(const Size& size)
{
// 	if (m_pOldBitmap)
// 		m_dc->SelectObject(m_pOldBitmap);
	
	HDC dcScreen;
	dcScreen = CreateDC(_T("DISPLAY"), NULL, NULL, NULL);
	DeleteDC(m_dc);
	m_dc = CreateCompatibleDC(dcScreen);
	DeleteObject(m_bitmap);
	m_bitmap = (HBITMAP)::CreateCompatibleBitmap(dcScreen, size.width, size.height);
//	m_pOldBitmap = m_dc->SelectObject(&m_bitmap);
	::SelectObject(m_dc,m_bitmap);
	
	m_size = size;
}

HDC XGraphics::GetDC()
{
	return m_dc;
}

int XGraphics::GetWidth() const
{
	return m_size.width;
}

int XGraphics::GetHeight() const
{
	return m_size.height;
}

void XGraphics::PushModleView(const Rect& rect)
{
	if (m_modelviews.empty())
		m_modelviews.push_back(rect);
	else
	{
		Rect rc(rect);
		rc.Offset(m_modelviews.back().x,m_modelviews.back().y);
		m_modelviews.push_back(rc);
	}	
}

void XGraphics::PopModleView()
{
	m_modelviews.pop_back();
}

void XGraphics::StretchImage(CxImage* pImage, const Rect& rect)
{
	if (!pImage->IsValid())
		return;
		
	Rect rcDest(rect);
	if (!m_modelviews.empty())
	{
		Rect rcModleView = m_modelviews.back();
		rcDest.Offset(rcModleView.x,rcModleView.y);
	}
	pImage->Stretch(m_dc, rcDest.GetRectStruct());
}

void XGraphics::SetBkMode(int nBkMode)
{
	::SetBkMode(m_dc,nBkMode);
}

void XGraphics::SetTextColor(COLORREF clr)
{
	::SetTextColor(m_dc,clr);
}
}