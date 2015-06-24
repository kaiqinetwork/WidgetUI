#ifndef WIDGETUI_WIDGETGRAPHIC_H
#define WIDGETUI_WIDGETGRAPHIC_H

#pragma once 

#include "WidgetUI.h"
#include "Geometry.h"

class CxImage;

namespace wui{

class WIDGETUI_API XGraphics
{
public:
	XGraphics(const Size& size);
	virtual ~XGraphics();
	
	HDC GetDC();
	void Resize(const Size& size);
	
	int GetWidth() const;
	int GetHeight() const;

	void PushModleView(const Rect& rect);
	void PopModleView();

	void FillSolidRect(int x, int y, int cx, int cy, COLORREF clr);
	void FillSolidRect(HDC hdc, LPCRECT lpRect, COLORREF clr);
	
	HFONT SelectFont(HFONT pFont);
	
	void SetBkMode(int nBkMode);
	void SetTextColor(COLORREF clr);
	void DrawText(LPCTSTR lpszString, int nCount, LPRECT lpRect, UINT nFormat);
	void DrawText(const std::string& str, LPRECT lpRect, UINT nFormat);
	
	void BltImage(CxImage* pImage, int x, int y);
	void StretchImage(CxImage* pImage, const Rect& rect);
	void DrawImage(CxImage* pImage, int x, int y, int cx, int cy, LPRECT lpClipRect, bool bSmooth = false);
	void DrawImage(CxImage* pImage, LPRECT lpRect, LPRECT lpClipRect, bool bSmooth = false);
	
protected:
	HDC m_dc;
	HBITMAP m_bitmap;     // Bitmap in memory device context
	HBITMAP m_pOldBitmap;
	Size m_size;
	std::deque<Rect> m_modelviews;
	
};
}

#endif //__WIDGETDC_H__
