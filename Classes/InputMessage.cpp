#include "StdAfx.h"
#include "InputMessage.h"

namespace wui{

class InputMessageData : public MessageData
{
public:
	InputEvent input_event;
};


InputMessage::InputMessage(void)
{
	SetType("input");
	SetData("input", boost::shared_ptr<InputMessageData>(new InputMessageData));
}

InputMessage::InputMessage(Message& message)
: Message(message)
{
	
}

InputMessage::~InputMessage(void)
{
}

std::string InputMessage::GetTypeName()
{
	return "input";
}

InputEvent InputMessage::GetEvent() const
{
	boost::shared_ptr<InputMessageData> d = boost::dynamic_pointer_cast<InputMessageData>(GetData("input"));
	if (!d)
		return InputEvent();
	return d->input_event;
}

void InputMessage::SetEvent(const InputEvent &event)
{
	boost::shared_ptr<InputMessageData> d = boost::dynamic_pointer_cast<InputMessageData>(GetData("input"));
	if (!d)
	{
		d = boost::shared_ptr<InputMessageData>(new InputMessageData);
		SetData("input", d);
	}
	d->input_event = event;
}
}