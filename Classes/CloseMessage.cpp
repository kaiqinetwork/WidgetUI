#include "StdAfx.h"
#include "CloseMessage.h"

namespace wui{
class CloseMessageData : public MessageData
{
public:
};

CloseMessage::CloseMessage(void)
{
	SetType("window close");
	SetData("window close", boost::shared_ptr<CloseMessageData>(new CloseMessageData));
}

CloseMessage::CloseMessage(Message& message)
{

}
CloseMessage::~CloseMessage(void)
{
}

std::string CloseMessage::GetTypeName()
{
	return "window close";
}
}