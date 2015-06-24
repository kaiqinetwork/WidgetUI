// Types.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "Types.h"

// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

#ifndef __ATLTYPES_INL__
#define __ATLTYPES_INL__

#pragma once

#ifndef GET_X_LPARAM
#define GET_X_LPARAM(lp)                        ((int)(short)LOWORD(lp))
#endif
#ifndef GET_Y_LPARAM
#define GET_Y_LPARAM(lp)                        ((int)(short)HIWORD(lp))
#endif

#ifndef __ATLTYPES_H__
#error atltypes.inl requires atltypes.h to be included first
#endif	// __ATLTYPES_H__

// CSize
ATLTYPES_INLINE Size::Size() throw()
{ /* random filled */ }
ATLTYPES_INLINE Size::Size(int initCX, int initCY) throw()
{ cx = initCX; cy = initCY; }
ATLTYPES_INLINE Size::Size(SIZE initSize) throw()
{ *(SIZE*)this = initSize; }
ATLTYPES_INLINE Size::Size(POINT initPt) throw()
{ *(POINT*)this = initPt; }
ATLTYPES_INLINE Size::Size(DWORD dwSize) throw()
{
	cx = (short)LOWORD(dwSize);
	cy = (short)HIWORD(dwSize);
}
ATLTYPES_INLINE BOOL Size::operator==(SIZE size) const throw()
{ return (cx == size.cx && cy == size.cy); }
ATLTYPES_INLINE BOOL Size::operator!=(SIZE size) const throw()
{ return (cx != size.cx || cy != size.cy); }
ATLTYPES_INLINE void Size::operator+=(SIZE size) throw()
{ cx += size.cx; cy += size.cy; }
ATLTYPES_INLINE void Size::operator-=(SIZE size) throw()
{ cx -= size.cx; cy -= size.cy; }
ATLTYPES_INLINE void Size::SetSize(int CX, int CY) throw()
{ cx = CX; cy = CY; }	
ATLTYPES_INLINE Size Size::operator+(SIZE size) const throw()
{ return Size(cx + size.cx, cy + size.cy); }
ATLTYPES_INLINE Size Size::operator-(SIZE size) const throw()
{ return Size(cx - size.cx, cy - size.cy); }
ATLTYPES_INLINE Size Size::operator-() const throw()
{ return Size(-cx, -cy); }
ATLTYPES_INLINE Point Size::operator+(POINT point) const throw()
{ return Point(cx + point.x, cy + point.y); }
ATLTYPES_INLINE Point Size::operator-(POINT point) const throw()
{ return Point(cx - point.x, cy - point.y); }
ATLTYPES_INLINE Rect Size::operator+(const RECT* lpRect) const throw()
{ return Rect(lpRect) + *this; }
ATLTYPES_INLINE Rect Size::operator-(const RECT* lpRect) const throw()
{ return Rect(lpRect) - *this; }

