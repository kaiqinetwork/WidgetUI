// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

#ifndef __ATLTYPES_H__
#define __ATLTYPES_H__

#pragma once

/////////////////////////////////////////////////////////////////////////////
// Classes declared in this file

class Size;
class Point;
class Rect;

/////////////////////////////////////////////////////////////////////////////
// CSize - An extent, similar to Windows SIZE structure.

class _declspec(dllexport) Size : public tagSIZE
{
public:

	// Constructors
	// construct an uninitialized size
	Size() throw();
	// create from two integers
	Size(int initCX, int initCY) throw();
	// create from another size
	Size(SIZE initSize) throw();
	// create from a point
	Size(POINT initPt) throw();
	// create from a DWORD: cx = LOWORD(dw) cy = HIWORD(dw)
	Size(DWORD dwSize) throw();

	// Operations
	BOOL operator==(SIZE size) const throw();
	BOOL operator!=(SIZE size) const throw();
	void operator+=(SIZE size) throw();
	void operator-=(SIZE size) throw();
	void SetSize(int CX, int CY) throw();

	// Operators returning CSize values
	Size operator+(SIZE size) const throw();
	Size operator-(SIZE size) const throw();
	Size operator-() const throw();

	// Operators returning CPoint values
	Point operator+(POINT point) const throw();
	Point operator-(POINT point) const throw();

	// Operators returning CRect values
	Rect operator+(const RECT* lpRect) const throw();
	Rect operator-(const RECT* lpRect) const throw();
};

/////////////////////////////////////////////////////////////////////////////
// CPoint - A 2-D point, similar to Windows POINT structure.

class _declspec(dllexport) Point : public tagPOINT
{
public:
	// Constructors

	// create an uninitialized point
	Point() throw();
	// create from two integers
	Point(int initX, int initY) throw();
	// create from another point
	Point(POINT initPt) throw();
	// create from a size
	Point(SIZE initSize) throw();
	// create from an LPARAM: x = LOWORD(dw) y = HIWORD(dw)
	Point(LPARAM dwPoint) throw();


	// Operations

	// translate the point
	void Offset(int xOffset, int yOffset) throw();
	void Offset(POINT point) throw();
	void Offset(SIZE size) throw();
	void SetPoint(int X, int Y) throw();

	BOOL operator==(POINT point) const throw();
	BOOL operator!=(POINT point) const throw();
	void operator+=(SIZE size) throw();
	void operator-=(SIZE size) throw();
	void operator+=(POINT point) throw();
	void operator-=(POINT point) throw();

	// Operators returning CPoint values
	Point operator+(SIZE size) const throw();
	Point operator-(SIZE size) const throw();
	Point operator-() const throw();
	Point operator+(POINT point) const throw();

	// Operators returning CSize values
	Size operator-(POINT point) const throw();

	// Operators returning CRect values
	Rect operator+(const RECT* lpRect) const throw();
	Rect operator-(const RECT* lpRect) const throw();
};

/////////////////////////////////////////////////////////////////////////////
// CRect - A 2-D rectangle, similar to Windows RECT structure.

class _declspec(dllexport) Rect : public tagRECT
{
	// Constructors
public:
	// uninitialized rectangle
	Rect() throw();
	// from left, top, right, and bottom
	Rect(int l, int t, int r, int b) throw();
	// copy constructor
	Rect(const RECT& srcRect) throw();
	// from a pointer to another rect
	Rect(LPCRECT lpSrcRect) throw();
	// from a point and size
	Rect(POINT point, SIZE size) throw();
	// from two points
	Rect(POINT topLeft, POINT bottomRight) throw();

	// Attributes (in addition to RECT members)

	// retrieves the width
	int Width() const throw();
	// returns the height
	int Height() const throw();
	// returns the size
	Size Size() const throw();
	// reference to the top-left point
	Point& TopLeft() throw();
	// reference to the bottom-right point
	Point& BottomRight() throw();
	// const reference to the top-left point
	const Point& TopLeft() const throw();
	// const reference to the bottom-right point
	const Point& BottomRight() const throw();
	// the geometric center point of the rectangle
	Point CenterPoint() const throw();
	// swap the left and right
	void SwapLeftRight() throw();
	static void WINAPI SwapLeftRight(LPRECT lpRect) throw();

