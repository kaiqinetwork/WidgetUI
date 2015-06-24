#include "StdAfx.h"
#include "MessageQueue.h"
#include <algorithm>
#include "WidgetHostWindow.h"

namespace wui{

MessageQueue MessageQueue::m_messageQueue;

MessageQueue::MessageQueue(void)
{
}

MessageQueue::~MessageQueue(void)
{
}

void MessageQueue::AddClient(WidgetHostWindow* pWindow)
{
	m_windows.push_back(pWindow);
}

void MessageQueue::RemoveClient(WidgetHostWindow* pWindow)
{
	std::list<WidgetHostWindow *>::iterator iter = std::find(m_windows.begin(), m_windows.end(), pWindow);
	if (iter == m_windows.end())
		return;
	m_windows.erase(iter);
}

void MessageQueue::ProcessMessage()
{
	for (std::list<WidgetHostWindow *>::iterator iter = m_windows.begin(), next;
		iter != m_windows.end(); iter = next)
	{
		next = iter;
		++next;
		(*iter)->GetInputContext().ProcessMessages();
	}
}
}