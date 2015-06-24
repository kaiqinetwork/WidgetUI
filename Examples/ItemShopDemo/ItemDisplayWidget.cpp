#include "StdAfx.h"
#include "ItemDisplayWidget.h"

ItemDisplayWidget::ItemDisplayWidget( Widget* pParent ) : Panel(pParent)
{
	m_pimgItemBk = new CxImage;
	m_pimgBuyBtn = new CxImage;
	m_pimgHot = new CxImage;
	m_pimgNew = new CxImage;
	m_pimgPresentBtn = new CxImage;
	m_imgBordertl.LoadResource(FindResource(NULL,MAKEINTRESOURCE(IDB_BORDERTL),_T("PNG")),CXIMAGE_FORMAT_PNG);
	m_imgBorderbg.LoadResource(FindResource(NULL,MAKEINTRESOURCE(IDB_BORDERBG),_T("PNG")),CXIMAGE_FORMAT_PNG);
	m_imgBorderbl.LoadResource(FindResource(NULL,MAKEINTRESOURCE(IDB_BORDERBL),_T("PNG")),CXIMAGE_FORMAT_PNG);
	m_imgBorderbm.LoadResource(FindResource(NULL,MAKEINTRESOURCE(IDB_BORDERBM),_T("PNG")),CXIMAGE_FORMAT_PNG);
	m_imgBorderbr.LoadResource(FindResource(NULL,MAKEINTRESOURCE(IDB_BORDERBR),_T("PNG")),CXIMAGE_FORMAT_PNG);
	m_imgBorderl.LoadResource(FindResource(NULL,MAKEINTRESOURCE(IDB_BORDERL),_T("PNG")),CXIMAGE_FORMAT_PNG);
	m_imgBorderr.LoadResource(FindResource(NULL,MAKEINTRESOURCE(IDB_BORDERR),_T("PNG")),CXIMAGE_FORMAT_PNG);
	m_imgBordertm.LoadResource(FindResource(NULL,MAKEINTRESOURCE(IDB_BORDERTM),_T("PNG")),CXIMAGE_FORMAT_PNG);
	m_imgBordertr.LoadResource(FindResource(NULL,MAKEINTRESOURCE(IDB_BORDERTR),_T("PNG")),CXIMAGE_FORMAT_PNG);
	m_pimgItemBk->Load("D:\\Program Files (x86)\\K7豫游游戏中心\\res\\itemshop\\itemback.bmp", CXIMAGE_FORMAT_BMP);
	m_pimgItemBk->IncreaseBpp(24);
	m_pimgItemBk->SetTransIndex(0);
	m_pimgItemBk->SetTransColor(CxImage::RGBtoRGBQUAD(RGB(255,0,255)));
	m_pimgBuyBtn->LoadResource(FindResource(NULL,MAKEINTRESOURCE(IDB_BUY),_T("PNG")),CXIMAGE_FORMAT_PNG);
	m_pimgPresentBtn->LoadResource(FindResource(NULL,MAKEINTRESOURCE(IDB_PRESENT),_T("PNG")),CXIMAGE_FORMAT_PNG);
	m_pimgHot->LoadResource(FindResource(NULL,MAKEINTRESOURCE(IDB_HOT),_T("PNG")),CXIMAGE_FORMAT_PNG);
	m_pimgNew->LoadResource(FindResource(NULL,MAKEINTRESOURCE(IDB_NEW),_T("PNG")),CXIMAGE_FORMAT_PNG);
	HFONT hf12bold = CreateFont(12, 0, 0, 0, FW_BOLD, FALSE, FALSE, 0, 
		ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, 
		DEFAULT_PITCH | FF_SWISS, _T("宋体"));


	SetBorderBLImage(&m_imgBorderbl);
	SetBorderBMImage(&m_imgBorderbm);
	SetBorderBRImage(&m_imgBorderbr);
	SetBorderLImage(&m_imgBorderl);
	SetBorderRImage(&m_imgBorderr);
	SetBorderTLImage(&m_imgBordertl);
	SetBorderTMImage(&m_imgBordertm);
	SetBorderTRImage(&m_imgBordertr);
	SetBkCenterImage(&m_imgBorderbg);

	Rect rect(0,0,0,0);
	
	m_pNameLabel = new Label(this);
	rect.x = 8;
	rect.width = 110;
	rect.y = 8;
	rect.height = 17;
	m_pNameLabel->SetGeometry(rect);
	m_pNameLabel->SetFont(hf12bold);
	m_pNameLabel->SetTextColor(RGB(77,74,65));

	m_pNewLabel = new Label(this);
	rect.x = rect.GetRight() + 3;
	rect.width = 15;
	m_pNewLabel->SetGeometry(rect);
	m_pNewLabel->SetBkImage(m_pimgNew);

	m_pHotLabel = new Label(this);
	rect.x = rect.GetRight() + 3;
	rect.width = 15;
	m_pHotLabel->SetGeometry(rect);
	m_pHotLabel->SetBkImage(m_pimgHot);

	m_pItemBkLabel = new Label(this);
	rect.y = rect.GetBottom() + 10;
	rect.height = (m_pimgItemBk->IsValid() ? m_pimgItemBk->GetHeight() : 70);
	rect.x = 8;
	rect.width = (m_pimgItemBk->IsValid() ? m_pimgItemBk->GetWidth() : 68);
	m_pItemBkLabel->SetGeometry(rect);
	m_pItemBkLabel->SetBkImage(m_pimgItemBk);

	m_pPriceLabel = new Label(this);
	rect.x = rect.GetRight() + 8;
	rect.width = 130;
	rect.y = rect.y - 3;
	rect.height = 17;
	m_pPriceLabel->SetGeometry(rect);
	m_pPriceLabel->SetTextColor(RGB(100,100,100));
	m_pPriceLabel->SetText(_T("价    格："));

	m_pVipPriceLabel = new Label(this);
	rect.Offset(0, 17);
	m_pVipPriceLabel->SetGeometry(rect);
	m_pVipPriceLabel->SetTextColor(RGB(100,100,100));
	m_pVipPriceLabel->SetText(_T("会员劲爆价8折起"));

	m_pActivityPriceLabel = new Label(this);
	rect.Offset(0, 17);
	m_pActivityPriceLabel->SetGeometry(rect);
	m_pActivityPriceLabel->SetTextColor(RGB(100,100,100));
	m_pActivityPriceLabel->SetText(_T("活动价格："));

	m_pPresentBtn = new PushButton(this);
	m_pPresentBtn->SetBkImage(m_pimgPresentBtn, PushButton::BTN_IMAGE_4STATE);
	rect.x = 87;
	rect.width = m_pPresentBtn->GetPreferredWidth();
	rect.y = rect.GetBottom() + 3;
	rect.height = m_pPresentBtn->GetPreferredHeight();
	m_pPresentBtn->SetGeometry(rect);
	m_pPresentBtn->SetUseHandCursor(true);

	m_pBuyBtn = new PushButton(this);
	m_pBuyBtn->SetBkImage(m_pimgBuyBtn, PushButton::BTN_IMAGE_4STATE);
	rect.x = rect.GetRight() + 41;
	rect.width= m_pBuyBtn->GetPreferredWidth();
	rect.height = m_pBuyBtn->GetPreferredHeight();
	m_pBuyBtn->SetGeometry(rect);
	m_pBuyBtn->SetUseHandCursor(true);

}
ItemDisplayWidget::~ItemDisplayWidget(void)
{
}
