#ifndef WIDGETUI_H
#define WIDGETUI_H

#ifdef WIDGETUI_EXPORT
	#define WIDGETUI_API __declspec(dllexport)
#elif defined(WIDGETUI_STATIC)
	#define WIDGETUI_API
#else
	#define WIDGETUI_API __declspec(dllimport)
#endif

#endif