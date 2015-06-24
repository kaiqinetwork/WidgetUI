#pragma once

#include "message.h"
#include "Geometry.h"

namespace wui{
class WIDGETUI_API ResizeMessage : public Message
{
public:
	ResizeMessage(void);
	ResizeMessage(Message& message);
	~ResizeMessage(void);
	
	static std::string GetTypeName();

	Rect GetGeometry() const;
	void SetGeometry(const Rect &rect);
};
}