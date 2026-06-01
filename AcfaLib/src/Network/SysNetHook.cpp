#include <Network/Network.h>

#include <sys/ppu_thread.h>
#include <sys/timer.h>
#include "sys_imports.h"

namespace AcfaLib
{
namespace Network
{
namespace SysNetHook
{
    SysNetFunctionTable original_sys_net_function_table = { 0 };
    volatile SysNetFunctionTable* const sys_net_function_table = (SysNetFunctionTable*)(0x01243978);

    static int hook_socketpoll(struct pollfd* fds, nfds_t nfds, int timeout)
    {
        int result = original_sys_net_function_table.socketpoll(fds, nfds, timeout);
        _sys_printf("[AcfaLib::SysNetHook] %d socketpoll(fds: %p, nfds: %u, timeout: %d)\n", result, fds, (unsigned int)nfds, timeout);
        return result;
    }

    static int hook_sys_net_set_udpp2p_test_param(sys_net_udpp2p_test_param_t* param)
    {
        int result = original_sys_net_function_table.sys_net_set_udpp2p_test_param(param);
        _sys_printf("[AcfaLib::SysNetHook] %d sys_net_set_udpp2p_test_param(param: %p)\n", result, param);
        return result;
    }

    static int hook_sys_net_initialize_network_ex(sys_net_initialize_parameter_t* param)
    {
        int result = original_sys_net_function_table.sys_net_initialize_network_ex(param);
        _sys_printf("[AcfaLib::SysNetHook] %d sys_net_initialize_network_ex(param: %p)\n", result, param);
        return result;
    }

    static int hook_getsockname(int s, struct sockaddr* name, socklen_t* namelen)
    {
        int result = original_sys_net_function_table.getsockname(s, name, namelen);
        if (result == 0 && namelen != NULL)
        {
            _sys_printf("[AcfaLib::SysNetHook] %d getsockname(s: %d, name: %p, namelen: %p (%u))\n", result, s, name, namelen, (unsigned int)*namelen);
        }
        else
        {
            _sys_printf("[AcfaLib::SysNetHook] %d getsockname(s: %d, name: %p, namelen: %p)\n", result, s, name, namelen);
        }

        return result;
    }

    static ssize_t hook_recvfrom(int s, void* buf, size_t len, int flags, struct sockaddr* from, socklen_t* fromlen)
    {
        ssize_t result = original_sys_net_function_table.recvfrom(s, buf, len, flags, from, fromlen);
        if (result >= 0 && fromlen != NULL)
        {
            _sys_printf("[AcfaLib::SysNetHook] %ld recvfrom(s: %d, buf: %p, len: %u, flags: 0x%X, from: %p, fromlen: %p (%u))\n", (long)result, s, buf, (unsigned int)len, flags, from, fromlen, (unsigned int)*fromlen);
        }
        else
        {
            _sys_printf("[AcfaLib::SysNetHook] %ld recvfrom(s: %d, buf: %p, len: %u, flags: 0x%X, from: %p, fromlen: %p)\n", (long)result, s, buf, (unsigned int)len, flags, from, fromlen);
        }

        return result;
    }

    static int hook_sys_net_if_ctl(int if_id, int code, void* ptr, int len)
    {
        int result = original_sys_net_function_table.sys_net_if_ctl(if_id, code, ptr, len);
        _sys_printf("[AcfaLib::SysNetHook] %d sys_net_if_ctl(if_id: %d, code: 0x%X, ptr: %p, len: %d)\n", result, if_id, code, ptr, len);
        return result;
    }

    static int hook_listen(int s, int backlog)
    {
        int result = original_sys_net_function_table.listen(s, backlog);
        _sys_printf("[AcfaLib::SysNetHook] %d listen(s: %d, backlog: %d)\n", result, s, backlog);
        return result;
    }

    static int hook_sys_net_show_nameserver(void)
    {
        int result = original_sys_net_function_table.sys_net_show_nameserver();
        _sys_printf("[AcfaLib::SysNetHook] %d sys_net_show_nameserver()\n", result);
        return result;
    }

    static int hook_getsockopt(int s, int level, int optname, void* optval, socklen_t* optlen)
    {
        int result = original_sys_net_function_table.getsockopt(s, level, optname, optval, optlen);
        if (result == 0 && optlen != NULL)
        {
            _sys_printf("[AcfaLib::SysNetHook] %d getsockopt(s: %d, level: %d, optname: %d, optval: %p, optlen: %p (%u))\n", result, s, level, optname, optval, optlen, (unsigned int)*optlen);
        }
        else
        {
            _sys_printf("[AcfaLib::SysNetHook] %d getsockopt(s: %d, level: %d, optname: %d, optval: %p, optlen: %p)\n", result, s, level, optname, optval, optlen);
        }

        return result;
    }

