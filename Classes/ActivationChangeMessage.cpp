#include "StdAfx.h"
#include "ActivationChangeMessage.h"

namespace wui{
class ActivationChangeMessageData : public MessageData
{
public:
	ActivationChangeMessage::ActivationType activation_type;
};

ActivationChangeMessage::ActivationChangeMessage(void)
{
	SetType("activation change");
	SetData("activation change", boost::shared_ptr<ActivationChangeMessageData>(
		new ActivationChangeMessageData));
}

ActivationChangeMessage::ActivationChangeMessage(Message& message)
: Message(message)
{

}
ActivationChangeMessage::~ActivationChangeMessage(void)
{
}

std::string ActivationChangeMessage::GetTypeName()
{
	return "activation change";
}

ActivationChangeMessage::ActivationType ActivationChangeMessage::GetActivationType() const
{
	boost::shared_ptr<ActivationChangeMessageData> d = boost::dynamic_pointer_cast<ActivationChangeMessageData>(GetData("activation change"));
	if (!d)
		return ACTIVATION_LOST;
	return d->activation_type;
}

void ActivationChangeMessage::SetActivationType(ActivationType type)
{
	boost::shared_ptr<ActivationChangeMessageData> d = boost::dynamic_pointer_cast<ActivationChangeMessageData>(GetData("activation change"));
	if (!d)
	{
		d = boost::shared_ptr<ActivationChangeMessageData>(
			new ActivationChangeMessageData);
		SetData("activation change", d);
	}
	d->activation_type = type;
}
}