// CPoint
ATLTYPES_INLINE Point::Point() throw()
{ /* random filled */ }
ATLTYPES_INLINE Point::Point(int initX, int initY) throw()
{ x = initX; y = initY; }
ATLTYPES_INLINE Point::Point(POINT initPt) throw()
{ *(POINT*)this = initPt; }
ATLTYPES_INLINE Point::Point(SIZE initSize) throw()
{ *(SIZE*)this = initSize; }
ATLTYPES_INLINE Point::Point(LPARAM dwPoint) throw()
{
	x = (short)GET_X_LPARAM(dwPoint);
	y = (short)GET_Y_LPARAM(dwPoint);
}
ATLTYPES_INLINE void Point::Offset(int xOffset, int yOffset) throw()
{ x += xOffset; y += yOffset; }
ATLTYPES_INLINE void Point::Offset(POINT point) throw()
{ x += point.x; y += point.y; }
ATLTYPES_INLINE void Point::Offset(SIZE size) throw()
{ x += size.cx; y += size.cy; }
ATLTYPES_INLINE void Point::SetPoint(int X, int Y) throw()
{ x = X; y = Y; }
ATLTYPES_INLINE BOOL Point::operator==(POINT point) const throw()
{ return (x == point.x && y == point.y); }
ATLTYPES_INLINE BOOL Point::operator!=(POINT point) const throw()
{ return (x != point.x || y != point.y); }
ATLTYPES_INLINE void Point::operator+=(SIZE size) throw()
{ x += size.cx; y += size.cy; }
ATLTYPES_INLINE void Point::operator-=(SIZE size) throw()
{ x -= size.cx; y -= size.cy; }
ATLTYPES_INLINE void Point::operator+=(POINT point) throw()
{ x += point.x; y += point.y; }
ATLTYPES_INLINE void Point::operator-=(POINT point) throw()
{ x -= point.x; y -= point.y; }
ATLTYPES_INLINE Point Point::operator+(SIZE size) const throw()
{ return Point(x + size.cx, y + size.cy); }
ATLTYPES_INLINE Point Point::operator-(SIZE size) const throw()
{ return Point(x - size.cx, y - size.cy); }
ATLTYPES_INLINE Point Point::operator-() const throw()
{ return Point(-x, -y); }
ATLTYPES_INLINE Point Point::operator+(POINT point) const throw()
{ return Point(x + point.x, y + point.y); }
ATLTYPES_INLINE Size Point::operator-(POINT point) const throw()
{ return Size(x - point.x, y - point.y); }
ATLTYPES_INLINE Rect Point::operator+(const RECT* lpRect) const throw()
{ return Rect(lpRect) + *this; }
ATLTYPES_INLINE Rect Point::operator-(const RECT* lpRect) const throw()
{ return Rect(lpRect) - *this; }

