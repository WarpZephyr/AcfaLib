#include <IO/IO.h>

#include <sys/ppu_thread.h>
#include <sys/timer.h>
#include "sys_imports.h"

namespace AcfaLib
{
namespace IO
{
namespace CellFsHook
{

    CellFsFunctionTable original_cell_fs_function_table = { 0 };
    volatile CellFsFunctionTable* const cell_fs_function_table = (CellFsFunctionTable*)(0x012438dc);

    static CellFsErrno hook_cellFsOpen(const char* path, int flags, int* fd, const void* arg, uint64_t size)
    {
        CellFsErrno result = original_cell_fs_function_table.cellFsOpen(path, flags, fd, arg, size);
        char buffer[128];
        if (result == CELL_FS_SUCCEEDED && fd != NULL)
        {
            _sys_printf("[AcfaLib::CellFsHook] %s cellFsOpen(path: \"%s\", flags: %s, fd: %p (%d), arg: %p, size: %llu)\n", AcfaLib::IO::Text::ToString(result), path, AcfaLib::IO::Text::OpenFlagsToString(flags, buffer, sizeof(buffer)), fd, *fd, arg, size);
        }
        else
        {
            _sys_printf("[AcfaLib::CellFsHook] %s cellFsOpen(path: \"%s\", flags: %s, fd: %p, arg: %p, size: %llu)\n", AcfaLib::IO::Text::ToString(result), path, AcfaLib::IO::Text::OpenFlagsToString(flags, buffer, sizeof(buffer)), fd, arg, size);
        }

        return result;
    }

    static CellFsErrno hook_cellFsRead(int fd, void* buf, uint64_t nbytes, uint64_t* nread)
    {
        CellFsErrno result = original_cell_fs_function_table.cellFsRead(fd, buf, nbytes, nread);
        if (result == CELL_FS_SUCCEEDED && nread != NULL)
        {
            _sys_printf("[AcfaLib::CellFsHook] %s cellFsRead(fd: %d, buf: %p, nbytes: %llu, nread: %p (%llu))\n", AcfaLib::IO::Text::ToString(result), fd, buf, nbytes, nread, *nread);
        }
        else
        {
            _sys_printf("[AcfaLib::CellFsHook] %s cellFsRead(fd: %d, buf: %p, nbytes: %llu, nread: %p)\n", AcfaLib::IO::Text::ToString(result), fd, buf, nbytes, nread);
        }

        return result;
    }

    static CellFsErrno hook_cellFsWrite(int fd, const void* buf, uint64_t nbytes, uint64_t* nwrite)
    {
        CellFsErrno result = original_cell_fs_function_table.cellFsWrite(fd, buf, nbytes, nwrite);
        if (result == CELL_FS_SUCCEEDED && nwrite != NULL)
        {
            _sys_printf("[AcfaLib::CellFsHook] %s cellFsWrite(fd: %d, buf: %p, nbytes: %llu, nwrite: %p (%llu))\n", AcfaLib::IO::Text::ToString(result), fd, buf, nbytes, nwrite, *nwrite);
        }
        else
        {
            _sys_printf("[AcfaLib::CellFsHook] %s cellFsWrite(fd: %d, buf: %p, nbytes: %llu, nwrite: %p)\n", AcfaLib::IO::Text::ToString(result), fd, buf, nbytes, nwrite);
        }

        return result;
    }

    static CellFsErrno hook_cellFsClose(int fd)
    {
        CellFsErrno result = original_cell_fs_function_table.cellFsClose(fd);
        _sys_printf("[AcfaLib::CellFsHook] %s cellFsClose(fd: %d)\n", AcfaLib::IO::Text::ToString(result), fd);
        return result;
    }

    static CellFsErrno hook_cellFsOpendir(const char* path, int* fd)
    {
        CellFsErrno result = original_cell_fs_function_table.cellFsOpendir(path, fd);
        if (result == CELL_FS_SUCCEEDED)
        {
            _sys_printf("[AcfaLib::CellFsHook] %s cellFsOpendir(path: \"%s\", fd: %p (%d))\n", AcfaLib::IO::Text::ToString(result), path, fd, *fd);
        }
        else
        {
            _sys_printf("[AcfaLib::CellFsHook] %s cellFsOpendir(path: \"%s\", fd: %p)\n", AcfaLib::IO::Text::ToString(result), path, fd);
        }

        return result;
    }

