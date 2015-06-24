#include "StdAfx.h"
#include "Geometry.h"
#include "Point.h"

namespace wui {
Size::Size(void)
{
}

Size::Size(int initCX, int initCY)
{ 
	width = initCX; height = initCY; 
}

Size::Size(const Size& size)
{
	*(Size*)this = size;
}

Size::Size(const Point& point)
{ 
	*(Point*)this = point;
}

Size::~Size(void)
{
}

bool Size::operator==( const Size& size )
{
	 return (width == size.width && height == size.height); 
}

bool Size::operator!=( const Size& size )
{
	return (width != size.width || height != size.height); 
}

void Size::operator+=( const Size& size )
{
	height += size.height;
	width += size.width;
}

void Size::operator-=( const Size& size )
{
	width -= size.width;
	height -= size.height;
}

Size& Size::operator=( const Size& size )
{
	width = size.width;
	height = size.height;
	return *this;
}

Size& Size::operator=( const Point& point )
{
	width = point.x;
	height = point.y;
	return *this;
}

Size& Size::operator+( const Size& right )
{
	width += right.width;
	height+= right.height;
	 return *this;
}

Size& Size::operator-( const Size& right )
{
	width -= right.width;
	height -= right.height;
	return *this;
}

void Size::SetSize( int w, int h )
{
	height = h;
	width = w;
}

Rect::Rect()
{
}

Rect::Rect( int xx, int yy, int width, int height )
{
	x = xx;
	y = yy;
	this -> width = width;
	this -> height = height;
}

Rect::Rect( const Point& origin, const Size& size )
{
	x = origin.x;
	y = origin.y;
	width = size.width;
	height = size.height;
}

Rect::Rect( const Rect& other )
{
	x = other.x;
	y = other.y;
	width = other.width;
	height = other.height;
}

int Rect::GetLeft() const
{
	return x;
}

int Rect::GetTop() const
{
	return y;
}

int Rect::GetRight() const
{
	return x+ width;
}

int Rect::GetBottom() const
{
	return y+ height;
}

bool Rect::Equals( const Rect & rect ) const
{
	return (this->height == rect.height && this->width == rect.width);
}

bool Rect::Contains( int xx, int yy ) const
{
	return (xx >= this->x && xx < this->GetRight() &&
				yy >= this->y && yy < this->GetBottom());
}

bool Rect::Contains( const Point& point ) const
{
	return (point.x >= this->x && point.x < this->GetRight() &&
		point.y >= this->y && point.y < this->GetBottom());
}

void Rect::Inflate( int dx, int dy )
{
	height -= dy;
	width += dx;
}

void Rect::Inflate( const Point& point )
{
	height -= point.y;
	width += point.x;
}

bool Rect::Intersects( const Rect& rect ) const
{
	return(abs((x+ width /2) - (rect.x+ rect.width /2)) < (width /2+ rect.width/2) && 
				abs((y+ height /2) - (rect.y+ rect.height /2)) < (height /2+ rect.height/2));
}

bool Rect::Union( const Rect& rect ) const
{
	return((Contains(rect.x,rect.y) && Contains(rect.GetRight(),rect.GetBottom())) || 
				(rect.Contains(x, y) && rect.Contains(GetRight(), GetBottom()) ) || (rect.x == x && rect.y == y));
}

void Rect::Offset( int dx, int dy )
{
	x += dx;
	y += dy;
}

void Rect::Offset( const Point& point )
{
	x += point.x;
	y += point.y;
}

bool Rect::RectIsEmpty() const
{
	return(height == 0 && width == 0 );
}

void Rect::SetRect( int x1, int y1, int x2, int y2 )
{
	x = x1;
	y = y1;
	width = x2 - x1;
	height = y2 - y1;
}

void Rect::SetRect( POINT topLeft, POINT bottomRight )
{
	x = topLeft.x;
	y = topLeft.y;
	width = bottomRight.x - topLeft.x;
	height = bottomRight.y - topLeft.y;
}

void Rect::SetRectEmpty()
{
	x = 0; y = 0; height = 0; width = 0;
}

RECT Rect::GetRectStruct() const
{
	RECT rect;
	rect.top = y;
	rect.bottom = y + height;
	rect.left = x;
	rect.right = x + width;
	return rect;
}
}
