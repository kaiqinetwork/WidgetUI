#pragma once

#include "WidgetUI.h"


namespace wui{

class InputDevice;

class WIDGETUI_API InputEvent
{
public:
	InputEvent(void);
	~InputEvent(void);
	
	enum Type
	{
		NO_KEY            = 0,
		PRESSED           = 1,
		RELEASED          = 2,
		DOUBLECLICK       = 3,
		POINTER_MOVED     = 4,
		BALL_MOVED        = 6,
	};
	
public:
	int m_id;
	std::string m_str;
	Type m_type;
	Point m_ptMouse;
	double m_axisPos;
	int m_nRepeatCount;
	InputDevice* m_pDevice;

	bool m_alt;
	bool m_shift;
	bool m_ctrl;
};
}