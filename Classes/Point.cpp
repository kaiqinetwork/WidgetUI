#include "StdAfx.h"
#include "Point.h"
#include "Geometry.h"

namespace wui{

Point::Point(void)
{
}

Point::~Point(void)
{
}

Point::Point( int xx, int yy )
{
	x = xx; y = yy;
}

Point::Point( const Point& point )
{
	x = point.x; y= point.y;
}

void Point::Offset( int xOffset, int yOffset )
{
	x += xOffset; y += yOffset;
}

void Point::Offset( const Point& point )
{
	x += point.x; y += point.y;
}

void Point::Offset( const Size& size )
{
	x += size.width; y += size.height; 
}

void Point::SetPoint( int xx, int yy )
{
	x = xx; y = yy;
}

bool Point::operator==( const Point& point ) const
{
	return (x == point.x && y == point.y);
}

bool Point::operator!=( const Point& point ) const
{
	return (x != point.x || y != point.y); 
}

void Point::operator+=( const Size& size )
{
	x += size.width; y += size.height; 
}

void Point::operator+=( const Point& point )
{
	x += point.x; y += point.y;
}
void Point::operator-=( const Size& size )
{
	x -= size.width; y -= size.height; 
}

void Point::operator-=( const Point& point )
{
	x -= point.x; y -= point.y;
}

Point Point::operator+( const Size& size ) const
{
	return Point(x + size.width, y + size.height);
}

Point Point::operator+( const Point& point ) const
{
	return Point(x + point.x, y + point.y);
}

Rect Point::operator+( const Rect& rect ) const
{
	return Rect(rect.x +x, rect.y +y, rect.width +x, rect.height +y);
}
Point Point::operator-( const Size& size ) const
{
	return Point(x - size.width, y - size.height); 
}

Point Point::operator-() const
{
	return Point(-x, -y);
}

Size Point::operator-( const Point& point ) const
{
	return Size(x - point.x, y - point.y);
}

Rect Point::operator-( const Rect& rect ) const
{
	return Rect(rect.x -x, rect.y -y, rect.width -x, rect.height -y); 
}

POINT Point::GetStructPoint()
{
	POINT pt;
	pt.x = x;
	pt.y = y;
	return pt;
}
}