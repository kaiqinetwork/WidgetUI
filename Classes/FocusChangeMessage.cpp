#include "StdAfx.h"
#include "FocusChangeMessage.h"

namespace wui{
class FocusChangeMessageData : public MessageData
{
public:
	FocusChangeMessage::FocusType focus_type;
};

FocusChangeMessage::FocusChangeMessage(void)
{
	SetType("focus change");
	SetData("focus change", boost::shared_ptr<FocusChangeMessageData>(new FocusChangeMessageData));
}

FocusChangeMessage::FocusChangeMessage(Message& message)
: Message(message)
{

}

FocusChangeMessage::~FocusChangeMessage(void)
{
}

std::string FocusChangeMessage::GetTypeName()
{
	return "focus change";
}

FocusChangeMessage::FocusType FocusChangeMessage::GetFocusType() const
{
	boost::shared_ptr<FocusChangeMessageData> d = boost::dynamic_pointer_cast<FocusChangeMessageData>(GetData("focus change"));
	if (!d)
		return LOSING_FOCUS;
	return d->focus_type;
}

void FocusChangeMessage::SetFocusType(FocusType focusType)
{
	boost::shared_ptr<FocusChangeMessageData> d = boost::dynamic_pointer_cast<FocusChangeMessageData>(GetData("focus change"));
	if (!d)
	{
		d = boost::shared_ptr<FocusChangeMessageData>(
			new FocusChangeMessageData);
		SetData("focus change", d);
	}
	d->focus_type = focusType;
}
}