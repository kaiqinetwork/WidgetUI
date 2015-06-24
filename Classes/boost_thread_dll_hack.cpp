#include <windows.h>
#include <boost/thread/detail/tss_hooks.hpp> 

extern "C" BOOL WINAPI RawDllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID); 

namespace 
{ 
	BOOL WINAPI dll_callback(HANDLE h, DWORD dwReason, LPVOID p) 
	{ 
		if(!RawDllMain(static_cast<HINSTANCE>(h),dwReason,p)) 
			return false; 

		switch (dwReason) 
		{ 
		case DLL_THREAD_DETACH: 
			boost::on_thread_exit(); 
			break; 
		case DLL_PROCESS_DETACH: 
			boost::on_process_exit(); 
			break; 
		} 
		return true; 
	} 
} 

extern "C" 
{ 
	extern BOOL (WINAPI * const _pRawDllMain)(HANDLE, DWORD, LPVOID)=&dll_callback;
} 

namespace boost 
{ 
	void tss_cleanup_implemented() 
	{} 
} 