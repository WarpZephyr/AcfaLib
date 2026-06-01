#include <cellstatus.h>
#include <sys/prx.h>
#include "main.h"

#pragma diag_suppress 77
SYS_MODULE_INFO( AcfaLib, 0, 1, 1);
SYS_MODULE_START( prx_entry );
SYS_MODULE_STOP( prx_stop );

SYS_LIB_DECLARE_WITH_STUB( LIBNAME, SYS_LIB_AUTO_EXPORT, STUBNAME );
SYS_LIB_EXPORT( _AcfaLib_export_function, LIBNAME );

// An exported function is needed to generate the project's PRX stub export library
extern "C" int _AcfaLib_export_function(void)
{
    return CELL_OK;
}

extern "C" int prx_entry(void)
{
    if (start() != 0)
    {
        return SYS_PRX_NO_RESIDENT;
    }

    return SYS_PRX_RESIDENT;
}

extern "C" int prx_stop(void)
{
    return SYS_PRX_RESIDENT;
}