    static int* hook__sys_net_errno_loc(void)
    {
        int* result = original_sys_net_function_table._sys_net_errno_loc();
        _sys_printf("[AcfaLib::SysNetHook] %p _sys_net_errno_loc()\n", result);
        return result;
    }

    static int hook_socketclose(int s)
    {
        int result = original_sys_net_function_table.socketclose(s);
        _sys_printf("[AcfaLib::SysNetHook] %d socketclose(s: %d)\n", result, s);
        return result;
    }

    static struct hostent* hook_gethostbyname(const char* name)
    {
        struct hostent* result = original_sys_net_function_table.gethostbyname(name);
        _sys_printf("[AcfaLib::SysNetHook] %p gethostbyname(name: \"%s\")\n", result, name ? name : "NULL");
        return result;
    }

    static int hook_sys_net_show_route(void)
    {
        int result = original_sys_net_function_table.sys_net_show_route();
        _sys_printf("[AcfaLib::SysNetHook] %d sys_net_show_route()\n", result);
        return result;
    }

    static char* hook_inet_ntoa(struct in_addr in)
    {
        char* result = original_sys_net_function_table.inet_ntoa(in);
        _sys_printf("[AcfaLib::SysNetHook] \"%s\" inet_ntoa(in.s_addr: 0x%X)\n", result ? result : "NULL", in.s_addr);
        return result;
    }

    static int hook_setsockopt(int s, int level, int optname, const void* optval, socklen_t optlen)
    {
        int result = original_sys_net_function_table.setsockopt(s, level, optname, optval, optlen);
        _sys_printf("[AcfaLib::SysNetHook] %d setsockopt(s: %d, level: %d, optname: %d, optval: %p, optlen: %u)\n", result, s, level, optname, optval, (unsigned int)optlen);
        return result;
    }

    static ssize_t hook_sendto(int s, const void* msg, size_t len, int flags, const struct sockaddr* to, socklen_t tolen)
    {
        ssize_t result = original_sys_net_function_table.sendto(s, msg, len, flags, to, tolen);
        _sys_printf("[AcfaLib::SysNetHook] %ld sendto(s: %d, msg: %p, len: %u, flags: 0x%X, to: %p, tolen: %u)\n",
            (long)result, s, msg, (unsigned int)len, flags, to, (unsigned int)tolen);
        return result;
    }

    static int hook_socket(int domain, int type, int protocol)
    {
        int result = original_sys_net_function_table.socket(domain, type, protocol);
        _sys_printf("[AcfaLib::SysNetHook] %d socket(domain: %d, type: %d, protocol: %d)\n", result, domain, type, protocol);
        return result;
    }

    static int hook_shutdown(int s, int how)
    {
        int result = original_sys_net_function_table.shutdown(s, how);
        _sys_printf("[AcfaLib::SysNetHook] %d shutdown(s: %d, how: %d)\n", result, s, how);
        return result;
    }

    static int hook_bind(int s, const struct sockaddr* addr, socklen_t addrlen)
    {
        int result = original_sys_net_function_table.bind(s, addr, addrlen);
        _sys_printf("[AcfaLib::SysNetHook] %d bind(s: %d, addr: %p, addrlen: %u)\n", result, s, addr, (unsigned int)addrlen);
        return result;
    }

    static int hook_sys_net_show_ifconfig(void)
    {
        int result = original_sys_net_function_table.sys_net_show_ifconfig();
        _sys_printf("[AcfaLib::SysNetHook] %d sys_net_show_ifconfig()\n", result);
        return result;
    }

    static int hook_sys_net_finalize_network(void)
    {
        int result = original_sys_net_function_table.sys_net_finalize_network();
        _sys_printf("[AcfaLib::SysNetHook] %d sys_net_finalize_network()\n", result);
        return result;
    }

    static in_addr_t hook_inet_addr(const char* cp)
    {
        in_addr_t result = original_sys_net_function_table.inet_addr(cp);
        if (cp != NULL)
        {
            _sys_printf("[AcfaLib::SysNetHook] 0x%X inet_addr(cp: \"%s\")\n", result, cp);
        }
        else
        {
            _sys_printf("[AcfaLib::SysNetHook] 0x%X inet_addr(cp: NULL)\n", result);
        }

        return result;
    }

