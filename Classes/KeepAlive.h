#pragma once

#include "WidgetUI.h"

namespace wui{

class WIDGETUI_API KeepAliveObject
{
public:
	KeepAliveObject();
	virtual ~KeepAliveObject();

	virtual void Process() = 0;
};

class WIDGETUI_API KeepAlive
{
friend KeepAliveObject;

public:
	static void Process();
	
	typedef std::vector<KeepAliveObject*> KeepAliveObjectVector;

private:
	static KeepAliveObjectVector keepAliveObjects;
};

}