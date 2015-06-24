#include <WidgetUI/WidgetHostWindow.h>
#include <WidgetUI/Window.h>
#include <WidgetUI/Manager.h>
#include <WidgetUI/WindowManager.h>
#include <WidgetUI/Widget.h>
#include <WidgetUI/Point.h>
#include <WidgetUI/Geometry.h>
#include <WidgetUI/WindowDescription.h>
#include <WidgetUI/Graphic.h>
#include <WidgetUI/PushButton.h>
#include <WidgetUI/Panel.h>
#include "stdafx.h"
#include "ximage/ximage.h"
#include <WidgetUI/Label.h>
#include "resource.h"

#pragma once

using namespace wui;

#define MAX_LOADSTRING 100

LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM);
void	CreatPanel(Widget *pParent);
void	CreateButton(Widget *pParent);
void	CreateLebel(Widget *pParent);

TCHAR szWindowClass[MAX_LOADSTRING];			// 主窗口类名

Panel *pPanel;
Label *pUserLabel;
Label *pPasswrdLabel;
Window* pWin;
Widget *pOwner;
Manager* pMgr;
PushButton* pPt;
WindowManager* pWinmgr;
TopLevelDescription* pDesc;

RECT rect,rcClient;
Rect rc;
CxImage img,button;
CxImage imgBordertl;
CxImage imgBorderbg;
CxImage imgBorderbl;
CxImage imgBorderbm;
CxImage imgBorderbr;
CxImage imgBorderl;
CxImage imgBorderr;
CxImage imgBordertm;
CxImage imgBordertr;
