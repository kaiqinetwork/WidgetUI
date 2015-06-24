#pragma once

#include "Message.h"

namespace wui{

class WIDGETUI_API PointerMessage : public Message
{
public:
	PointerMessage(void);
	PointerMessage(Message& message);
	~PointerMessage(void);
	
	static std::string GetTypeName();

	enum PointerType
	{
		POINTER_ENTER,
		POINTER_LEAVE
	};

	PointerType GetPointerType() const;

	void SetPointerType(PointerType type);
};
}