    static CellFsErrno hook_cellFsReaddir(int fd, CellFsDirent* dir, uint64_t* nread)
    {
        CellFsErrno result = original_cell_fs_function_table.cellFsReaddir(fd, dir, nread);
        if (result == CELL_FS_SUCCEEDED && dir != NULL && nread != NULL)
        {
            char buffer[384];
            _sys_printf("[AcfaLib::CellFsHook] %s cellFsReaddir(fd: %d, dir: %p (%s), nread: %p (%llu))\n", AcfaLib::IO::Text::ToString(result), fd, dir, AcfaLib::IO::Text::ToStringInline(dir, buffer, sizeof(buffer)), nread, *nread);
        }
        else
        {
            _sys_printf("[AcfaLib::CellFsHook] %s cellFsReaddir(fd: %d, dir: %p, nread: %p)\n", AcfaLib::IO::Text::ToString(result), fd, dir, nread);
        }

        return result;
    }

    static CellFsErrno hook_cellFsClosedir(int fd)
    {
        CellFsErrno result = original_cell_fs_function_table.cellFsClosedir(fd);
        _sys_printf("[AcfaLib::CellFsHook] %s cellFsClosedir(fd: %d)\n", AcfaLib::IO::Text::ToString(result), fd);
        return result;
    }

    static CellFsErrno hook_cellFsStat(const char* path, CellFsStat* sb)
    {
        CellFsErrno result = original_cell_fs_function_table.cellFsStat(path, sb);
        if (result == CELL_FS_SUCCEEDED && sb != NULL)
        {
            char buffer[512];
            _sys_printf("[AcfaLib::CellFsHook] %s cellFsStat(path: \"%s\", sb: %p (%s))\n", AcfaLib::IO::Text::ToString(result), path, sb, AcfaLib::IO::Text::ToStringInline(sb, buffer, sizeof(buffer)));
        }
        else
        {
            _sys_printf("[AcfaLib::CellFsHook] %s cellFsStat(path: \"%s\", sb: %p)\n", AcfaLib::IO::Text::ToString(result), path, sb);
        }

        return result;
    }

    static CellFsErrno hook_cellFsFstat(int fd, CellFsStat* sb)
    {
        CellFsErrno result = original_cell_fs_function_table.cellFsFstat(fd, sb);
        if (result == CELL_FS_SUCCEEDED && sb != NULL)
        {
            char buffer[512];
            _sys_printf("[AcfaLib::CellFsHook] %s cellFsFstat(fd: %d, sb: %p (%s))\n", AcfaLib::IO::Text::ToString(result), fd, sb, AcfaLib::IO::Text::ToStringInline(sb, buffer, sizeof(buffer)));
        }
        else
        {
            _sys_printf("[AcfaLib::CellFsHook] %s cellFsFstat(fd: %d, sb: %p)\n", AcfaLib::IO::Text::ToString(result), fd, sb);
        }

        return result;
    }

    static CellFsErrno hook_cellFsMkdir(const char* path, CellFsMode mode)
    {
        CellFsErrno result = original_cell_fs_function_table.cellFsMkdir(path, mode);
        char buffer[256];
        _sys_printf("[AcfaLib::CellFsHook] %s cellFsMkdir(path: \"%s\", mode: %s)\n", AcfaLib::IO::Text::ToString(result), path, AcfaLib::IO::Text::ToStringInline(mode, buffer, sizeof(buffer)));
        return result;
    }

    static CellFsErrno hook_cellFsRename(const char* from, const char* to)
    {
        CellFsErrno result = original_cell_fs_function_table.cellFsRename(from, to);
        _sys_printf("[AcfaLib::CellFsHook] %s cellFsRename(from: \"%s\", to: \"%s\")\n", AcfaLib::IO::Text::ToString(result), from, to);
        return result;
    }

    static CellFsErrno hook_cellFsRmdir(const char* path)
    {
        CellFsErrno result = original_cell_fs_function_table.cellFsRmdir(path);
        _sys_printf("[AcfaLib::CellFsHook] %s cellFsRmdir(path: \"%s\")\n", AcfaLib::IO::Text::ToString(result), path);
        return result;
    }