    static struct hostent* hook_gethostbyaddr(const char* addr, socklen_t len, int type)
    {
        struct hostent* result = original_sys_net_function_table.gethostbyaddr(addr, len, type);
        _sys_printf("[AcfaLib::SysNetHook] %p gethostbyaddr(addr: %p, len: %u, type: %d)\n", result, (const void*)addr, (unsigned int)len, type);
        return result;
    }

    static int hook_sys_net_free_thread_context(sys_net_thread_id_t tid, int flags)
    {
        int result = original_sys_net_function_table.sys_net_free_thread_context(tid, flags);
        _sys_printf("[AcfaLib::SysNetHook] %d sys_net_free_thread_context(tid: %p, flags: 0x%X)\n", result, (void*)tid, flags);
        return result;
    }

    static void Wait()
    {
        // We scan until the slot switches to RPCS3's HLE space (away from the default EBOOT stub)
        while (true)
        {
            // Check if the emulator has overwritten the slot with HLE
            uint32_t current_val = (uint32_t)sys_net_function_table->socketpoll;
            if (current_val != 0x01AC7150 && current_val != 0)
            {
                break;
            }

            // Sleep for 1 millisecond (1000 microseconds) between checks to prevent CPU core starvation
            sys_timer_usleep(1000);
        }
    }

    static void Patch()
    {
        original_sys_net_function_table = *(SysNetFunctionTable*)sys_net_function_table;
        sys_net_function_table->socketpoll = (socketpoll_t)&hook_socketpoll;
        sys_net_function_table->sys_net_set_udpp2p_test_param = (sys_net_set_udpp2p_test_param_t)&hook_sys_net_set_udpp2p_test_param;
        sys_net_function_table->sys_net_initialize_network_ex = (sys_net_initialize_network_ex_t)&hook_sys_net_initialize_network_ex;
        sys_net_function_table->getsockname = (getsockname_t)&hook_getsockname;
        sys_net_function_table->recvfrom = (recvfrom_t)&hook_recvfrom;
        sys_net_function_table->sys_net_if_ctl = (sys_net_if_ctl_t)&hook_sys_net_if_ctl;
        sys_net_function_table->listen = (listen_t)&hook_listen;
        sys_net_function_table->sys_net_show_nameserver = (sys_net_show_nameserver_t)&hook_sys_net_show_nameserver;
        sys_net_function_table->getsockopt = (getsockopt_t)&hook_getsockopt;
        sys_net_function_table->_sys_net_errno_loc = (_sys_net_errno_loc_t)&hook__sys_net_errno_loc;
        sys_net_function_table->socketclose = (socketclose_t)&hook_socketclose;
        sys_net_function_table->gethostbyname = (gethostbyname_t)&hook_gethostbyname;
        sys_net_function_table->sys_net_show_route = (sys_net_show_route_t)&hook_sys_net_show_route;
        sys_net_function_table->inet_ntoa = (inet_ntoa_t)&hook_inet_ntoa;
        sys_net_function_table->setsockopt = (setsockopt_t)&hook_setsockopt;
        sys_net_function_table->sendto = (sendto_t)&hook_sendto;
        sys_net_function_table->socket = (socket_t)&hook_socket;
        sys_net_function_table->shutdown = (shutdown_t)&hook_shutdown;
        sys_net_function_table->bind = (bind_t)&hook_bind;
        sys_net_function_table->sys_net_show_ifconfig = (sys_net_show_ifconfig_t)&hook_sys_net_show_ifconfig;
        sys_net_function_table->sys_net_finalize_network = (sys_net_finalize_network_t)&hook_sys_net_finalize_network;
        sys_net_function_table->inet_addr = (inet_addr_t)&hook_inet_addr;
        sys_net_function_table->gethostbyaddr = (gethostbyaddr_t)&hook_gethostbyaddr;
        sys_net_function_table->sys_net_free_thread_context = (sys_net_free_thread_context_t)&hook_sys_net_free_thread_context;
    }

    static void Worker(uint64_t arg)
    {
        _sys_printf("[AcfaLib::SysNetHook] Starting...\n");
        Wait();
        Patch();

        _sys_printf("[AcfaLib::SysNetHook] Started.\n");
        sys_ppu_thread_exit(0);
    }

    bool Start()
    {
        // Spawn the background scanner thread using native cellOS parameters
        sys_ppu_thread_t thread_id;
        int priority = 1000;           // Standard medium priority
        size_t stack_size = 0x2000;    // Allocate an 8KB stack window
        uint64_t flags = 0;            // Default thread flags (SYS_PPU_THREAD_CREATE_JOINABLE)
        return sys_ppu_thread_create(&thread_id, Worker, 0, priority, stack_size, flags, "AcfaLib::SysNetHook::Worker") == CELL_OK;
    }

}
}
}