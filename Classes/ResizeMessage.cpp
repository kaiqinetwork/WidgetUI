#include "StdAfx.h"
#include "ResizeMessage.h"

namespace wui{

class ResizeMessageData : public MessageData
{
public:
	Rect rect;
};

ResizeMessage::ResizeMessage(void)
{
	SetType("resize");
	SetData("resize", boost::shared_ptr<ResizeMessageData>(new ResizeMessageData));
}

ResizeMessage::ResizeMessage(Message& message)
: Message(message)
{

}

ResizeMessage::~ResizeMessage(void)
{
}

std::string ResizeMessage::GetTypeName()
{
	return "resize";
}

Rect ResizeMessage::GetGeometry() const
{
	boost::shared_ptr<ResizeMessageData> d = boost::dynamic_pointer_cast<ResizeMessageData>(GetData("resize"));
	if (!d)
		return Rect(0, 0, 0, 0);
	return d->rect;
}

void ResizeMessage::SetGeometry(const Rect &rect)
{
	boost::shared_ptr<ResizeMessageData> d = boost::dynamic_pointer_cast<ResizeMessageData>(GetData("resize"));
	if (!d)
	{
		d = boost::shared_ptr<ResizeMessageData>(new ResizeMessageData);
		SetData("resize", d);
	}
	d->rect = rect;
}
}