	// convert between CRect and LPRECT/LPCRECT (no need for &)
	operator LPRECT() throw();
	operator LPCRECT() const throw();

	// returns TRUE if rectangle has no area
	BOOL IsRectEmpty() const throw();
	// returns TRUE if rectangle is at (0,0) and has no area
	BOOL IsRectNull() const throw();
	// returns TRUE if point is within rectangle
	BOOL PtInRect(POINT point) const throw();

	// Operations

	// set rectangle from left, top, right, and bottom
	void SetRect(int x1, int y1, int x2, int y2) throw();
	void SetRect(POINT topLeft, POINT bottomRight) throw();
	// empty the rectangle
	void SetRectEmpty() throw();
	// copy from another rectangle
	void CopyRect(LPCRECT lpSrcRect) throw();
	// TRUE if exactly the same as another rectangle
	BOOL EqualRect(LPCRECT lpRect) const throw();

	// Inflate rectangle's width and height by
	// x units to the left and right ends of the rectangle
	// and y units to the top and bottom.
	void InflateRect(int x, int y) throw();
	// Inflate rectangle's width and height by
	// size.cx units to the left and right ends of the rectangle
	// and size.cy units to the top and bottom.
	void InflateRect(SIZE size) throw();
	// Inflate rectangle's width and height by moving individual sides.
	// Left side is moved to the left, right side is moved to the right,
	// top is moved up and bottom is moved down.
	void InflateRect(LPCRECT lpRect) throw();
	void InflateRect(int l, int t, int r, int b) throw();

	// deflate the rectangle's width and height without
	// moving its top or left
	void DeflateRect(int x, int y) throw();
	void DeflateRect(SIZE size) throw();
	void DeflateRect(LPCRECT lpRect) throw();
	void DeflateRect(int l, int t, int r, int b) throw();

	// translate the rectangle by moving its top and left
	void OffsetRect(int x, int y) throw();
	void OffsetRect(SIZE size) throw();
	void OffsetRect(POINT point) throw();
	void NormalizeRect() throw();

	// absolute position of rectangle
	void MoveToY(int y) throw();
	void MoveToX(int x) throw();
	void MoveToXY(int x, int y) throw();
	void MoveToXY(POINT point) throw();

	// set this rectangle to intersection of two others
	BOOL IntersectRect(LPCRECT lpRect1, LPCRECT lpRect2) throw();

	// set this rectangle to bounding union of two others
	BOOL UnionRect(LPCRECT lpRect1, LPCRECT lpRect2) throw();

	// set this rectangle to minimum of two others
	BOOL SubtractRect(LPCRECT lpRectSrc1, LPCRECT lpRectSrc2) throw();

	// Additional Operations
	void operator=(const RECT& srcRect) throw();
	BOOL operator==(const RECT& rect) const throw();
	BOOL operator!=(const RECT& rect) const throw();
	void operator+=(POINT point) throw();
	void operator+=(SIZE size) throw();
	void operator+=(LPCRECT lpRect) throw();
	void operator-=(POINT point) throw();
	void operator-=(SIZE size) throw();
	void operator-=(LPCRECT lpRect) throw();
	void operator&=(const RECT& rect) throw();
	void operator|=(const RECT& rect) throw();

	// Operators returning CRect values
	Rect operator+(POINT point) const throw();
	Rect operator-(POINT point) const throw();
	Rect operator+(LPCRECT lpRect) const throw();
	Rect operator+(SIZE size) const throw();
	Rect operator-(SIZE size) const throw();
	Rect operator-(LPCRECT lpRect) const throw();
	Rect operator&(const RECT& rect2) const throw();
	Rect operator|(const RECT& rect2) const throw();
	Rect MulDiv(int nMultiplier, int nDivisor) const throw();
};

#ifndef _DEBUG
#define ATLTYPES_INLINE inline
#include <Types.inl>
#endif


#endif // __ATLTYPES_H__
