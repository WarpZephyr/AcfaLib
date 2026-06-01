#pragma once
#include <cell/cell_fs.h>

namespace AcfaLib
{
namespace IO
{
namespace Text
{
    const char* ToStringInline(const CellFsMode mode, char* buffer, size_t size);
    const char* ToStringInline(const CellFsStat* stat, char* buffer, size_t size);
    const char* ToStringInline(const CellFsDirent* dir, char* buffer, size_t size);
    const char* ToStringInline(const CellFsAio* aio, char* buffer, size_t size);
    const char* OpenFlagsToString(const int flags, char* buffer, size_t size);
    const char* ToString(const CellFsErrno code);
}
}
}