    static CellFsErrno hook_cellFsUnlink(const char* path)
    {
        CellFsErrno result = original_cell_fs_function_table.cellFsUnlink(path);
        _sys_printf("[AcfaLib::CellFsHook] %s cellFsUnlink(path: \"%s\")\n", AcfaLib::IO::Text::ToString(result), path);
        return result;
    }

    static CellFsErrno hook_cellFsLseek(int fd, int64_t offset, int whence, uint64_t* pos)
    {
        CellFsErrno result = original_cell_fs_function_table.cellFsLseek(fd, offset, whence, pos);
        if (result == CELL_FS_SUCCEEDED && pos != NULL)
        {
            _sys_printf("[AcfaLib::CellFsHook] %s cellFsLseek(fd: %d, offset: %lld, whence: %d, pos: %p (%llu))\n", AcfaLib::IO::Text::ToString(result), fd, offset, whence, pos, *pos);
        }
        else
        {
            _sys_printf("[AcfaLib::CellFsHook] %s cellFsLseek(fd: %d, offset: %lld, whence: %d, pos: %p)\n", AcfaLib::IO::Text::ToString(result), fd, offset, whence, pos);
        }

        return result;
    }

    static CellFsErrno hook_cellFsFGetBlockSize(int fd, uint64_t* sector_size, uint64_t* block_size)
    {
        CellFsErrno result = original_cell_fs_function_table.cellFsFGetBlockSize(fd, sector_size, block_size);
        if (result == CELL_FS_SUCCEEDED && sector_size != NULL && block_size != NULL)
        {
            _sys_printf("[AcfaLib::CellFsHook] %s cellFsFGetBlockSize(fd: %d, sector_size: %p (%llu), block_size: %p (%llu))\n", AcfaLib::IO::Text::ToString(result), fd, sector_size, *sector_size, block_size, *block_size);
        }
        else
        {
            _sys_printf("[AcfaLib::CellFsHook] %s cellFsFGetBlockSize(fd: %d, sector_size: %p, block_size: %p)\n", AcfaLib::IO::Text::ToString(result), fd, sector_size, block_size);
        }

        return result;
    }

    static CellFsErrno hook_cellFsFtruncate(int fd, uint64_t size)
    {
        CellFsErrno result = original_cell_fs_function_table.cellFsFtruncate(fd, size);
        _sys_printf("[AcfaLib::CellFsHook] %s cellFsFtruncate(fd: %d, size: %llu)\n", AcfaLib::IO::Text::ToString(result), fd, size);
        return result;
    }

    static CellFsErrno hook_cellFsAioInit(const char* mount_point)
    {
        CellFsErrno result = original_cell_fs_function_table.cellFsAioInit(mount_point);
        _sys_printf("[AcfaLib::CellFsHook] %s cellFsAioInit(mount_point: \"%s\")\n", AcfaLib::IO::Text::ToString(result), mount_point);
        return result;
    }

    static CellFsErrno hook_cellFsAioRead(CellFsAio* aio, int* id, void(*func)(CellFsAio* aio, CellFsErrno err, int id, uint64_t size))
    {
        CellFsErrno result = original_cell_fs_function_table.cellFsAioRead(aio, id, func);

        char buffer[128];
        if (result == CELL_FS_SUCCEEDED && id != NULL)
        {
            _sys_printf("[AcfaLib::CellFsHook] %s cellFsAioRead(aio: %p (%s), id: %p (%d), func: %p)\n", AcfaLib::IO::Text::ToString(result), aio, AcfaLib::IO::Text::ToStringInline(aio, buffer, sizeof(buffer)), id, *id, func);
        }
        else
        {
            _sys_printf("[AcfaLib::CellFsHook] %s cellFsAioRead(aio: %p (%s), id: %p, func: %p)\n", AcfaLib::IO::Text::ToString(result), aio, AcfaLib::IO::Text::ToStringInline(aio, buffer, sizeof(buffer)), id, func);
        }

        return result;
    }

