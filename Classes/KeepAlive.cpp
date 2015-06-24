#include "StdAfx.h"
#include "KeepAlive.h"
#include "MessageQueue.h"
#include <algorithm>

namespace wui{

KeepAlive::KeepAliveObjectVector KeepAlive::keepAliveObjects;

void KeepAlive::Process()
{
	MessageQueue::m_messageQueue.ProcessMessage();
	
	for (KeepAliveObjectVector::iterator iter = keepAliveObjects.begin(); iter != keepAliveObjects.end(); ++iter)
	{
		(*iter)->Process();
	}
}

KeepAliveObject::KeepAliveObject()
{
	KeepAlive::keepAliveObjects.push_back(this);
}

KeepAliveObject::~KeepAliveObject()
{
	KeepAlive::keepAliveObjects.erase(std::find(KeepAlive::keepAliveObjects.begin(), 
		KeepAlive::keepAliveObjects.end(), this));
}
}