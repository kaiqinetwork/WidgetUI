#pragma once
#include "Window.h"
#include "WindowDescription.h"
#include "Manager.h"
#include "Label.h"
#include "Panel.h"
#include "ItemDisplayWidget.h"
#include "ximage/ximage.h"
#include "Resource.h"

using namespace wui;

class ItemsDisplayWindow : public Panel
{
public:
	ItemsDisplayWindow( Widget* pWin, Rect rcClient );
	~ItemsDisplayWindow(void);


private:
	CxImage* m_imgBk;
	Label*	 m_pPathLabel;
	ItemDisplayWidget* m_pItemwidget[9];
};
