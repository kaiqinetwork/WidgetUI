#pragma once
#include <boost/signals.hpp>
#include "Widget.h"

namespace wui {

class AxContainer;

class WIDGETUI_API AxWidget : public Widget
{
public:
    AxWidget(Widget* pParent);
    virtual ~AxWidget();

	virtual RECT GetControlRect();
	virtual HWND GetParentWnd();
	virtual HWND GetHostWnd();
	virtual void SetFocus(bool bFocus);
	virtual void SetCapture(bool bCapture);

	virtual void SetDelayCreate(bool bDelayCreate = true);
	virtual void SetModuleName(LPCTSTR pstrText);
	virtual bool CreateControl(const CLSID clsid);
	virtual bool CreateControl(LPCTSTR pstrCLSID);
	virtual void SetVisible(bool bVisible = true, bool bActivateRootWin = true);
	virtual void SetGeometry(Rect geometry);
	virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

	virtual CLSID GetClisd() const;
	virtual bool IsDelayCreate() const;
	virtual const std::string & GetModuleName() const;

	virtual void OnRender(XGraphics& graphic, const Rect& rcUpdate);
	virtual void OnProcessMessage(Message& wuiMsg);
	virtual bool OnTranslateAccelerator(MSG &msg);


protected:
    virtual bool DoCreateControl();
	bool wuiMessageToWinMsg(Message &wuiMsg, MSG &winMsg);
	LPUNKNOWN GetUnknownControl();

protected:
	LONG m_dwRef;
    CLSID m_clsid;
	std::string m_strModuleName;
    bool m_bCreated;
    bool m_bDelayCreate;
	AxContainer *m_pContainer;
};

} // namespace wui
