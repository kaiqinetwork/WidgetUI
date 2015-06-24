#include "StdAfx.h"
#include "ItemsDisplayWindow.h"

ItemsDisplayWindow::ItemsDisplayWindow( Widget* pWin, Rect rcClient )
 : Panel(pWin)
{
	m_imgBk = new CxImage;
	m_imgBk->Load("..\\res\\bg.bmp",CXIMAGE_FORMAT_BMP);
	SetGeometry(rcClient);

 	SetBkCenterImage(m_imgBk);

	Rect rect(5,7,155,30);
	HFONT hfont = CreateFont(14, 0, 0, 0, FW_BOLD, FALSE, FALSE, 0, 
		ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, 
		DEFAULT_PITCH | FF_SWISS, _T("宋体"));
	m_pPathLabel = new Label(this);
	m_pPathLabel->SetGeometry(rect);
	m_pPathLabel->SetTextColor(RGB(2,121,161));
	m_pPathLabel->SetFont(hfont);
	m_pPathLabel->SetText(_T("游戏商城 > 全部道具"));

	rect.y = rect.GetBottom() + 2;
	rect.height = 107;

	for (int i = 0; i < 9; i++)
	{
		m_pItemwidget[i] = new ItemDisplayWidget(this);
		if (i > 0 && i % 3 == 0)
		{
			rect.x = 5;
			rect.y = rect.GetBottom() + 5;
			rect.height = 109;
		}
		rect.width = 219;
		m_pItemwidget[i]->SetGeometry(rect);
		rect.x = rect.GetRight() + 6;
	}
	
}
ItemsDisplayWindow::~ItemsDisplayWindow(void)
{
	delete m_imgBk;
}
