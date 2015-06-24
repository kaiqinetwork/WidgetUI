#pragma once

#include "Widget.h"
#include "Geometry.h"

class CxImage;

namespace wui{

class WIDGETUI_API Panel : public Widget
{
public:
	Panel(Widget *pParent);
	virtual ~Panel(void);

	Size GetPreferredSize();
	
	void SetBkCenterImage(CxImage* pImage);
	void SetBorderTLImage(CxImage* pImage);
	void SetBorderTMImage(CxImage* pImage);
	void SetBorderTRImage(CxImage* pImage);
	void SetBorderLImage(CxImage* pImage);
	void SetBorderRImage(CxImage* pImage);
	void SetBorderBLImage(CxImage* pImage);
	void SetBorderBMImage(CxImage* pImage);
	void SetBorderBRImage(CxImage* pImage);

private:
	void OnRender(XGraphics& graphic, const Rect &rcUpdate);
	
private:
	CxImage* m_pImageBorderTL;					// ×óÉÏ½ÇÍ¼Æ¬
	CxImage* m_pImageBorderTM;					// ÉÏ±ßÖÐÍ¼Æ¬
	CxImage* m_pImageBorderTR;					// ÓÒÉÏ½ÇÍ¼Æ¬
	CxImage* m_pImageBorderBL;					// ×óÏÂ½ÇÍ¼Æ¬
	CxImage* m_pImageBorderBM;					// ÏÂÖÐ±ßÍ¼Æ¬
	CxImage* m_pImageBorderBR;					// ÓÒÏÂ½ÇÍ¼Æ¬
	CxImage* m_pImageBorderL;					// ×ó±ß¿òÍ¼Æ¬
	CxImage* m_pImageBorderR;					// ÓÒ±ß¿òÍ¼Æ¬
	CxImage* m_pImageBackCenter;				// ÖÐ¼äÍ¼Æ¬
};
}
