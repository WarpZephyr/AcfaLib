#pragma once
#include <stdio.h>

extern "C"
{
	void* _sys_malloc(size_t size);
	void _sys_free(void* ptr);
	void* _sys_memalign(size_t boundary, size_t size);
	int _sys_printf(const char* format, ...);
	int _sys_sprintf(char* buffer, const char* format, ...);
	int _sys_snprintf(char* buffer, int size, const char* format, ...);
	int _sys_strncasecmp(const char* s1, const char* s2, size_t n);
	char* _sys_strncat(char* dest, const char* src, size_t n);
	int _sys_strncmp(const char* s1, const char* s2, size_t n);
	char* _sys_strncpy(char* dest, const char* src, size_t n);
	int _sys_memcmp(const void* s1, const void* s2, size_t n);
	void* _sys_memcpy(void* dest, const void* src, size_t n);
	void* _sys_memset(void* m, int c, size_t n);
	void* _sys_memmove(void* str1, const void* str2, size_t n);
	int _sys_vprintf(const char* format, va_list arg);
	int _sys_vsprintf(char* str, const char* format, va_list arg);
	int _sys_vsnprintf(char* s, size_t n, const char* fmt, va_list arg);
}