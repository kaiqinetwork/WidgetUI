#include "StdAfx.h"
#include "WUIMessage.h"

namespace wui{

Message::Message(void)
: m_bConsumed(false), m_target(NULL)
{
}

Message::~Message(void)
{
}

std::string Message::GetType() const
{
	return m_type;
}

bool Message::IsType(const std::string& type) const
{
	return m_type.compare(type) == 0;
}

Message::MessageDataPtr Message::GetData(const std::string& dataName) const
{
	DataObjectMap::const_iterator it = m_dataObjects.find(dataName);
	if (it != m_dataObjects.end())
		return it->second;
	else
		return MessageDataPtr();
}

Widget* Message::GetTarget() const
{
	return m_target;
}

bool Message::IsConsumed() const
{
	return m_bConsumed;
}

void Message::SetType(const std::string &type)
{
	m_type = type;
}

void Message::SetData(const std::string &dataName, const MessageDataPtr &ptr)
{
	m_dataObjects[dataName] = ptr;
}

void Message::SetTarget(Widget *target)
{
	m_target = target;
}

void Message::SetNull()
{
	m_bConsumed = false;
	m_type = "";
	m_target = NULL;
	m_dataObjects.clear();
}

void Message::SetConsumed()
{
	m_bConsumed = true;
}
}