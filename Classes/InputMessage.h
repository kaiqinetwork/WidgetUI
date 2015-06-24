#pragma once

#include "Message.h"
#include "InputEvent.h"

namespace wui{

class WIDGETUI_API InputMessage : public Message
{
public:
	InputMessage(void);
	InputMessage(Message& message);
	~InputMessage(void);
	
	static std::string GetTypeName();

	InputEvent GetEvent() const;
	void SetEvent(const InputEvent &event);
};
}
