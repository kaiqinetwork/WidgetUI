#include "StdAfx.h"
#include "InputDevice.h"
#include <boost/format.hpp>
#include "Keys.h"
#include "InputContext.h"

namespace wui{

InputDevice::InputDevice(void)
{
}

InputDevice::~InputDevice(void)
{
}

std::string InputDevice::GetName() const
{
	return std::string();
}

std::string InputDevice::GetDeviceName() const
{
	return std::string();
}

InputDevice::Type InputDevice::GetType() const
{
	return UNKNOWN;
}

std::string InputDevice::GetKeyName(int id) const
{
	return std::string();
}

std::string InputDevice::KeyIdToString(int keycode) const
{
	switch (keycode)
	{
	case WUI_KEY_BACKSPACE: return "backspace";
	case WUI_KEY_TAB: return "tab";
	case WUI_KEY_CLEAR: return "clear";
	case WUI_KEY_RETURN: return "return";
	case WUI_KEY_SHIFT: return "shift";
	case WUI_KEY_CONTROL: return "control";
	case WUI_KEY_MENU: return "menu";
	case WUI_KEY_PAUSE: return "pause";
	case WUI_KEY_ESCAPE: return "escape";
	case WUI_KEY_SPACE: return "space";
	case WUI_KEY_PRIOR: return "prior";
	case WUI_KEY_NEXT: return "next";
	case WUI_KEY_END: return "end";
	case WUI_KEY_HOME: return "home";
	case WUI_KEY_LEFT: return "left";
	case WUI_KEY_UP: return "up";
	case WUI_KEY_RIGHT: return "right";
	case WUI_KEY_DOWN: return "down";
	case WUI_KEY_SELECT: return "select";
	case WUI_KEY_PRINT: return "print";
	case WUI_KEY_EXECUTE: return "execute";
	case WUI_KEY_INSERT: return "insert";
	case WUI_KEY_DELETE: return "delete";
	case WUI_KEY_HELP: return "help";
	case WUI_KEY_0: return "0";
	case WUI_KEY_1: return "1";
	case WUI_KEY_2: return "2";
	case WUI_KEY_3: return "3";
	case WUI_KEY_4: return "4";
	case WUI_KEY_5: return "5";
	case WUI_KEY_6: return "6";
	case WUI_KEY_7: return "7";
	case WUI_KEY_8: return "8";
	case WUI_KEY_9: return "9";
	case WUI_KEY_A: return "a";
	case WUI_KEY_B: return "b";
	case WUI_KEY_C: return "c";
	case WUI_KEY_D: return "d";
	case WUI_KEY_E: return "e";
	case WUI_KEY_F: return "f";
	case WUI_KEY_G: return "g";
	case WUI_KEY_H: return "h";
	case WUI_KEY_I: return "i";
	case WUI_KEY_J: return "j";
	case WUI_KEY_K: return "k";
	case WUI_KEY_L: return "l";
	case WUI_KEY_M: return "m";
	case WUI_KEY_N: return "n";
	case WUI_KEY_O: return "o";
	case WUI_KEY_P: return "p";
	case WUI_KEY_Q: return "q";
	case WUI_KEY_R: return "r";
	case WUI_KEY_S: return "s";
	case WUI_KEY_T: return "t";
	case WUI_KEY_U: return "u";
	case WUI_KEY_V: return "v";
	case WUI_KEY_W: return "w";
	case WUI_KEY_X: return "x";
	case WUI_KEY_Y: return "y";
	case WUI_KEY_Z: return "z";
	case WUI_KEY_LWIN: return "lwin";
	case WUI_KEY_RWIN: return "rwin";
	case WUI_KEY_NUMPAD0: return "numpad0";
	case WUI_KEY_NUMPAD1: return "numpad1";
	case WUI_KEY_NUMPAD2: return "numpad2";
	case WUI_KEY_NUMPAD3: return "numpad3";
	case WUI_KEY_NUMPAD4: return "numpad4";
	case WUI_KEY_NUMPAD5: return "numpad5";
	case WUI_KEY_NUMPAD6: return "numpad6";
	case WUI_KEY_NUMPAD7: return "numpad7";
	case WUI_KEY_NUMPAD8: return "numpad8";
	case WUI_KEY_NUMPAD9: return "numpad9";
	case WUI_KEY_MULTIPLY: return "multiply";
	case WUI_KEY_ADD: return "add";
	case WUI_KEY_SEPARATOR: return "separator";
	case WUI_KEY_SUBTRACT: return "subtract";
	case WUI_KEY_DECIMAL: return "decimal";
	case WUI_KEY_DIVIDE: return "divide";
	case WUI_KEY_F1: return "f1";
	case WUI_KEY_F2: return "f2";
	case WUI_KEY_F3: return "f3";
	case WUI_KEY_F4: return "f4";
	case WUI_KEY_F5: return "f5";
	case WUI_KEY_F6: return "f6";
	case WUI_KEY_F7: return "f7";
	case WUI_KEY_F8: return "f8";
	case WUI_KEY_F9: return "f9";
	case WUI_KEY_F10: return "f10";
	case WUI_KEY_F11: return "f11";
	case WUI_KEY_F12: return "f12";
	case WUI_KEY_F13: return "f13";
	case WUI_KEY_F14: return "f14";
	case WUI_KEY_F15: return "f15";
	case WUI_KEY_F16: return "f16";
	case WUI_KEY_F17: return "f17";
	case WUI_KEY_F18: return "f18";
	case WUI_KEY_F19: return "f19";
	case WUI_KEY_F20: return "f20";
	case WUI_KEY_F21: return "f21";
	case WUI_KEY_F22: return "f22";
	case WUI_KEY_F23: return "f23";
	case WUI_KEY_F24: return "f24";
	case WUI_KEY_NUMLOCK: return "numlock";
	case WUI_KEY_SCROLL: return "scroll";
	case WUI_KEY_LSHIFT: return "lshift";
	case WUI_KEY_RSHIFT: return "rshift";
	case WUI_KEY_LCONTROL: return "lcontrol";
	case WUI_KEY_RCONTROL: return "rcontrol";
	case WUI_KEY_LMENU: return "lmenu";
	case WUI_KEY_RMENU: return "rmenu";
	default:
		{
			std::string str;
			//str.Format(_T("key_%d"), keycode);
			char buf[100];
			sprintf(buf,"%s  %d","key_",keycode);
			str = buf;
			return str;
		}
	}
}

int InputDevice::StringToKeyId(const std::string &str) const
{
	if (str.substr(0, 4) == "key_")
	{
		return atoi(str.substr(4).c_str());
	}

	static std::map<std::string, int> string_to_id;
	static bool first_call = true;
	if (first_call)
	{
		first_call = false;
		string_to_id["backspace"] = WUI_KEY_BACKSPACE;
		string_to_id["tab"] = WUI_KEY_TAB;
		string_to_id["clear"] = WUI_KEY_CLEAR;
		string_to_id["return"] = WUI_KEY_RETURN;
		string_to_id["shift"] = WUI_KEY_SHIFT;
		string_to_id["control"] = WUI_KEY_CONTROL;
		string_to_id["menu"] = WUI_KEY_MENU;
		string_to_id["pause"] = WUI_KEY_PAUSE;
		string_to_id["escape"] = WUI_KEY_ESCAPE;
		string_to_id["space"] = WUI_KEY_SPACE;
		string_to_id["prior"] = WUI_KEY_PRIOR;
		string_to_id["next"] = WUI_KEY_NEXT;
		string_to_id["end"] = WUI_KEY_END;
		string_to_id["home"] = WUI_KEY_HOME;
		string_to_id["left"] = WUI_KEY_LEFT;
		string_to_id["up"] = WUI_KEY_UP;
		string_to_id["right"] = WUI_KEY_RIGHT;
		string_to_id["down"] = WUI_KEY_DOWN;
		string_to_id["select"] = WUI_KEY_SELECT;
		string_to_id["print"] = WUI_KEY_PRINT;
		string_to_id["execute"] = WUI_KEY_EXECUTE;
		string_to_id["insert"] = WUI_KEY_INSERT;
		string_to_id["delete"] = WUI_KEY_DELETE;
		string_to_id["help"] = WUI_KEY_HELP;
		string_to_id["0"] = WUI_KEY_0;
		string_to_id["1"] = WUI_KEY_1;
		string_to_id["2"] = WUI_KEY_2;
		string_to_id["3"] = WUI_KEY_3;
		string_to_id["4"] = WUI_KEY_4;
		string_to_id["5"] = WUI_KEY_5;
		string_to_id["6"] = WUI_KEY_6;
		string_to_id["7"] = WUI_KEY_7;
		string_to_id["8"] = WUI_KEY_8;
		string_to_id["9"] = WUI_KEY_9;
		string_to_id["a"] = WUI_KEY_A;
		string_to_id["b"] = WUI_KEY_B;
		string_to_id["c"] = WUI_KEY_C;
		string_to_id["d"] = WUI_KEY_D;
		string_to_id["e"] = WUI_KEY_E;
		string_to_id["f"] = WUI_KEY_F;
		string_to_id["g"] = WUI_KEY_G;
		string_to_id["h"] = WUI_KEY_H;
		string_to_id["i"] = WUI_KEY_I;
		string_to_id["j"] = WUI_KEY_J;
		string_to_id["k"] = WUI_KEY_K;
		string_to_id["l"] = WUI_KEY_L;
		string_to_id["m"] = WUI_KEY_M;
		string_to_id["n"] = WUI_KEY_N;
		string_to_id["o"] = WUI_KEY_O;
		string_to_id["p"] = WUI_KEY_P;
		string_to_id["q"] = WUI_KEY_Q;
		string_to_id["r"] = WUI_KEY_R;
		string_to_id["s"] = WUI_KEY_S;
		string_to_id["t"] = WUI_KEY_T;
		string_to_id["u"] = WUI_KEY_U;
		string_to_id["v"] = WUI_KEY_V;
		string_to_id["w"] = WUI_KEY_W;
		string_to_id["x"] = WUI_KEY_X;
		string_to_id["y"] = WUI_KEY_Y;
		string_to_id["z"] = WUI_KEY_Z;
		string_to_id["lwin"] = WUI_KEY_LWIN;
		string_to_id["rwin"] = WUI_KEY_RWIN;
		string_to_id["numpad0"] = WUI_KEY_NUMPAD0;
		string_to_id["numpad1"] = WUI_KEY_NUMPAD1;
		string_to_id["numpad2"] = WUI_KEY_NUMPAD2;
		string_to_id["numpad3"] = WUI_KEY_NUMPAD3;
		string_to_id["numpad4"] = WUI_KEY_NUMPAD4;
		string_to_id["numpad5"] = WUI_KEY_NUMPAD5;
		string_to_id["numpad6"] = WUI_KEY_NUMPAD6;
		string_to_id["numpad7"] = WUI_KEY_NUMPAD7;
		string_to_id["numpad8"] = WUI_KEY_NUMPAD8;
		string_to_id["numpad9"] = WUI_KEY_NUMPAD9;
		string_to_id["multiply"] = WUI_KEY_MULTIPLY;
		string_to_id["add"] = WUI_KEY_ADD;
		string_to_id["separator"] = WUI_KEY_SEPARATOR;
		string_to_id["subtract"] = WUI_KEY_SUBTRACT;
		string_to_id["decimal"] = WUI_KEY_DECIMAL;
		string_to_id["divide"] = WUI_KEY_DIVIDE;
		string_to_id["f1"] = WUI_KEY_F1;
		string_to_id["f2"] = WUI_KEY_F2;
		string_to_id["f3"] = WUI_KEY_F3;
		string_to_id["f4"] = WUI_KEY_F4;
		string_to_id["f5"] = WUI_KEY_F5;
		string_to_id["f6"] = WUI_KEY_F6;
		string_to_id["f7"] = WUI_KEY_F7;
		string_to_id["f8"] = WUI_KEY_F8;
		string_to_id["f9"] = WUI_KEY_F9;
		string_to_id["f10"] = WUI_KEY_F10;
		string_to_id["f11"] = WUI_KEY_F11;
		string_to_id["f12"] = WUI_KEY_F12;
		string_to_id["f13"] = WUI_KEY_F13;
		string_to_id["f14"] = WUI_KEY_F14;
		string_to_id["f15"] = WUI_KEY_F15;
		string_to_id["f16"] = WUI_KEY_F16;
		string_to_id["f17"] = WUI_KEY_F17;
		string_to_id["f18"] = WUI_KEY_F18;
		string_to_id["f19"] = WUI_KEY_F19;
		string_to_id["f20"] = WUI_KEY_F20;
		string_to_id["f21"] = WUI_KEY_F21;
		string_to_id["f22"] = WUI_KEY_F22;
		string_to_id["f23"] = WUI_KEY_F23;
		string_to_id["f24"] = WUI_KEY_F24;
		string_to_id["numlock"] = WUI_KEY_NUMLOCK;
		string_to_id["scroll"] = WUI_KEY_SCROLL;
		string_to_id["lshift"] = WUI_KEY_LSHIFT;
		string_to_id["rshift"] = WUI_KEY_RSHIFT;
		string_to_id["lcontrol"] = WUI_KEY_LCONTROL;
		string_to_id["rcontrol"] = WUI_KEY_RCONTROL;
		string_to_id["lmenu"] = WUI_KEY_LMENU;
		string_to_id["rmenu"] = WUI_KEY_RMENU;
	}

	return string_to_id[str];
}

bool InputDevice::GetKeycode(int nKeyCode) const
{
	return false;
}

Point InputDevice::GetPosition() const
{
	return Point(GetX(), GetY());
}

int InputDevice::GetX() const
{
	return 0;
}

int InputDevice::GetY() const
{
	return 0;
}

void InputDevice::SetPosition(int x, int y)
{

}

boost::signal<void (const InputEvent &, const InputState &)> & InputDevice::sigKeyDown()
{
	return m_sigKeyDown;
}

boost::signal<void (const InputEvent &, const InputState &)> & InputDevice::sigKeyUp()
{
	return m_sigKeyUp;
}

boost::signal<void (const InputEvent &, const InputState &)> & InputDevice::sigPointerMove()
{
	return m_sigPointerMove;
}

boost::signal<void (const InputEvent &, const InputState &)> & InputDevice::sigBallMove()
{
	return m_sigBallMove;
}

boost::signal<void (const InputEvent &, const InputState &)> & InputDevice::sigKeyDblclk()
{
	return m_sigKeyDblclk;
}

void InputDevice::OnEvent(const InputEvent &e)
{
	for (std::vector<InputContext*>::iterator iter = m_InputContexts.begin(); 
		iter != m_InputContexts.end(); ++iter)
	{
		InputContext* ic = *iter;
		if (ic)
			ic->ReceivedEvent(e, this);
	}
}
}