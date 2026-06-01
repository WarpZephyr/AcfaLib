#pragma once
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/poll.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netex/errno.h>
#include <netex/net.h>
#include <netex/ifctl.h>
#include <netex/udpp2p.h>

typedef int (*accept_t)(int s, struct sockaddr* addr, socklen_t* addrlen);
typedef int (*bind_t)(int s, const struct sockaddr* addr, socklen_t addrlen);
typedef int (*connect_t)(int s, const struct sockaddr* name, socklen_t namelen);
typedef int (*getpeername_t)(int s, struct sockaddr* name, socklen_t* namelen);
typedef int (*getsockname_t)(int s, struct sockaddr* name, socklen_t* namelen);
typedef int (*getsockopt_t)(int s, int level, int optname, void* optval, socklen_t* optlen);
typedef int (*listen_t)(int s, int backlog);
typedef ssize_t (*recv_t)(int s, void* buf, size_t len, int flags);
typedef ssize_t (*recvfrom_t)(int s, void* buf, size_t len, int flags, struct sockaddr* from, socklen_t* fromlen);
typedef ssize_t (*recvmsg_t)(int s, struct msghdr* msg, int flags);
typedef ssize_t (*send_t)(int s, const void* msg, size_t len, int flags);
typedef ssize_t (*sendto_t)(int s, const void* msg, size_t len, int flags, const struct sockaddr* to, socklen_t tolen);
typedef ssize_t (*sendmsg_t)(int s, const struct msghdr* msg, int flags);
typedef int (*setsockopt_t)(int s, int level, int optname, const void* optval, socklen_t optlen);
typedef int (*shutdown_t)(int s, int how);
typedef int (*socket_t)(int domain, int type, int protocol);
typedef int (*socketclose_t)(int s);
typedef int (*socketpoll_t)(struct pollfd* fds, nfds_t nfds, int timeout);
typedef struct hostent* (*gethostbyaddr_t)(const char* addr, socklen_t len, int type);
typedef struct hostent* (*gethostbyname_t)(const char* name);
typedef int* (*_sys_net_h_errno_loc_t)(void);
typedef in_addr_t (*inet_addr_t)(const char* cp);
typedef struct in_addr (*inet_makeaddr_t)(in_addr_t net, in_addr_t lna);
typedef in_addr_t (*inet_netof_t)(struct in_addr in);
typedef in_addr_t (*inet_lnaof_t)(struct in_addr in);
typedef in_addr_t (*inet_network_t)(const char* cp);
typedef char* (*inet_ntoa_t)(struct in_addr in);
typedef int (*inet_aton_t)(const char* cp, struct in_addr* addr);
typedef const char* (*inet_ntop_t)(int af, const void* src, char* dst, socklen_t size);
typedef int (*inet_pton_t)(int af, const char* src, void* dst);
typedef int* (*_sys_net_errno_loc_t)(void);
typedef int (*sys_net_initialize_network_ex_t)(sys_net_initialize_parameter_t* param);
typedef int (*sys_net_finalize_network_t)(void);
typedef int (*sys_net_abort_socket_t)(int sockfd, int flags);
typedef int (*sys_net_abort_resolver_t)(sys_net_thread_id_t tid, int flags);
typedef int (*sys_net_open_dump_t)(int len, int flags);
typedef int (*sys_net_read_dump_t)(int id, void* buf, int len, int* pflags);
typedef int (*sys_net_close_dump_t)(int id, int* pflags);
typedef int (*sys_net_set_resolver_configurations_t)(int retrans, int retry, int flags);
typedef int (*sys_net_free_thread_context_t)(sys_net_thread_id_t tid, int flags);
typedef int (*sys_net_show_ifconfig_t)(void);
typedef int (*sys_net_show_nameserver_t)(void);
typedef int (*sys_net_show_route_t)(void);
typedef int (*sys_net_set_netemu_test_param_t)(sys_net_test_param_t* param);
typedef int (*sys_net_get_netemu_test_param_t)(sys_net_test_param_t* param);
typedef int (*sys_net_if_ctl_t)(int if_id, int code, void* ptr, int len);
typedef int (*sys_net_set_udpp2p_test_param_t)(sys_net_udpp2p_test_param_t* param);
typedef int (*sys_net_get_udpp2p_test_param_t)(sys_net_udpp2p_test_param_t* param);

#pragma pack(push, 1) // Ensures no compiler alignment padding messes up the offsets
struct SysNetFunctionTable
{
	socketpoll_t socketpoll;
	sys_net_set_udpp2p_test_param_t sys_net_set_udpp2p_test_param;
	sys_net_initialize_network_ex_t sys_net_initialize_network_ex;
	getsockname_t getsockname;
	recvfrom_t recvfrom;
	sys_net_if_ctl_t sys_net_if_ctl;
	listen_t listen;
	sys_net_show_nameserver_t sys_net_show_nameserver;
	getsockopt_t getsockopt;
	_sys_net_errno_loc_t _sys_net_errno_loc;
	socketclose_t socketclose;
	gethostbyname_t gethostbyname;
	sys_net_show_route_t sys_net_show_route;
	inet_ntoa_t inet_ntoa;
	setsockopt_t setsockopt;
	sendto_t sendto;
	socket_t socket;
	shutdown_t shutdown;
	bind_t bind;
	sys_net_show_ifconfig_t sys_net_show_ifconfig;
	sys_net_finalize_network_t sys_net_finalize_network;
	inet_addr_t inet_addr;
	gethostbyaddr_t gethostbyaddr;
	sys_net_free_thread_context_t sys_net_free_thread_context;
};
#pragma pack(pop)