// CRect
ATLTYPES_INLINE Rect::Rect() throw()
{ /* random filled */ }
ATLTYPES_INLINE Rect::Rect(int l, int t, int r, int b) throw()
{ left = l; top = t; right = r; bottom = b; }
ATLTYPES_INLINE Rect::Rect(const RECT& srcRect) throw()
{ ::CopyRect(this, &srcRect); }
ATLTYPES_INLINE Rect::Rect(LPCRECT lpSrcRect) throw()
{ ::CopyRect(this, lpSrcRect); }
ATLTYPES_INLINE Rect::Rect(POINT point, SIZE size) throw()
{ right = (left = point.x) + size.cx; bottom = (top = point.y) + size.cy; }
ATLTYPES_INLINE Rect::Rect(POINT topLeft, POINT bottomRight) throw()
{ left = topLeft.x; top = topLeft.y;
right = bottomRight.x; bottom = bottomRight.y; }
ATLTYPES_INLINE int Rect::Width() const throw()
{ return right - left; }
ATLTYPES_INLINE int Rect::Height() const throw()
{ return bottom - top; }
ATLTYPES_INLINE Size Rect::Size() const throw()
{ return Size(right - left, bottom - top); }
ATLTYPES_INLINE Point& Rect::TopLeft() throw()
{ return *((Point*)this); }
ATLTYPES_INLINE Point& Rect::BottomRight() throw()
{ return *((Point*)this+1); }
ATLTYPES_INLINE const Point& Rect::TopLeft() const throw()
{ return *((Point*)this); }
ATLTYPES_INLINE const Point& Rect::BottomRight() const throw()
{ return *((Point*)this+1); }
ATLTYPES_INLINE Point Rect::CenterPoint() const throw()
{ return Point((left+right)/2, (top+bottom)/2); }
ATLTYPES_INLINE void Rect::SwapLeftRight() throw()
{ SwapLeftRight(LPRECT(this)); }
ATLTYPES_INLINE void WINAPI Rect::SwapLeftRight(LPRECT lpRect) throw()
{ LONG temp = lpRect->left; lpRect->left = lpRect->right; lpRect->right = temp; }
ATLTYPES_INLINE Rect::operator LPRECT() throw()
{ return this; }
ATLTYPES_INLINE Rect::operator LPCRECT() const throw()
{ return this; }
ATLTYPES_INLINE BOOL Rect::IsRectEmpty() const throw()
{ return ::IsRectEmpty(this); }
ATLTYPES_INLINE BOOL Rect::IsRectNull() const throw()
{ return (left == 0 && right == 0 && top == 0 && bottom == 0); }
ATLTYPES_INLINE BOOL Rect::PtInRect(POINT point) const throw()
{ return ::PtInRect(this, point); }
ATLTYPES_INLINE void Rect::SetRect(int x1, int y1, int x2, int y2) throw()
{ ::SetRect(this, x1, y1, x2, y2); }
ATLTYPES_INLINE void Rect::SetRect(POINT topLeft, POINT bottomRight) throw()
{ ::SetRect(this, topLeft.x, topLeft.y, bottomRight.x, bottomRight.y); }
ATLTYPES_INLINE void Rect::SetRectEmpty() throw()
{ ::SetRectEmpty(this); }
ATLTYPES_INLINE void Rect::CopyRect(LPCRECT lpSrcRect) throw()
{ ::CopyRect(this, lpSrcRect); }
ATLTYPES_INLINE BOOL Rect::EqualRect(LPCRECT lpRect) const throw()
{ return ::EqualRect(this, lpRect); }
ATLTYPES_INLINE void Rect::InflateRect(int x, int y) throw()
{ ::InflateRect(this, x, y); }
ATLTYPES_INLINE void Rect::InflateRect(SIZE size) throw()
{ ::InflateRect(this, size.cx, size.cy); }
ATLTYPES_INLINE void Rect::DeflateRect(int x, int y) throw()
{ ::InflateRect(this, -x, -y); }
ATLTYPES_INLINE void Rect::DeflateRect(SIZE size) throw()
{ ::InflateRect(this, -size.cx, -size.cy); }
ATLTYPES_INLINE void Rect::OffsetRect(int x, int y) throw()
{ ::OffsetRect(this, x, y); }
ATLTYPES_INLINE void Rect::OffsetRect(POINT point) throw()
{ ::OffsetRect(this, point.x, point.y); }
ATLTYPES_INLINE void Rect::OffsetRect(SIZE size) throw()
{ ::OffsetRect(this, size.cx, size.cy); }
ATLTYPES_INLINE void Rect::MoveToY(int y) throw()
{ bottom = Height() + y; top = y; }
ATLTYPES_INLINE void Rect::MoveToX(int x) throw()
{ right = Width() + x; left = x; }
ATLTYPES_INLINE void Rect::MoveToXY(int x, int y) throw()
{ MoveToX(x); MoveToY(y); }
ATLTYPES_INLINE void Rect::MoveToXY(POINT pt) throw()
{ MoveToX(pt.x); MoveToY(pt.y); }
ATLTYPES_INLINE BOOL Rect::IntersectRect(LPCRECT lpRect1, LPCRECT lpRect2) throw()
{ return ::IntersectRect(this, lpRect1, lpRect2);}
ATLTYPES_INLINE BOOL Rect::UnionRect(LPCRECT lpRect1, LPCRECT lpRect2) throw()
{ return ::UnionRect(this, lpRect1, lpRect2); }
ATLTYPES_INLINE void Rect::operator=(const RECT& srcRect) throw()
{ ::CopyRect(this, &srcRect); }
ATLTYPES_INLINE BOOL Rect::operator==(const RECT& rect) const throw()
{ return ::EqualRect(this, &rect); }
ATLTYPES_INLINE BOOL Rect::operator!=(const RECT& rect) const throw()
{ return !::EqualRect(this, &rect); }
ATLTYPES_INLINE void Rect::operator+=(POINT point) throw()
{ ::OffsetRect(this, point.x, point.y); }
ATLTYPES_INLINE void Rect::operator+=(SIZE size) throw()
{ ::OffsetRect(this, size.cx, size.cy); }
ATLTYPES_INLINE void Rect::operator+=(LPCRECT lpRect) throw()
{ InflateRect(lpRect); }
ATLTYPES_INLINE void Rect::operator-=(POINT point) throw()
{ ::OffsetRect(this, -point.x, -point.y); }
ATLTYPES_INLINE void Rect::operator-=(SIZE size) throw()
{ ::OffsetRect(this, -size.cx, -size.cy); }
ATLTYPES_INLINE void Rect::operator-=(LPCRECT lpRect) throw()
{ DeflateRect(lpRect); }
ATLTYPES_INLINE void Rect::operator&=(const RECT& rect) throw()
{ ::IntersectRect(this, this, &rect); }
ATLTYPES_INLINE void Rect::operator|=(const RECT& rect) throw()
{ ::UnionRect(this, this, &rect); }
ATLTYPES_INLINE Rect Rect::operator+(POINT pt) const throw()
{ Rect rect(*this); ::OffsetRect(&rect, pt.x, pt.y); return rect; }
ATLTYPES_INLINE Rect Rect::operator-(POINT pt) const throw()
{ Rect rect(*this); ::OffsetRect(&rect, -pt.x, -pt.y); return rect; }
ATLTYPES_INLINE Rect Rect::operator+(SIZE size) const throw()
{ Rect rect(*this); ::OffsetRect(&rect, size.cx, size.cy); return rect; }
ATLTYPES_INLINE Rect Rect::operator-(SIZE size) const throw()
{ Rect rect(*this); ::OffsetRect(&rect, -size.cx, -size.cy); return rect; }
ATLTYPES_INLINE Rect Rect::operator+(LPCRECT lpRect) const throw()
{ Rect rect(this); rect.InflateRect(lpRect); return rect; }
ATLTYPES_INLINE Rect Rect::operator-(LPCRECT lpRect) const throw()
{ Rect rect(this); rect.DeflateRect(lpRect); return rect; }
ATLTYPES_INLINE Rect Rect::operator&(const RECT& rect2) const throw()
{ Rect rect; ::IntersectRect(&rect, this, &rect2);
return rect; }
ATLTYPES_INLINE Rect Rect::operator|(const RECT& rect2) const throw()
{ Rect rect; ::UnionRect(&rect, this, &rect2);
return rect; }
ATLTYPES_INLINE BOOL Rect::SubtractRect(LPCRECT lpRectSrc1, LPCRECT lpRectSrc2) throw()
{ return ::SubtractRect(this, lpRectSrc1, lpRectSrc2); }

