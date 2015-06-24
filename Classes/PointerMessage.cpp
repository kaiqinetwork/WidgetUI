#include "StdAfx.h"
#include "PointerMessage.h"

namespace wui{

class PointerMessageData : public MessageData
{
public:
	PointerMessage::PointerType pointer_type;
};

PointerMessage::PointerMessage(void)
{
	SetType("pointer");
	SetData("pointer", boost::shared_ptr<PointerMessageData>(new PointerMessageData));
}

PointerMessage::PointerMessage(Message& message)
: Message(message)
{

}

PointerMessage::~PointerMessage(void)
{
}

std::string PointerMessage::GetTypeName()
{
	return "pointer";
}

PointerMessage::PointerType PointerMessage::GetPointerType() const
{
	boost::shared_ptr<PointerMessageData> d = boost::dynamic_pointer_cast<PointerMessageData>(GetData("pointer"));
	if (!d)
		return POINTER_ENTER;
	return d->pointer_type;
}

void PointerMessage::SetPointerType(PointerType type)
{
	boost::shared_ptr<PointerMessageData> d = boost::dynamic_pointer_cast<PointerMessageData>(GetData("pointer"));
	if (!d)
	{
		d = boost::shared_ptr<PointerMessageData>(
			new PointerMessageData);
		SetData("pointer", d);
	}
	d->pointer_type = type;
}
}