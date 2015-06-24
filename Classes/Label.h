#pragma once

#include "Widget.h"
#include "Geometry.h"

namespace wui{

class WIDGETUI_API Label : public Widget
{
public:
	Label(Widget* pParent);
	virtual ~Label(void);
	
	enum Alignment
	{
		ALIGN_LEFT,
		ALIGN_CENTER,
		ALIGN_RIGHT,
		ALIGN_JUSTIFY
	};
	
	Alignment GetAlignment() const;
	std::string GetText() const;
	Size GetPreferredSize();
	void SetText(const std::string& text);
	void SetAlignment(Alignment align);
	void SetFont(HFONT pFont);
	void SetTextColor(COLORREF clrText);
	void SetBkImage(CxImage* pImage);

protected:
	void OnProcessMessage(Message &msg);
	void OnRender(XGraphics &graphic, const Rect &rcUpdate);
	
protected:
	Alignment m_alignment;
	std::string m_strText;
	HFONT m_pFont;
	COLORREF m_clrText;
	CxImage* m_pBkImage;
};
}