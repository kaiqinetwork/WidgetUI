#include "StdAfx.h"
#include "InputContext.h"
#include "InputDevice.h"

namespace wui{

InputContext::InputContext(void)
{
}

InputContext::~InputContext(void)
{
}

int InputContext::GetKeyboardCount() const
{
	return m_keyboards.size();
}

int InputContext::GetMouseCount() const
{
	return m_mice.size();
}

InputDevice* InputContext::GetKeyboard(int keyboard /*= 0*/)
{
	return m_keyboards[keyboard];
}

InputDevice* InputContext::GetMouse(int mouse /*= 0*/)
{
	return m_mice[mouse];
}

InputDevice* InputContext::GetDevice(const std::string &strDeviceName)
{
	size_t i;
	for (i = 0; i < m_keyboards.size(); ++i)
		if (m_keyboards[i]->GetDeviceName() == strDeviceName)
			return m_keyboards[i];

	for (i = 0; i < m_mice.size(); ++i)
		if (m_mice[i]->GetDeviceName() == strDeviceName)
			return m_mice[i];
	
	return NULL;
}

void InputContext::ReceivedEvent(const InputEvent &e, InputDevice* pInputDevice)
{
	m_events.push_back(
		std::pair<InputEvent, InputDevice*>(
		e,
		pInputDevice));
}

void InputContext::AddKeyboard(InputDevice* keyboard)
{
	m_keyboards.push_back(keyboard);
	keyboard->m_InputContexts.push_back(this);
}

void InputContext::AddMouse(InputDevice* mouse)
{
	m_mice.push_back(mouse);
	mouse->m_InputContexts.push_back(this);
}

void InputContext::ProcessMessages()
{
	std::vector<std::pair<InputEvent, InputDevice*>>::size_type pos, size;
	size = m_events.size();
	for (pos = 0; pos < size; pos++)
	{
		InputEvent event = m_events[pos].first;
		if (m_events[pos].second == NULL)
			continue;
		event.m_pDevice = m_events[pos].second;

		switch (event.m_type)
		{
		case InputEvent::PRESSED:
			event.m_pDevice->sigKeyDown()(event, m_inputState);
			break;
		case InputEvent::RELEASED:
			event.m_pDevice->sigKeyUp()(event, m_inputState);
			break;
		case InputEvent::DOUBLECLICK:
			event.m_pDevice->sigKeyDblclk()(event, m_inputState);
			break;
		case InputEvent::POINTER_MOVED:
			event.m_pDevice->sigPointerMove()(event, m_inputState);
			break;
		case InputEvent::BALL_MOVED:
			event.m_pDevice->sigBallMove()(event, m_inputState);
			break;
		default:	// Added to stop the compiler warning about "no_key" not handled in switch
			break;
		}
	}
	m_events.clear();
}
}