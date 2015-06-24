#pragma once

#include "InputDevice.h"
namespace wui{

class WidgetHostWindow;

class KeyboardDevice : public InputDevice
{
public:
	KeyboardDevice(WidgetHostWindow* pWindow);
	~KeyboardDevice(void);
	
	std::string GetName() const;
	std::string GetDeviceName() const;
	Type GetType() const;
	std::string GetKeyName(int id) const;
	bool GetKeycode(int nKeyCode) const;
	
private:
	WidgetHostWindow* m_pWindow;
};

}