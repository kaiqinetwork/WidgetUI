#pragma once

#include "stdafx.h"
#include "LoginDemo.h"
#include <WidgetUI/WindowDescription.h>
#include <WidgetUI/Graphic.h>
#include <WidgetUI/WidgetHostWindow.h>
#include <WidgetUI/KeepAlive.h>
#include <WidgetUI/Geometry.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM) ;

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,PSTR szCmdLine, int iCmdShow)      
{
	HWND                    hwnd ;
	MSG                     msg ;

	//hwnd = CreateDialog(hInstance,MAKEINTRESOURCE(IDD_DIALOG1),NULL,(DLGPROC)WndProc);
	pMgr = new Manager();
	pWinmgr = new WindowManager();
	pMgr->SetWindowManager(pWinmgr);
	hwnd = CreateDialogParam(hInstance,MAKEINTRESOURCE(IDD_LOGINDEMO),NULL,(DLGPROC)WndProc,NULL);
	
	img.LoadResource(FindResource(NULL,MAKEINTRESOURCE(IDB_PNG2),_T("PNG")),CXIMAGE_FORMAT_PNG);
	imgBordertl.LoadResource(FindResource(NULL,MAKEINTRESOURCE(IDB_BORDERTL),_T("PNG")),CXIMAGE_FORMAT_PNG);
	imgBorderbg.LoadResource(FindResource(NULL,MAKEINTRESOURCE(IDB_BORDERBG),_T("PNG")),CXIMAGE_FORMAT_PNG);
	imgBorderbl.LoadResource(FindResource(NULL,MAKEINTRESOURCE(IDB_BORDERBL),_T("PNG")),CXIMAGE_FORMAT_PNG);
	imgBorderbm.LoadResource(FindResource(NULL,MAKEINTRESOURCE(IDB_BORDERBM),_T("PNG")),CXIMAGE_FORMAT_PNG);
	imgBorderbr.LoadResource(FindResource(NULL,MAKEINTRESOURCE(IDB_BORDERBR),_T("PNG")),CXIMAGE_FORMAT_PNG);
	imgBorderl.LoadResource(FindResource(NULL,MAKEINTRESOURCE(IDB_BORDERL),_T("PNG")),CXIMAGE_FORMAT_PNG);
	imgBorderr.LoadResource(FindResource(NULL,MAKEINTRESOURCE(IDB_BORDERR),_T("PNG")),CXIMAGE_FORMAT_PNG);
	imgBordertm.LoadResource(FindResource(NULL,MAKEINTRESOURCE(IDB_BORDERTM),_T("PNG")),CXIMAGE_FORMAT_PNG);
	imgBordertr.LoadResource(FindResource(NULL,MAKEINTRESOURCE(IDB_BORDERTR),_T("PNG")),CXIMAGE_FORMAT_PNG);
	
	ShowWindow (hwnd, iCmdShow) ;
	UpdateWindow (hwnd) ;

	while (GetMessage (&msg, NULL, 0, 0))        
	{
		TranslateMessage (&msg) ;
		DispatchMessage (&msg) ;
		KeepAlive::Process();
	}    
	return msg.wParam ;       
}


LRESULT CALLBACK WndProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{

	HDC hdc;
	switch (message)
	{
	case   WM_CREATE :
		return 0 ;

	case WM_INITDIALOG:
		{
			GetWindowRect(hDlg,&rect);
			GetClientRect(hDlg,&rcClient);
			pDesc = new TopLevelDescription();
			//pDesc->SetHandle(hDlg);
			pDesc->SetOwnerHandle(hDlg);
			pDesc->SetChildWindow(true);
			pDesc->SetPosition(Rect(rcClient.left,rcClient.top,rcClient.right-rcClient.left,rcClient.bottom-rcClient.top),false);
			// 			WUIHostWind = new WidgetHostWindow(*pDesc);
			//			WUIHostWind->IsVisible(FALSE);
			pWin = new Window(pMgr, *pDesc);
			//hwnd2 = WUIHostWind->GetHwnd();
			pWin->SetWindowGeometry(Rect(rcClient.left,rcClient.top,rcClient.right-rcClient.left,rcClient.bottom-rcClient.top));
			
			CreatPanel(pWin);
			CreateButton(pWin);
			CreateLebel(pWin);

			return (INT_PTR)TRUE;
		}
	case WM_PAINT:
		{
			// 			hdc = BeginPaint(hDlg, &ps);
			hdc = pWin->GetHostWindow()->GetGraphic().GetDC();
			GetClientRect(hDlg,&rect);
			img.Draw(hdc,rect);

			// 			EndPaint(hDlg,&ps);
			break;
		}
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDOK:
		case IDCANCEL:
			DestroyWindow(hDlg);
			break;
		}
		break;
	case   WM_DESTROY :
		PostQuitMessage (0) ;
		return 0 ;

	}

	return DefWindowProc (hDlg, message, wParam, lParam) ;       
}

void CreatPanel( Widget *pParent )
{
	Rect rcPanel(10,10,0,0);
	pPanel = new Panel(pParent);
	pPanel->SetBorderBLImage(&imgBorderbl);
	pPanel->SetBorderBMImage(&imgBorderbm);
	pPanel->SetBorderBRImage(&imgBorderbr);
	pPanel->SetBorderLImage(&imgBorderl);
	pPanel->SetBorderRImage(&imgBorderr);
	pPanel->SetBorderTLImage(&imgBordertl);
	pPanel->SetBorderTMImage(&imgBordertm);
	pPanel->SetBorderTRImage(&imgBordertr);
	rcPanel.width = pDesc->GetSize().width -20;
	rcPanel.height = pDesc->GetSize().height -20;
	pPanel->SetGeometry(rcPanel);
}

void CreateButton( Widget *pParent )
{
	button.LoadResource(FindResource(NULL,MAKEINTRESOURCE(IDB_PNG1),_T("PNG")),CXIMAGE_FORMAT_PNG);
	pPt = new PushButton(pParent);
	// 			DrawText(hdc2,"Hello World!",
	// 				-1,&rect,DT_SINGLELINE|DT_CENTER|DT_VCENTER);
	pPt->SetBkImage(&button, PushButton::BTN_IMAGE_3STATE);
	rc.width = pPt->GetPreferredWidth();
	rc.height = pPt->GetPreferredHeight();
	rc.x = pDesc->GetSize().width*3/4;
	rc.y = pDesc->GetSize().height/2 - 30;
	//pPt->sigClicked();
	pPt->SetGeometry(rc);
	pPt->SetUseHandCursor(true);

}

void CreateLebel( Widget *pParent )
{
	Rect rcLabel(0,0,0,0);
	pUserLabel = new Label(pParent);
	pPasswrdLabel = new Label(pParent);
	pUserLabel->SetTextColor(RGB(0,0,0));
	pPasswrdLabel->SetTextColor(RGB(0,0,0));
	pUserLabel->SetAlignment(Label::ALIGN_CENTER);
	pPasswrdLabel->SetAlignment(Label::ALIGN_CENTER);
	rcLabel.x = rcClient.right/6;
	rcLabel.y = rcClient.bottom/3;
	rcLabel.width = 100;
	rcLabel.height = 20;
	pUserLabel->SetGeometry(rcLabel);
	std::string str1 = "ÕË  ºÅ£º";
	pUserLabel->SetText(str1);

	rcLabel.y = rcClient.bottom/2;
	pPasswrdLabel->SetGeometry(rcLabel);
	std::string str2 = "ÃÜ  Âë£º";
	pPasswrdLabel->SetText(str2);

}