ATLTYPES_INLINE void Rect::NormalizeRect() throw()
{
	int nTemp;
	if (left > right)
	{
		nTemp = left;
		left = right;
		right = nTemp;
	}
	if (top > bottom)
	{
		nTemp = top;
		top = bottom;
		bottom = nTemp;
	}
}

ATLTYPES_INLINE void Rect::InflateRect(LPCRECT lpRect) throw()
{
	left -= lpRect->left;		top -= lpRect->top;
	right += lpRect->right;		bottom += lpRect->bottom;
}

ATLTYPES_INLINE void Rect::InflateRect(int l, int t, int r, int b) throw()
{
	left -= l;			top -= t;
	right += r;			bottom += b;
}

ATLTYPES_INLINE void Rect::DeflateRect(LPCRECT lpRect) throw()
{
	left += lpRect->left;	top += lpRect->top;
	right -= lpRect->right;	bottom -= lpRect->bottom;
}

ATLTYPES_INLINE void Rect::DeflateRect(int l, int t, int r, int b) throw()
{
	left += l;		top += t;
	right -= r;		bottom -= b;
}

ATLTYPES_INLINE Rect Rect::MulDiv(int nMultiplier, int nDivisor) const throw()
{
	return Rect(
		::MulDiv(left, nMultiplier, nDivisor),
		::MulDiv(top, nMultiplier, nDivisor),
		::MulDiv(right, nMultiplier, nDivisor),
		::MulDiv(bottom, nMultiplier, nDivisor));
}


#endif	// __ATLTYPES_INL__

