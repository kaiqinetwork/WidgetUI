#pragma once

#include <boost/signals.hpp>
#include "InputEvent.h"
#include "InputState.h"

namespace wui{

class InputContext;

class InputDevice
{
	friend class InputContext;
	
public:
	InputDevice(void);
	virtual ~InputDevice(void);
	
	enum Type
	{
		KEYBOARD,
		POINTER,
		UNKNOWN
	};

	virtual std::string GetName() const;
	virtual std::string GetDeviceName() const;
	virtual Type GetType() const;
	virtual std::string GetKeyName(int id) const;
	virtual std::string KeyIdToString(int keycode) const;
	virtual int StringToKeyId(const std::string &str) const;
	virtual bool GetKeycode(int nKeyCode) const;
	virtual Point GetPosition() const;
	virtual int GetX() const;
	virtual int GetY() const;
	
	virtual void SetPosition(int x, int y);
	
	boost::signal<void (const InputEvent &, const InputState &)> &sigKeyDown();
	boost::signal<void (const InputEvent &, const InputState &)> &sigKeyUp();
	boost::signal<void (const InputEvent &, const InputState &)> &sigPointerMove();
	boost::signal<void (const InputEvent &, const InputState &)> &sigBallMove();
	boost::signal<void (const InputEvent &, const InputState &)> &sigKeyDblclk();
	
	virtual void OnEvent(const InputEvent &e);
	
private:
	boost::signal<void (const InputEvent &, const InputState &)> m_sigKeyDown;
	boost::signal<void (const InputEvent &, const InputState &)> m_sigKeyUp;
	boost::signal<void (const InputEvent &, const InputState &)> m_sigPointerMove;
	boost::signal<void (const InputEvent &, const InputState &)> m_sigBallMove;
	boost::signal<void (const InputEvent &, const InputState &)> m_sigKeyDblclk;
	
	std::vector<InputContext*> m_InputContexts;
};

}