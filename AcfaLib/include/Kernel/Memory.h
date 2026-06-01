#pragma once
#include <stdint.h>

namespace AcfaLib
{
namespace Kernel
{
namespace Memory
{
	bool Start();
	int Read(void* dest, void* src, size_t size);
	int Write(void* dest, const void* src, size_t size);
}
}
}