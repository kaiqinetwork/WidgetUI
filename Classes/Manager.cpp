#include "StdAfx.h"
#include "Manager.h"
#include "WindowManager.h"
#include "ActivationChangeMessage.h"
#include "FocusChangeMessage.h"
#include "CloseMessage.h"
#include "InputMessage.h"
#include "Keys.h"
#include "ResizeMessage.h"
#include "PointerMessage.h"
#include "InputDevice.h"

namespace wui{

Manager::Manager(void)
: m_pWindowManager(NULL), m_pMouseCaptureWidget(NULL), m_pMouseOverWidget(NULL)
{
}

Manager::~Manager(void)
{
	while (!m_rootWidgets.empty())
	{
		TopLevelWindow *window = m_rootWidgets.front();

		// The following line deletes the CL_GUIComponent, which should cause it to clean up every aspect of the window.
		// Do not add additional window clean up code here, since that would cause it not to be called if the
		// application deletes a component by itself. -- mbn 22. Oct 2008
		delete window->pWidget;
	}
}

Widget * Manager::GetCaptureWidget() const
{
	return m_pMouseCaptureWidget;
}

Widget * Manager::GetFocusedWidget()
{
	for (std::vector<TopLevelWindow *>::const_iterator iter = m_rootWidgets.begin(); iter != m_rootWidgets.end(); ++iter)
	{
		if (m_pWindowManager->HasFocus(*iter))
		{
			return (*iter)->pFocusedWidget;
		}
	}
	
	return NULL;
}

void Manager::SetCaptureWidget(Widget *pWidget, bool state)
{
	if (state)
	{
		m_pMouseCaptureWidget = pWidget;
		m_pWindowManager->CaptureMouse(GetTopLevelWindow(pWidget), true);
	}
	else
	{
		if (m_pMouseCaptureWidget == pWidget)
		{
			m_pWindowManager->CaptureMouse(GetTopLevelWindow(pWidget), false);
			m_pMouseCaptureWidget = NULL;
		}
	}
}

void Manager::RequestRepaint(const Rect &rect, Widget *pRootWidget)
{
	for (std::vector<TopLevelWindow *>::iterator iter = m_rootWidgets.begin(); iter != m_rootWidgets.end(); ++iter)
	{
		TopLevelWindow *cur = *iter;

		if (cur->pWidget == pRootWidget)
		{
			m_pWindowManager->RequestRepaint(cur, rect);
			break;
		}
	}
}

Widget * Manager::GetOwnerWidget(const Widget *pWidget)
{
	for (std::vector<TopLevelWindow *>::iterator iter = m_rootWidgets.begin(); iter != m_rootWidgets.end(); ++iter)
	{
		if ((*iter)->pWidget == pWidget)
		{
			return (*iter)->pOwner;
		}
	}
	return NULL;
}

const Widget * Manager::GetOwnerWidget(const Widget *pWidget) const
{
	for (std::vector<TopLevelWindow *>::const_iterator iter = m_rootWidgets.begin(); iter != m_rootWidgets.end(); ++iter)
	{
		if ((*iter)->pWidget == pWidget)
		{
			return (*iter)->pOwner;
		}
	}
	return NULL;
}

bool Manager::HasFocus(const Widget *pWidget) const
{
	TopLevelWindow* pToplevelWindow = GetTopLevelWindow(pWidget);
	if (pToplevelWindow->pFocusedWidget == pWidget)
		return m_pWindowManager->HasFocus(pToplevelWindow);
	else
		return false;
}

TopLevelWindow * Manager::GetTopLevelWindow(const Widget * const pWidget) const
{
	Widget* pRoot = (Widget*)pWidget;
	while (pRoot->GetParentWidget())
		pRoot = pRoot->GetParentWidget();
		
	for (std::vector<TopLevelWindow *>::const_iterator iter = m_rootWidgets.begin(); iter != m_rootWidgets.end(); ++iter)
	{
		if ((*iter)->pWidget == pRoot)
		{
			return (*iter);
		}
	}
	return NULL;
}

Widget * Manager::GetCancelWidget(Widget *pWidget/*=0*/)
{
	if (pWidget && pWidget->IsCancel())
		return pWidget;
	
	Widget* pChild = pWidget->GetFirstChild();
	while (pChild)
	{
		if (pChild->IsCancel())
			return pChild;
		
		if (pChild->HasChildWidgets())
		{
			Widget* pDef = GetCancelWidget(pChild);
			if (pDef)
			{
				return pDef;
			}
		}
		
		pChild = pChild->GetNextSibling();
	}
	
	return NULL;
}

Widget * Manager::GetDefaultWidget(Widget *pWidget/*=0*/)
{
	if (pWidget && pWidget->IsDefault())
		return pWidget;

	Widget* pChild = pWidget->GetFirstChild();
	while (pChild)
	{
		if (pChild->IsDefault())
			return pChild;

		if (pChild->HasChildWidgets())
		{
			Widget* pDef = GetDefaultWidget(pChild);
			if (pDef)
			{
				return pDef;
			}
		}

		pChild = pChild->GetNextSibling();
	}

	return NULL;
}

void Manager::AddWidget(Widget *pWidget, Widget *pOwner, TopLevelDescription desc)
{
	TopLevelWindow* pTopLevelWindow = new TopLevelWindow();
	pTopLevelWindow->pWidget = pWidget;
	pTopLevelWindow->pFocusedWidget = pWidget;
	pTopLevelWindow->pOwner = NULL;
	
	TopLevelWindow* pOwnerWindow = NULL;
	if (pOwner)
	{
		pTopLevelWindow->pOwner = pOwner;
		
		Widget* pRootOwner = pTopLevelWindow->pOwner->GetTopLevelWidget();
		for (std::vector<TopLevelWindow *>::iterator iter = m_rootWidgets.begin(); iter != m_rootWidgets.end(); ++iter)
		{
			if ((*iter)->pWidget == pRootOwner)
			{
				pOwnerWindow = *iter;
				break;
			}
		}
	}
	
	m_pWindowManager->CreateWindowHack(pTopLevelWindow, pOwnerWindow, pWidget, desc);
	m_rootWidgets.push_back(pTopLevelWindow);
}

void Manager::RemoveWidget(Widget *pWidget)
{
	if (m_pMouseCaptureWidget && m_pMouseCaptureWidget == pWidget)
	{
		m_pWindowManager->CaptureMouse(GetTopLevelWindow(m_pMouseCaptureWidget), false);
		m_pMouseCaptureWidget = NULL;
	}
	
	if (m_pMouseOverWidget && m_pMouseOverWidget == pWidget)
	{
		m_pMouseOverWidget = NULL;
	}
	
	// Remove the widgets that this widget owns
	bool bOwnerWidgetDeleted;
	do 
	{
		bOwnerWidgetDeleted = false;
		for (std::vector<TopLevelWindow *>::iterator iter = m_rootWidgets.begin(); iter != m_rootWidgets.end(); ++iter)
		{
			if ((*iter)->pOwner == pWidget)
			{
				delete (*iter)->pWidget;
				
				bOwnerWidgetDeleted = true;
				break;
			}
		}
	} while (bOwnerWidgetDeleted);
	
	// Remove this widget
	for (std::vector<TopLevelWindow *>::iterator iter = m_rootWidgets.begin(); iter != m_rootWidgets.end(); ++iter)
	{
		TopLevelWindow* pCur = *iter;
		
		if (pCur->pFocusedWidget && pCur->pFocusedWidget == pWidget)
			pCur->pFocusedWidget = NULL;
			
		if (pCur->pWidget == pWidget)
		{
			m_pWindowManager->DestroyWindow(pCur);
			m_rootWidgets.erase(iter);
			delete pCur;
			break;
		}
	}
}

void Manager::GainFocus(Widget *pWidget)
{
	TopLevelWindow* pTopLevelWindow = GetTopLevelWindow(pWidget);
	if (pTopLevelWindow == NULL)
		return;
	
	if (pTopLevelWindow->pFocusedWidget == pWidget)
		return;
	
	if (m_pWindowManager->HasFocus(pTopLevelWindow))
	{
		if (pTopLevelWindow->pFocusedWidget)
		{
			FocusChangeMessage message;
			message.SetTarget(pTopLevelWindow->pFocusedWidget);
			message.SetFocusType(FocusChangeMessage::LOSING_FOCUS);
			DispatchMessage(message);
		}
		
		pTopLevelWindow->pFocusedWidget = pWidget;
		
		if (pTopLevelWindow->pFocusedWidget)
		{
			FocusChangeMessage message;
			message.SetTarget(pTopLevelWindow->pFocusedWidget);
			message.SetFocusType(FocusChangeMessage::GAINED_FOCUS);
			DispatchMessage(message);
		}
	}
	else
	{
		pTopLevelWindow->pFocusedWidget = pWidget;
	}
}

void Manager::LooseFocus(Widget *pWidget)
{
	TopLevelWindow* pTopLevelWindow = GetTopLevelWindow(pWidget);
	if (pTopLevelWindow == NULL)
		return;

	if (pTopLevelWindow->pFocusedWidget != pWidget)
		return;
	
	if (m_pWindowManager->HasFocus(pTopLevelWindow))
	{
		if (pTopLevelWindow->pFocusedWidget)
		{
			FocusChangeMessage message;
			message.SetTarget(pTopLevelWindow->pFocusedWidget);
			message.SetFocusType(FocusChangeMessage::LOSING_FOCUS);
			DispatchMessage(message);
		}

		pTopLevelWindow->pFocusedWidget = NULL;
	}
	else
	{
		pTopLevelWindow->pFocusedWidget = NULL;
	}
}

void Manager::SetEnabled(Widget *pWidget, bool bEnable)
{
	m_pWindowManager->EnableWindow(GetTopLevelWindow(pWidget), bEnable);
}

void Manager::DispatchMessage(Message &message)
{
	DeliverMessage(message);
	
	Widget* pMessageOriginalWidget = message.GetTarget();
	
	if (message.IsConsumed() && message.GetTarget() && message.IsType(InputMessage::GetTypeName()))
	{
		while (message.GetTarget() && !message.IsConsumed())
		{
			// Translate mouse pos to parent coordinates.
			InputMessage input(message);
			InputEvent event = input.GetEvent();
			event.m_ptMouse.x += message.GetTarget()->GetGeometry().GetLeft();
			event.m_ptMouse.y += message.GetTarget()->GetGeometry().GetTop();
			input.SetEvent(event);

			message.SetTarget(message.GetTarget()->GetParentWidget());
			if (message.GetTarget())
				DeliverMessage(message);
		}
	}
	
	if (message.GetTarget() == NULL && !message.IsConsumed())
		message.SetTarget(pMessageOriginalWidget);
}

void Manager::ProcessStandardGuiKeys(Message &message)
{
	if (message.IsType(InputMessage::GetTypeName()))
	{
		InputMessage input = message;
		InputEvent e = input.GetEvent();

		if (e.m_type == InputEvent::PRESSED && 
			(e.m_id == WUI_KEY_TAB || e.m_id == WUI_KEY_LEFT || e.m_id == WUI_KEY_RIGHT || e.m_id == WUI_KEY_UP || e.m_id == WUI_KEY_DOWN))
		{
			Widget *pFocusWidget = input.GetTarget();

			if (e.m_id == WUI_KEY_TAB)
			{
				if (!e.m_shift)
					pFocusWidget->FocusNext();
				else
					pFocusWidget->FocusPrevious();
			}
			else if (e.m_id == WUI_KEY_LEFT || e.m_id == WUI_KEY_UP)
			{
				pFocusWidget->FocusNext();
			}
			else if (e.m_id == WUI_KEY_RIGHT || e.m_id == WUI_KEY_DOWN)
			{
				pFocusWidget->FocusPrevious();
			}
		}
		else if (e.m_id == WUI_KEY_RETURN && e.m_type == InputEvent::RELEASED)
		{
			Widget *pWidget = input.GetTarget();
			if (pWidget)
			{
				TopLevelWindow *pWindow = GetTopLevelWindow(pWidget);
				Widget *pDefaultedWidget = GetDefaultWidget(pWindow->pWidget);

				if (pDefaultedWidget)
				{
					input.SetTarget(pDefaultedWidget);
					DispatchMessage(input);
				}
			}
		}
		else if (e.m_id == WUI_KEY_ESCAPE)
		{
			Widget *pWidget = input.GetTarget();
			if (pWidget)
			{
				TopLevelWindow *pWindow = GetTopLevelWindow(pWidget);
				Widget *pCancelWidget = GetCancelWidget(pWindow->pWidget);
				if (pCancelWidget)
				{
					input.SetTarget(pCancelWidget);
					DispatchMessage(input);
				}
			}
		}
	}
}

void Manager::OnFocusLost(TopLevelWindow *pToplevelWindow)
{
	if (pToplevelWindow->pFocusedWidget)
	{
		FocusChangeMessage message;
		message.SetTarget(pToplevelWindow->pFocusedWidget);
		message.SetFocusType(FocusChangeMessage::LOSING_FOCUS);
		DispatchMessage(message);
	}

	ActivationChangeMessage message;
	message.SetTarget(pToplevelWindow->pFocusedWidget);
	message.SetActivationType(ActivationChangeMessage::ACTIVATION_LOST);
	DispatchMessage(message);
}

void Manager::OnFocusGained(TopLevelWindow *pToplevelWindow)
{
	if (pToplevelWindow->pFocusedWidget == NULL)
		pToplevelWindow->pFocusedWidget = pToplevelWindow->pWidget;

	if (pToplevelWindow->pFocusedWidget)
	{
		FocusChangeMessage message;
		message.SetTarget(pToplevelWindow->pFocusedWidget);
		message.SetFocusType(FocusChangeMessage::GAINED_FOCUS);
		DispatchMessage(message);
	}

	ActivationChangeMessage message;
	message.SetTarget(pToplevelWindow->pWidget);
	message.SetActivationType(ActivationChangeMessage::ACTIVATION_GAINED);
	DispatchMessage(message);
}

void Manager::OnResize(TopLevelWindow *pToplevelWindow, const Size &newSize)
{
	Widget *pWidget = pToplevelWindow->pWidget;
	pWidget->m_geometry = pWidget->GetGeometry();
	pWidget->GeometryUpdated();
	
	ResizeMessage message;
	message.SetTarget(pWidget);
	message.SetGeometry(pWidget->GetGeometry());
	DispatchMessage(message);
}

void Manager::OnPaint(TopLevelWindow *pToplevelWindow, const Rect &updateRect)
{
	pToplevelWindow->pWidget->Paint(updateRect);
}

void Manager::OnClose(TopLevelWindow *pToplevelWindow)
{
	CloseMessage message;
	message.SetTarget(pToplevelWindow->pWidget);
	DispatchMessage(message);
}

void Manager::OnInputReceived(TopLevelWindow *pToplevelWindow, const InputEvent &inputEvent, const InputState &inputState)
{
	// Find target for input message:
	Widget *pTarget = NULL;
	if (inputEvent.m_pDevice->GetType() == InputDevice::POINTER)
	{
		pTarget = m_pMouseCaptureWidget;

		if (pTarget == NULL)
			pTarget = pToplevelWindow->pWidget->GetWidgetAt(
			pToplevelWindow->pWidget->WindowToWidgetCoords(inputEvent.m_ptMouse));
	}
	else
	{
		if (pToplevelWindow->pFocusedWidget)
			pTarget = pToplevelWindow->pFocusedWidget;
		else
			pTarget = pToplevelWindow->pWidget; // All keyboard messages are redirected to the toplevel widget if no widget is focused.
	}

	if (pTarget)
	{
		// Localize mouse position:
		InputEvent localInputEvent = inputEvent;
		localInputEvent.m_ptMouse = pTarget->WindowToWidgetCoords(inputEvent.m_ptMouse);

		// For pointer movements we may need to generate enter and leave messages:
		if (localInputEvent.m_type == InputEvent::POINTER_MOVED &&
			pTarget != m_pMouseOverWidget)
		{
			if (m_pMouseOverWidget)
			{
				PointerMessage msg;
				msg.SetPointerType(PointerMessage::POINTER_LEAVE);
				msg.SetTarget(m_pMouseOverWidget);
				DispatchMessage(msg);
			}
			m_pMouseOverWidget = pTarget;
			if (m_pMouseOverWidget)
			{
				PointerMessage msg;
				msg.SetPointerType(PointerMessage::POINTER_ENTER);
				msg.SetTarget(m_pMouseOverWidget);
				DispatchMessage(msg);
			}
		}

		InputMessage message;
		message.SetTarget(pTarget);
		message.SetEvent(localInputEvent);
		DispatchMessage(message);

		if (!message.IsConsumed())
			ProcessStandardGuiKeys(message);
	}
}

void Manager::DeliverMessage(Message &message)
{
	m_sigFilterMessage(message);

	if (!message.IsConsumed())
	{
		Widget *pTarget = message.GetTarget();
		if (pTarget)
		{
			if (!message.IsConsumed())
			{
				if (!pTarget->sigProcessMessage().empty())
					pTarget->sigProcessMessage()(message);
			}

			if (!message.IsConsumed())
			{
				if (message.GetType() == ActivationChangeMessage::GetTypeName())
				{
					switch (ActivationChangeMessage(message).GetActivationType())
					{
					case ActivationChangeMessage::ACTIVATION_LOST:
						if (!pTarget->sigDeactivated().empty() && pTarget->sigDeactivated()())
							message.SetConsumed();
						break;
					case ActivationChangeMessage::ACTIVATION_GAINED:
						if (!pTarget->sigActivated().empty() && pTarget->sigActivated()())
							message.SetConsumed();
						break;
					}
				}
				else if (message.GetType() == CloseMessage::GetTypeName())
				{
					if (!pTarget->sigClose().empty() && pTarget->sigClose()())
						message.SetConsumed();
				}
				else if (message.GetType() == FocusChangeMessage::GetTypeName())
				{
					switch (FocusChangeMessage(message).GetFocusType())
					{
					case FocusChangeMessage::LOSING_FOCUS:
						if (!pTarget->sigFocusLost().empty() && pTarget->sigFocusLost()())
							message.SetConsumed();
						break;
					case FocusChangeMessage::GAINED_FOCUS:
						if (!pTarget->sigFocusGained().empty() && pTarget->sigFocusGained()())
							message.SetConsumed();
						break;
					}
				}
				else if (message.GetType() == InputMessage::GetTypeName())
				{
					InputMessage input(message);
					InputEvent e = input.GetEvent();

					if (!pTarget->sigInput().empty() && pTarget->sigInput()(e))
						message.SetConsumed();

					switch (e.m_type)
					{
					case InputEvent::PRESSED:
						if (!pTarget->sigInputPressed().empty() && pTarget->sigInputPressed()(e))
							message.SetConsumed();
						break;
					case InputEvent::RELEASED:
						if (!pTarget->sigInputReleased().empty() && pTarget->sigInputReleased()(e))
							message.SetConsumed();
						break;
					case InputEvent::DOUBLECLICK:
						if (!pTarget->sigInputDoubleClick().empty() && pTarget->sigInputDoubleClick()(e))
							message.SetConsumed();
						break;
					case InputEvent::POINTER_MOVED:
						if (!pTarget->sigInputPointerMoved().empty() && pTarget->sigInputPointerMoved()(e))
							message.SetConsumed();
						break;
					default:
						break;
					}
				}
				else if (message.GetType() == PointerMessage::GetTypeName())
				{
					switch (PointerMessage(message).GetPointerType())
					{
					case PointerMessage::POINTER_ENTER:
						if (!pTarget->sigPointerEnter().empty() && pTarget->sigPointerEnter()())
							message.SetConsumed();
						break;
					case PointerMessage::POINTER_LEAVE:
						if (!pTarget->sigPointerExit().empty() && pTarget->sigPointerExit()())
							message.SetConsumed();
						break;
					}
				}
				else if (message.GetType() == ResizeMessage::GetTypeName())
				{
					// No need to do anything here. Already handled elsewhere.
					message.SetConsumed();
				}
			}
		}
	}
}

WindowManager* Manager::GetWindowManager() const
{
	return m_pWindowManager;
}

void Manager::SetWindowManager(WindowManager* pWindowManager)
{
	m_pWindowManager = pWindowManager;
	m_pWindowManager->SetManager(this);
}

void Manager::OnDestroy(TopLevelWindow *pToplevelWindow)
{
}
}