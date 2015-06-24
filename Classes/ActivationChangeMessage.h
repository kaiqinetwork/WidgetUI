#pragma once

#include "Message.h"

namespace wui{
class WIDGETUI_API ActivationChangeMessage : public Message
{
public:
	ActivationChangeMessage(void);
	ActivationChangeMessage(Message& message);
	~ActivationChangeMessage(void);
	
	static std::string GetTypeName();

	enum ActivationType
	{
		ACTIVATION_LOST,
		ACTIVATION_GAINED
	};

	ActivationType GetActivationType() const;
	void SetActivationType(ActivationType type);
};

}
