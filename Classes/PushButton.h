#pragma once

#include "Widget.h"
#include "Geometry.h"

//class CPPToolTip;

namespace wui{

class WIDGETUI_API PushButton : public Widget
{
public:
	PushButton(Widget* pParent);
	virtual ~PushButton(void);
	
	enum {
		BTN_IMAGE_1STATE,
		BTN_IMAGE_3STATE,
		BTN_IMAGE_4STATE,
		BTN_IMAGE_5STATE,
	};
	
	bool IsHot() const;
	bool IsPushed() const;
	const std::string &GetText() const;
	CxImage* GetBkImage() const;
	Size GetPreferredSize();
	
	void SetPushed(bool enable);
	void SetText(const char* lpszText);
	void SetBkImage(CxImage* pImage, int nImageType = BTN_IMAGE_1STATE);
	void SetUseHandCursor(bool bUse);
	void SetFont(HFONT pFont);
	void SetTextColor(COLORREF clrText);
	
	//void SetTooltip(CPPToolTip* pTooltip);
	void SetTooltipText(LPCTSTR lpszText);

	boost::signal<void ()>& sigClicked();

private:
	void OnRender(XGraphics& graphic, const Rect& rcUpdate);
	void OnProcessMessage(Message& msg);
	void OnEnableModeChanged();

protected:
	std::string m_strText;
	CxImage* m_pBkImage;
	int m_nBkImageType;
	HFONT m_pFont;
	COLORREF m_clrText;
	
	std::string m_strTooltipText;
	//CPPToolTip* m_pTooltip;
	bool m_bTooltipShowed;
	
	bool m_bUseHandCursor;
	
	bool m_bPressed;
	bool m_bHot;

private:	
	boost::signal<void ()> m_sigClicked;
};
}