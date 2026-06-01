#include <Runtime/Runtime.h>
#include "sys_imports.h"

namespace AcfaLib
{
namespace Runtime
{
namespace Debug
{
	volatile int is_paused;

	void Pause()
	{
#if _DEBUG
		_sys_printf("[AcfaLib::Debug] Paused.\n");
		is_paused = 1;
		
		_sys_printf("[AcfaLib::Debug] is_paused location: %p\n", (void*)&is_paused);
		while (is_paused)
		{
			
		}

		_sys_printf("[AcfaLib::Debug] Resumed.\n");
#endif
	}
}
}
}