#pragma once

#include "Widget.h"
#include "Geometry.h"

namespace wui{

class WIDGETUI_API ProgressBar : public Widget
{
public:
	ProgressBar(Widget* pParent);
	virtual ~ProgressBar(void);

	int GetMin() const;
	int GetMax() const;
	int GetStepSize() const;
	int GetPosition() const;
	void SetMin(int nMin);
	void SetMax(int nMax);
	void SetRange(int nMin, int nMax);
	void SetStepSize(int nSize);
	void SetPosition(int nPos);
	void AdvancePosition(int nPos);
	void StepPosition();
	
	void SetBkImage(CxImage* pImage);
	void SetBarImage(CxImage* pImage);
	
protected:
	void CheckRange();

	void OnProcessMessage(Message &msg);
	void OnRender(XGraphics &graphic, const Rect &rcUpdate);
	
protected:
	CxImage* m_pBkImage;
	CxImage* m_pBarImage;
	
	int m_nMin;
	int m_nMax;
	int m_nSetpSize;
	int m_nPosition;
};
}