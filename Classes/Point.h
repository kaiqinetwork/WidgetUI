#pragma once

#include "WidgetUI.h"
#include "Geometry.h"

namespace wui {

class Point;
class Size;
class Rect;

class WIDGETUI_API Point
{
public:
	int x;
	int y;
	
public:
	Point(void);
	Point(int xx, int yy);
	Point(const Point& point);
	virtual ~Point(void);
	
	void Offset(int xOffset, int yOffset);
	void Offset(const Point& point);
	void Offset(const Size& size);
	void SetPoint(int xx, int yy);
	POINT GetStructPoint();
	
	bool operator==(const Point& point) const;
	bool operator!=(const Point& point) const;
	void operator+=(const Size& size);
	void operator-=(const Size& size);
	void operator+=(const Point& point);
	void operator-=(const Point& point);
	
	// Operators returning Point values
	Point operator+(const Size& size) const;
	Point operator-(const Size& size) const;
	Point operator-() const;
	Point operator+(const Point& point) const;

	// Operators returning CSize values
	Size operator-(const Point& point) const;

	// Operators returning CRect values
	Rect operator+(const Rect& rect) const;
	Rect operator-(const Rect& rect) const;
};

}


