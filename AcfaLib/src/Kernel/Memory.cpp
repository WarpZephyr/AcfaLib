#include <Kernel/Kernel.h>
#include <stdint.h>
#include <sys/process.h>
#include "sys_imports.h"

namespace AcfaLib
{
namespace Kernel
{
namespace Memory
{

typedef int (*read_process_memory_t)(sys_pid_t pid, void* address, void* data, size_t size);
typedef int (*write_process_memory_t)(sys_pid_t pid, void* address, const void* data, size_t size);

static sys_pid_t pid = 0;
static read_process_memory_t read_process_memory = 0;
static write_process_memory_t write_process_memory = 0;

static int sys_dbg_read_process_memory(sys_pid_t pid, void* address, void* data, size_t size)
{
    system_call_4(904, (uint64_t)pid, (uint64_t)address, (uint64_t)size, (uint64_t)data);
    return_to_user_prog(int);
}

static int sys_dbg_write_process_memory(sys_pid_t pid, void* address, const void* data, size_t size)
{
    system_call_4(905, (uint64_t)pid, (uint64_t)address, (uint64_t)size, (uint64_t)data);
    return_to_user_prog(int);
}

static int ps3mapi_get_process_mem(sys_pid_t pid, void* address, void* data, size_t size)
{
    system_call_6(8, 0x7777, 0x31, (uint64_t)pid, (uint64_t)address, (uint64_t)data, (uint64_t)size);
    return_to_user_prog(int);
}

static int ps3mapi_set_process_mem(sys_pid_t pid, void* address, const void* data, size_t size)
{
    system_call_6(8, 0x7777, 0x32, (uint64_t)pid, (uint64_t)address, (uint64_t)data, (uint64_t)size);
    return_to_user_prog(int);
}

bool Start()
{
    _sys_printf("[AcfaLib::Memory] Starting...\n");
    uint8_t elf_header[4];
    static uint8_t expected[4] = { 0x7F, 'E', 'L', 'F' };

    pid = sys_process_getpid();

    int dbg_r = sys_dbg_read_process_memory(pid, (void*)0x10000, elf_header, 4);
    if (dbg_r == CELL_OK && _sys_memcmp(elf_header, expected, 4) == 0)
    {
        read_process_memory = &sys_dbg_read_process_memory;
        write_process_memory = &sys_dbg_write_process_memory;
        _sys_printf("[AcfaLib::Memory] Started.\n");
        return true;
    }

    int mapi_r = ps3mapi_get_process_mem(pid, (void*)0x10000, elf_header, 4);
    if (mapi_r == CELL_OK && _sys_memcmp(elf_header, expected, 4) == 0)
    {
        read_process_memory = &ps3mapi_get_process_mem;
        write_process_memory = &ps3mapi_set_process_mem;
        _sys_printf("[AcfaLib::Memory] Started.\n");
        return true;
    }

    return false;
}

int Read(void* dest, void* src, size_t size)
{
    return read_process_memory(pid, dest, src, size);
}

int Write(void* dest, const void* src, size_t size)
{
    return write_process_memory(pid, dest, src, size);
}

}
}
}