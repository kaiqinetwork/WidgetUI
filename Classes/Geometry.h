#pragma once

#include "WidgetUI.h"
#include "Point.h"

namespace wui {

class Point;
class Size;
class Rect;


class WIDGETUI_API Size
{
public:
	int width;
	int height;

public:
	Size(void);
	Size(int w, int h);
	Size(const Size& size);
	Size(const Point& point);
	virtual ~Size(void);

	bool operator==(const Size& size);
	bool operator!=(const Size& size);
	void operator+=(const Size& size);
	void operator-=(const Size& size);

	Size& operator=(const Size& size);
	Size& operator=(const Point& point);
	Size& operator+(const Size& right);
	Size& operator-(const Size& right);

	void SetSize(int w, int h);
};

class WIDGETUI_API Rect
{
public:
	int x;
	int y;
	int width;
	int height;
	
public:
	Rect();
	Rect(int xx, int yy, int width, int height);
	Rect(const Point& origin, const Size& size);
	Rect(const Rect& other);
	
	
	int GetLeft() const;
	int GetTop() const;
	int GetRight() const;
	int GetBottom() const;

	bool RectIsEmpty() const;
	bool Equals(const Rect & rect) const;
	bool Contains(int xx, int yy) const;
	bool Contains(const Point& point) const;
	void Inflate(int dx, int dy);
	void Inflate(const Point& point);
	bool Intersects(const Rect& rect) const;
	bool Union(const Rect& rect) const;
	void Offset(int dx, int dy);
	void Offset(const Point& point);
	void SetRect(int x1, int y1, int x2, int y2);
	void SetRect(POINT topLeft, POINT bottomRight);
	void SetRectEmpty();
	RECT GetRectStruct() const;
};

}


