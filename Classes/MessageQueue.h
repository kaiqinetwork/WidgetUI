#pragma once

namespace wui{

class WidgetHostWindow;

class MessageQueue
{
public:
	MessageQueue(void);
	~MessageQueue(void);
	
	static MessageQueue m_messageQueue;
	
	void AddClient(WidgetHostWindow* pWindow);
	void RemoveClient(WidgetHostWindow* pWindow);
	
	void ProcessMessage();
	
private:
	std::list<WidgetHostWindow *> m_windows;
};

}