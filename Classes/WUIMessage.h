#pragma once

#include "WidgetUI.h"
#include <boost/smart_ptr.hpp>
#include <string>

namespace wui{

class Widget;

class MessageData
{
public:
	virtual ~MessageData() { }
};

class WIDGETUI_API Message
{
public:
	Message(void);
	virtual ~Message(void);
	
	typedef boost::shared_ptr<MessageData> MessageDataPtr;
	
	std::string GetType() const;
	bool IsType(const std::string& type) const;
	MessageDataPtr GetData(const std::string& dataName) const;
	Widget* GetTarget() const;
	bool IsConsumed() const;
	
	void SetType(const std::string &type);
	void SetData(const std::string &dataName, const MessageDataPtr &ptr);
	void SetTarget(Widget *target);
	void SetNull();
	void SetConsumed();
	
private:
	bool m_bConsumed;
	std::string m_type;
	Widget* m_target;
	typedef std::map<std::string, MessageDataPtr> DataObjectMap;
	DataObjectMap m_dataObjects;
};
}