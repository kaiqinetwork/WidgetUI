#include "StdAfx.h"
#include "KeyboardDevice.h"
#include "WidgetHostWindow.h"

namespace wui{

KeyboardDevice::KeyboardDevice(WidgetHostWindow* pWindow)
: m_pWindow(pWindow)
{
}

KeyboardDevice::~KeyboardDevice(void)
{
}

std::string KeyboardDevice::GetName() const
{
	return "System Keyboard";
}

std::string KeyboardDevice::GetDeviceName() const
{
	return "System Keyboard";
}

InputDevice::Type KeyboardDevice::GetType() const
{
	return InputDevice::KEYBOARD;
}

std::string KeyboardDevice::GetKeyName(int id) const
{
	char name[1024];
	UINT scancode = MapVirtualKey(id, 0);
	int length = GetKeyNameText(scancode << 16, name, 1024);
	return std::string(name, length);
}

bool KeyboardDevice::GetKeycode(int nKeyCode) const
{
	// Ignore all key events when we don't have focus
	if (!m_pWindow->HasFocus())
		return false;

	return (GetKeyState(nKeyCode) & 0xfe) != 0;
}
}