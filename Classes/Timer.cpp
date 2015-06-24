#include "StdAfx.h"
#include "Timer.h"
#include "KeepAlive.h"
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/thread/condition.hpp>

namespace wui{

class TimerObject
{
public:
	TimerObject(boost::asio::io_service& ioservice) 
		: bRepeating(false), bExpired(false), timer(ioservice) {}
	
	boost::asio::deadline_timer timer;
	unsigned int timeout;
	bool bRepeating;
	bool bExpired;
	boost::signal<void ()> sigExpired;
};

class TimerThread : public KeepAliveObject
{
public:
	TimerThread() : m_aliveTimer(m_ioservie), m_timerCond(false)
	{
		m_aliveTimer.expires_from_now(boost::posix_time::minutes(15));
		m_aliveTimer.async_wait(boost::bind(&TimerThread::HandleAliveTimer, this, boost::asio::placeholders::error));
		m_pThread = new boost::thread(boost::bind(&boost::asio::io_service::run, &m_ioservie));
	}

	~TimerThread()
	{
		boost::system::error_code ignored_ec;
		for (std::map<int, TimerObject*>::iterator iter = m_timerObjects.begin(); iter != m_timerObjects.end(); ++iter)
		{
			iter->second->timer.cancel(ignored_ec);
			delete iter->second; 
		}
		m_aliveTimer.cancel(ignored_ec);
		m_ioservie.stop();
		m_pThread->join();
		delete m_pThread;
	}

	void Start(int nTimerId, unsigned int nNewTimeout, bool bRepeat)
	{
		TimerObject*& pObject = m_timerObjects[nTimerId];
		if (pObject == NULL)
		{
			pObject = new TimerObject(m_ioservie);
		}

		boost::system::error_code ignored_ec;
		pObject->timer.cancel(ignored_ec);
		pObject->bRepeating = bRepeat;
		pObject->timeout = nNewTimeout;
		pObject->bExpired = false;
		pObject->timer.expires_from_now(boost::posix_time::milliseconds(nNewTimeout));
		pObject->timer.async_wait(boost::bind(&TimerThread::HandleTimerObjectExpired, this, 
			boost::asio::placeholders::error, pObject));
	}

	void Stop(int nTimerId)
	{
		TimerObject* pObject = m_timerObjects[nTimerId];
		if (pObject)
		{
			boost::system::error_code ignored_ec;
			pObject->timer.cancel(ignored_ec);
			pObject->bExpired = false;
		}
	}

	void RemoveTimer(int nTimerId)
	{
		// Remove the unused timers, to prevent memory leaks
		std::map<int, TimerObject *>::iterator it = m_timerObjects.find(nTimerId);
		if (it != m_timerObjects.end())
		{
			boost::system::error_code ignored_ec;
			it->second->timer.cancel(ignored_ec);
			delete it->second;
			m_timerObjects.erase(it);
		}
	}

	void Process()
	{
		boost::mutex::scoped_lock lock(m_timerMutex);
		if (!m_timerCond)
			return;
		
		TimerObject* pTimerObject = NULL;
		for (std::map<int, TimerObject*>::iterator iter = m_timerObjects.begin(); iter != m_timerObjects.end(); ++iter)
		{
			pTimerObject = iter->second;
			if (pTimerObject->bExpired)
			{
				pTimerObject->sigExpired();

				if (pTimerObject->bRepeating)
				{
					pTimerObject->timer.expires_from_now(boost::posix_time::milliseconds(pTimerObject->timeout));
					pTimerObject->timer.async_wait(boost::bind(&TimerThread::HandleTimerObjectExpired, this, 
						boost::asio::placeholders::error, pTimerObject));
				}

				pTimerObject->bExpired = false;
			}
		}
		
		m_timerCond = false;
	}

	boost::signal<void ()>& sigExpired(int nTimerId)
	{
		return GetTimerObject(nTimerId).sigExpired;
	}

private:
	void HandleAliveTimer(const boost::system::error_code& e)
	{
		if (e)
			return;
		
		m_aliveTimer.expires_from_now(boost::posix_time::minutes(15));
		m_aliveTimer.async_wait(boost::bind(&TimerThread::HandleAliveTimer, this, boost::asio::placeholders::error));
	}

	void HandleTimerObjectExpired(const boost::system::error_code& e, TimerObject* pTimerObject)
	{
		if (e)
			return;
		
		pTimerObject->bExpired = true;
		
		boost::mutex::scoped_lock lock(m_timerMutex);
		m_timerCond = true;
	}

	TimerObject &GetTimerObject(int nTimerId)
	{
		// Find existing timer
		std::map<int, TimerObject* >::iterator it = m_timerObjects.find(nTimerId);
		if (it != m_timerObjects.end())
		{
			return *(it->second);
		}

		TimerObject *object = new TimerObject(m_ioservie);
		m_timerObjects[nTimerId] = object;
		return *object;
	}

private:
	std::map<int, TimerObject*> m_timerObjects;
	boost::asio::io_service m_ioservie;
	boost::thread* m_pThread;
	boost::mutex m_timerMutex;
	bool m_timerCond;
	boost::asio::deadline_timer m_aliveTimer;
};


TimerThread *Timer::timerThread = NULL;
int Timer::nTimerThreadInstanceCount = 0;
int Timer::nTimerThreadMaxId = 0;

Timer::Timer(void)
: m_nTimeout(0), m_bRepeating(false), m_nId(0)
{
	if (!nTimerThreadInstanceCount)
	{
		timerThread = new TimerThread();
	}
	nTimerThreadInstanceCount++;
	nTimerThreadMaxId++;
	m_nId = nTimerThreadMaxId;
}

Timer::~Timer(void)
{
	timerThread->RemoveTimer(m_nId);
	nTimerThreadInstanceCount--;
	if (!nTimerThreadInstanceCount)
	{
		delete timerThread;
		timerThread = NULL;
	}
}

void Timer::Start(unsigned int nNewTimeout, bool bRepeat)
{
	timerThread->Start(m_nId, nNewTimeout, bRepeat);
}

void Timer::Stop()
{
	timerThread->Stop(m_nId);
}

boost::signal<void ()>& Timer::sigExpired()
{
	return timerThread->sigExpired(m_nId);
}
}