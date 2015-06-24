#pragma once
#include "Widget.h"
#include "ximage/ximage.h"
#include "Label.h"
#include "PushButton.h"
#include "Panel.h"
#include "Resource.h"

using namespace wui;
class ItemDisplayWidget : public Panel
{
public:
	ItemDisplayWidget(Widget* pParent);
	~ItemDisplayWidget(void);


private:
	CxImage m_imgBordertl;
	CxImage m_imgBorderbg;
	CxImage m_imgBorderbl;
	CxImage m_imgBorderbm;
	CxImage m_imgBorderbr;
	CxImage m_imgBorderl;
	CxImage m_imgBorderr;
	CxImage m_imgBordertm;
	CxImage m_imgBordertr;
	CxImage* m_pimgItemBk;
	CxImage* m_pimgNew;
	CxImage* m_pimgHot;
	CxImage* m_pimgBuyBtn;
	CxImage* m_pimgPresentBtn;


	Label* m_pNameLabel;
	Label* m_pPriceLabel;
	Label* m_pActivityPriceLabel;
	Label* m_pVipPriceLabel;
	Label* m_pPriceLabel2;
	Label* m_pActivityPriceLabel2;
	Label* m_pActivityLabel;
	Label* m_pHotLabel;
	Label* m_pNewLabel;
	Label* m_pItemBkLabel;
	Label* m_pItemLabel;
	Label* m_pGoldLabel[2];
	PushButton* m_pBuyBtn;
	PushButton* m_pPresentBtn;
};
