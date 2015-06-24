#pragma once

#include "Message.h"

namespace wui{

class WIDGETUI_API CloseMessage : public Message
{
public:
	CloseMessage(void);
	CloseMessage(Message& message);
	~CloseMessage(void);
	
	static std::string GetTypeName();
};

}