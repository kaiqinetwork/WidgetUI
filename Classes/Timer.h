#pragma once

#include "WidgetUI.h"
#include <boost/asio.hpp>
#include <boost/signals.hpp>

namespace wui{

class TimerThread;

class WIDGETUI_API Timer
{
public:
	Timer(void);
	~Timer(void);
	
	void Start(unsigned int nNewTimeout, bool bRepeat);
	void Stop();

	bool IsRepeating() const { return m_bRepeating; }
	unsigned int GetTimeout() const { return m_nTimeout; }

	boost::signal<void ()>& sigExpired();

private:
	static TimerThread *timerThread;
	static int nTimerThreadInstanceCount;
	static int nTimerThreadMaxId;	// Unique timer id

	unsigned int m_nTimeout;
	bool m_bRepeating;
	int m_nId;
};

}