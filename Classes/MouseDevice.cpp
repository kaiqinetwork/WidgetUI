#include "StdAfx.h"
#include "MouseDevice.h"
#include "WidgetHostWindow.h"

namespace wui{

MouseDevice::MouseDevice(WidgetHostWindow* pWindow)
: m_pWindow(pWindow)
{
	for (int i=0; i<32; i++) m_keyStates[i] = false;
}

MouseDevice::~MouseDevice(void)
{
}

std::string MouseDevice::GetName() const
{
	return "System Mouse";
}

std::string MouseDevice::GetDeviceName() const
{
	return "System Mouse";
}

InputDevice::Type MouseDevice::GetType() const
{
	return InputDevice::POINTER;
}

std::string MouseDevice::GetKeyName(int id) const
{
	switch (id)
	{
	case 0: return "Mouse left";
	case 1: return "Mouse right";
	case 2: return "Mouse middle";
	case 3: return "Mouse wheel up";
	case 4: return "Mouse wheel down";
	}

	std::string str;
	//str.Format(_T("Mouse button %d"), id);
	char buf[100];
	sprintf(buf,"%s  %d","key_",id);
	str = buf;
	return str;
}

bool MouseDevice::GetKeycode(int nKeyCode) const
{
	if (nKeyCode < 0 || nKeyCode >= 32) return false;
	return m_keyStates[nKeyCode];
}

void MouseDevice::SetPosition(int x, int y)
{
	POINT pt;
	pt.x = x;
	pt.y = y;

	ClientToScreen(m_pWindow->GetHwnd(), &pt);
	SetCursorPos(pt.x, pt.y);
}

int MouseDevice::GetX() const
{
	POINT ptCursor;
	GetCursorPos(&ptCursor);

	BOOL res = ScreenToClient(m_pWindow->GetHwnd(), &ptCursor);
	if (res == FALSE) return 0;

	return ptCursor.x;
}

int MouseDevice::GetY() const
{
	POINT ptCursor;
	GetCursorPos(&ptCursor);

	BOOL res = ScreenToClient(m_pWindow->GetHwnd(), &ptCursor);
	if (res == FALSE) return 0;

	return ptCursor.y;
}

void MouseDevice::SetKeyState(int id, bool value)
{
	m_keyStates[id] = value;
}
}