    static CellFsErrno hook_cellFsAioWrite(CellFsAio* aio, int* id, void(*func)(CellFsAio* aio, CellFsErrno err, int id, uint64_t size))
    {
        CellFsErrno result = original_cell_fs_function_table.cellFsAioWrite(aio, id, func);

        char buffer[128];
        if (result == CELL_FS_SUCCEEDED && id != NULL)
        {
            _sys_printf("[AcfaLib::CellFsHook] %s cellFsAioWrite(aio: %p (%s), id: %p (%d), func: %p)\n", AcfaLib::IO::Text::ToString(result), aio, AcfaLib::IO::Text::ToStringInline(aio, buffer, sizeof(buffer)), id, *id, func);
        }
        else
        {
            _sys_printf("[AcfaLib::CellFsHook] %s cellFsAioWrite(aio: %p (%s), id: %p, func: %p)\n", AcfaLib::IO::Text::ToString(result), aio, AcfaLib::IO::Text::ToStringInline(aio, buffer, sizeof(buffer)), id, func);
        }

        return result;
    }

    static void Wait()
    {
        // We scan until the slot switches to RPCS3's HLE space (away from the default EBOOT stub)
        // The default value is 0x01AC33F8
        while (true)
        {
            // Check if the emulator has overwritten the slot with HLE (0x01E10C00)
            uint32_t current_val = (uint32_t)cell_fs_function_table->cellFsOpen;
            if (current_val != 0x01AC33F8 && current_val != 0)
            {
                break;
            }

            // Sleep for 1 millisecond (1000 microseconds) between checks to prevent CPU core starvation
            sys_timer_usleep(1000);
        }
    }

    static void Patch()
    {
        original_cell_fs_function_table = *(CellFsFunctionTable*)cell_fs_function_table;
        cell_fs_function_table->cellFsOpen = (cellFsOpen_t)&hook_cellFsOpen;
        cell_fs_function_table->cellFsRead = (cellFsRead_t)&hook_cellFsRead;
        cell_fs_function_table->cellFsWrite = (cellFsWrite_t)&hook_cellFsWrite;
        cell_fs_function_table->cellFsClose = (cellFsClose_t)&hook_cellFsClose;
        cell_fs_function_table->cellFsOpendir = (cellFsOpendir_t)&hook_cellFsOpendir;
        cell_fs_function_table->cellFsReaddir = (cellFsReaddir_t)&hook_cellFsReaddir;
        cell_fs_function_table->cellFsClosedir = (cellFsClosedir_t)&hook_cellFsClosedir;
        cell_fs_function_table->cellFsStat = (cellFsStat_t)&hook_cellFsStat;
        cell_fs_function_table->cellFsFstat = (cellFsFstat_t)&hook_cellFsFstat;
        cell_fs_function_table->cellFsMkdir = (cellFsMkdir_t)&hook_cellFsMkdir;
        cell_fs_function_table->cellFsRename = (cellFsRename_t)&hook_cellFsRename;
        cell_fs_function_table->cellFsRmdir = (cellFsRmdir_t)&hook_cellFsRmdir;
        cell_fs_function_table->cellFsUnlink = (cellFsUnlink_t)&hook_cellFsUnlink;
        cell_fs_function_table->cellFsLseek = (cellFsLseek_t)&hook_cellFsLseek;
        cell_fs_function_table->cellFsFGetBlockSize = (cellFsFGetBlockSize_t)&hook_cellFsFGetBlockSize;
        cell_fs_function_table->cellFsFtruncate = (cellFsFtruncate_t)&hook_cellFsFtruncate;
        cell_fs_function_table->cellFsAioInit = (cellFsAioInit_t)&hook_cellFsAioInit;
        cell_fs_function_table->cellFsAioRead = (cellFsAioRead_t)&hook_cellFsAioRead;
        cell_fs_function_table->cellFsAioWrite = (cellFsAioWrite_t)&hook_cellFsAioWrite;
    }

    static void Worker(uint64_t arg)
    {
        _sys_printf("[AcfaLib::CellFsHook] Starting...\n");
        Wait();
        Patch();

        _sys_printf("[AcfaLib::CellFsHook] Started.\n");
        sys_ppu_thread_exit(0);
    }

    bool Start()
    {
        // Spawn the background scanner thread using native cellOS parameters
        sys_ppu_thread_t thread_id;
        int priority = 1000;           // Standard medium priority
        size_t stack_size = 0x2000;    // Allocate an 8KB stack window
        uint64_t flags = 0;            // Default thread flags (SYS_PPU_THREAD_CREATE_JOINABLE)
        return sys_ppu_thread_create(&thread_id, Worker, 0, priority, stack_size, flags, "AcfaLib::CellFsHook::Worker") == CELL_OK;
    }
}
}
}