#include "StdAfx.h"
#include "InputEvent.h"

namespace wui{

InputEvent::InputEvent(void)
: m_id(-1), m_type(InputEvent::NO_KEY), m_nRepeatCount(0), m_pDevice(NULL)
{
}

InputEvent::~InputEvent(void)
{
}
}