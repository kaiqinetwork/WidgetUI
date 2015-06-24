#pragma once

#include "InputState.h"
#include "InputEvent.h"

namespace wui{

class InputDevice;

class InputContext
{
public:
	InputContext(void);
	~InputContext(void);
	
	int GetKeyboardCount() const;
	int GetMouseCount() const;
	InputDevice* GetKeyboard(int keyboard = 0);
	InputDevice* GetMouse(int mouse = 0);
	InputDevice* GetDevice(const std::string &strDeviceName);
	
	void AddKeyboard(InputDevice* keyboard);
	void AddMouse(InputDevice* mouse);
	
	void ReceivedEvent(const InputEvent &e, InputDevice* pInputDevice);
	
	void ProcessMessages();
	
private:
	std::vector<InputDevice*> m_keyboards;
	std::vector<InputDevice*> m_mice;
	InputState m_inputState;
	std::vector<std::pair<InputEvent, InputDevice*>> m_events;
};
}
