#pragma once

#include "InputDevice.h"

namespace wui{

class WidgetHostWindow;

class MouseDevice : public InputDevice
{
public:
	MouseDevice(WidgetHostWindow* pWindow);
	~MouseDevice(void);
	
	std::string GetName() const;
	std::string GetDeviceName() const;
	Type GetType() const;
	std::string GetKeyName(int id) const;
	bool GetKeycode(int nKeyCode) const;
	int GetX() const;
	int GetY() const;
	
	void SetPosition(int x, int y);
	void SetKeyState(int id, bool value);
	
private:
	WidgetHostWindow* m_pWindow;
	
	bool m_keyStates[32];
};

}