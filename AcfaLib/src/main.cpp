#include "main.h"

int start()
{
    _sys_printf("[AcfaLib] Plugin initializing...\n");
    if (AcfaLib::Kernel::Memory::Start() != true)
    {
        _sys_printf("[AcfaLib] Failed to initialize AcfaLib::Memory!\n");
        return 1;
    }

    if (AcfaLib::IO::CellFsHook::Start() != true)
    {
        _sys_printf("[AcfaLib] Failed to initialize AcfaLib::CellFsHook!\n");
        return 1;
    }

    _sys_printf("[AcfaLib] Plugin initialized.\n");
    return 0;
}