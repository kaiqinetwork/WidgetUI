#pragma once

#include "Message.h"

namespace wui{
class WIDGETUI_API FocusChangeMessage : public Message
{
public:
	FocusChangeMessage(void);
	FocusChangeMessage(Message& message);
	~FocusChangeMessage(void);
	
	static std::string GetTypeName();
	
	enum FocusType
	{
		LOSING_FOCUS,
		GAINED_FOCUS
	};
	
	FocusType GetFocusType() const;
	void SetFocusType(FocusType focusType);
};
}
